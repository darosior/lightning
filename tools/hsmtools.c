#include <bitcoin/privkey.h>
#include <ccan/err/err.h>
#include <ccan/noerr/noerr.h>
#include <ccan/read_write_all/read_write_all.h>
#include <ccan/str/str.h>
#include <common/utils.h>
#include <errno.h>
#include <fcntl.h>
#include <sodium.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERROR_HSM_FILE errno
#define ERROR_USAGE 2
#define ERROR_LIBSODIUM 3

static void show_usage(void)
{
	printf("./hsmtools <method> [arguments]\n");
	printf("methods:\n");
	printf("	- decrypthsm <path/to/hsm_secret> <password>\n");
	printf("	- encrypthsm <path/to/hsm_secret> <password>\n");
	exit(0);
}

static bool ensure_hsm_secret_exists(int fd)
{
	if (fsync(fd) != 0) {
		close(fd);
		return false;
	}
	if (close(fd) != 0)
		return false;

	fd = open(".", O_RDONLY);
	if (fd < 0)
		return false;
	if (fsync(fd) != 0) {
		close(fd);
		return false;
	}

	close(fd);
	return true;
}

static int decrypt_hsm(const char *hsm_secret_path, const char *passwd)
{
	int fd;
	struct stat st;
	struct secret key, hsm_secret;
	u8 salt[16] = "c-lightning\0\0\0\0\0";
	crypto_secretstream_xchacha20poly1305_state crypto_state;
	u8 header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
	/* The cipher size is static with xchacha20poly1305. */
	u8 cipher[sizeof(struct secret) + crypto_secretstream_xchacha20poly1305_ABYTES];

	if (sodium_init() == -1)
		errx(ERROR_LIBSODIUM,
		     "Could not initialize libsodium. Not enough entropy ?");

	if (stat(hsm_secret_path, &st) != 0)
		errx(ERROR_HSM_FILE, "Could not stat hsm_secret");
	if (st.st_size <= 32)
		errx(ERROR_USAGE, "hsm_secret is not encrypted");
	fd = open(hsm_secret_path, O_RDONLY);
	if (fd < 0)
		errx(ERROR_HSM_FILE, "Could not open hsm_secret");

	if (!read_all(fd, header, crypto_secretstream_xchacha20poly1305_HEADERBYTES))
		errx(ERROR_HSM_FILE, "Could not read cipher header");
	if (!read_all(fd, cipher, sizeof(cipher)))
		errx(ERROR_HSM_FILE, "Could not read cipher body");

	/* Derive the encryption key from the password provided, and try to decrypt
	 * the cipher. */
	if (crypto_pwhash(key.data, sizeof(key.data), passwd, strlen(passwd), salt,
	                  crypto_pwhash_argon2id_OPSLIMIT_MODERATE,
	                  crypto_pwhash_argon2id_MEMLIMIT_MODERATE,
	                  crypto_pwhash_ALG_ARGON2ID13) != 0)
		errx(ERROR_LIBSODIUM, "Could not derive a key from the password.");
	if (crypto_secretstream_xchacha20poly1305_init_pull(&crypto_state, header,
	                                                    key.data) != 0)
		errx(ERROR_LIBSODIUM, "Could not initialize the crypto state");
	if (crypto_secretstream_xchacha20poly1305_pull(&crypto_state, hsm_secret.data,
	                                               NULL, 0, cipher, sizeof(cipher),
	                                               NULL, 0) != 0)
		errx(ERROR_LIBSODIUM, "Could not retrieve the seed. Wrong password ?");
	close(fd);

	/* Create a backup file, "just in case". */
	rename(hsm_secret_path, "hsm_secret.backup");
	fd = open(hsm_secret_path, O_CREAT|O_EXCL|O_WRONLY, 0400);
	if (fd < 0)
		errx(ERROR_HSM_FILE, "Could not open new hsm_secret");

	if (!write_all(fd, &hsm_secret, sizeof(hsm_secret))) {
		unlink_noerr(hsm_secret_path);
		close(fd);
		rename("hsm_secret.backup", hsm_secret_path);
		errx(ERROR_HSM_FILE,
		     "Failure writing plaintext seed to hsm_secret.");
	}

	/* Be as paranoïd as in hsmd with the file state on disk. */
	if (!ensure_hsm_secret_exists(fd)) {
		unlink_noerr(hsm_secret_path);
		rename("hsm_secret.backup", hsm_secret_path);
		errx(ERROR_HSM_FILE,
		     "Could not ensure hsm_secret existence.");
	}
	unlink_noerr("hsm_secret.backup");

	printf("Succesfully decrypted hsm_secret, be careful now :-).\n");
	return 0;
}

static int encrypt_hsm(const char *hsm_secret_path, const char *passwd)
{
	int fd;
	struct stat st;
	struct secret key, hsm_secret;
	u8 salt[16] = "c-lightning\0\0\0\0\0";
	crypto_secretstream_xchacha20poly1305_state crypto_state;
	u8 header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
	/* The cipher size is static with xchacha20poly1305. */
	u8 cipher[sizeof(struct secret) + crypto_secretstream_xchacha20poly1305_ABYTES];

	if (sodium_init() == -1)
		errx(ERROR_LIBSODIUM,
		     "Could not initialize libsodium. Not enough entropy ?");

	if (stat(hsm_secret_path, &st) != 0)
		errx(ERROR_HSM_FILE, "Could not stat hsm_secret");
	if (st.st_size > 32)
		errx(ERROR_USAGE, "hsm_secret is already encrypted");
	fd = open(hsm_secret_path, O_RDONLY);
	if (fd < 0)
		errx(ERROR_HSM_FILE, "Could not open hsm_secret");
	if (!read_all(fd, &hsm_secret, sizeof(hsm_secret)))
		errx(ERROR_HSM_FILE, "Could not read hsm_secret");

	/* Derive the encryption key from the password provided, and try to encrypt
	 * the seed. */
	if (crypto_pwhash(key.data, sizeof(key.data), passwd, strlen(passwd), salt,
	                  crypto_pwhash_argon2id_OPSLIMIT_MODERATE,
	                  crypto_pwhash_argon2id_MEMLIMIT_MODERATE,
	                  crypto_pwhash_ALG_ARGON2ID13) != 0)
		errx(ERROR_LIBSODIUM, "Could not derive a key from the password.");
	if (crypto_secretstream_xchacha20poly1305_init_push(&crypto_state, header,
	                                                    key.data) != 0)
		errx(ERROR_LIBSODIUM, "Could not initialize the crypto state");
	if (crypto_secretstream_xchacha20poly1305_push(&crypto_state, cipher,
	                                               NULL, hsm_secret.data,
	                                               sizeof(hsm_secret.data),
	                                               NULL, 0, 0) != 0)
		errx(ERROR_LIBSODIUM, "Could not encrypt the seed.");

	/* Create a backup file, "just in case". */
	rename(hsm_secret_path, "hsm_secret.backup");
	fd = open(hsm_secret_path, O_CREAT|O_EXCL|O_WRONLY, 0400);
	if (fd < 0)
		errx(ERROR_HSM_FILE, "Could not open new hsm_secret");

	/* Write the encrypted hsm_secret. */
	if (!write_all(fd, header, sizeof(header))
		|| !write_all(fd, cipher, sizeof(cipher))) {
		unlink_noerr(hsm_secret_path);
		close(fd);
		rename("hsm_secret.backup", hsm_secret_path);
		errx(ERROR_HSM_FILE, "Failure writing cipher to hsm_secret.");
	}

	/* Be as paranoïd as in hsmd with the file state on disk. */
	if (!ensure_hsm_secret_exists(fd)) {
		unlink_noerr(hsm_secret_path);
		rename("hsm_secret.backup", hsm_secret_path);
		errx(ERROR_HSM_FILE, "Could not ensure hsm_secret existence.");
	}
	unlink_noerr("hsm_secret.backup");

	printf("Succesfully encrypted hsm_secret. You'll now have to pass the "
	       "--encrypted-hsm startup option.\n");
	return 0;
}

int main(int argc, char *argv[])
{
	const char *method;

	setup_locale();
	err_set_progname(argv[0]);

	method = argv[1];
	if (!method)
		show_usage();

	if (streq(method, "decrypthsm")) {
		if (!argv[2] || !argv[3])
			show_usage();
		return decrypt_hsm(argv[2], argv[3]);
	}

	if (streq(method, "encrypthsm")) {
		if (!argv[2] || !argv[3])
			show_usage();
		return encrypt_hsm(argv[2], argv[3]);
	}

	show_usage();
}
