lightning-plugin -- Manage plugins with RPC
===========================================

SYNOPSIS
--------

**plugin** command \[parameter\] \[second\_parameter\]

DESCRIPTION
-----------

The **plugin** RPC command allows to manage plugins without having to
restart lightningd. It takes 1 to 3 parameters: a command
(start/stop/startdir/rescan/list) which describes the action to take and
optionally one or two parameters which describes the plugin on which the
action has to be taken.

The *start* command takes a path as the first parameter and will load the
plugin available from this path. It also takes an optional second parameter
which allows to set the time in seconds to wait for the plugin to complete
the handshake with `lightningd`.

The *stop* command takes a plugin name as parameter. It will kill and
unload the specified plugin.

The *startdir* command takes a directory path as first parameter and will
load all plugins this directory contains. It also takes a second parameter
which allows to set the time in seconds to wait for __each__ plugin to
complete the handshake with `lightningd`.

The *rescan* command starts all not-already-loaded plugins from the
default plugins directory (by default *~/.lightning/plugins*).

The *list* command will return all the active plugins.

RETURN VALUE
------------

On success, all subcommands but *stop* return an array *plugins* of
objects, one by plugin.
Each object contains the name of the plugin (*name* field) and its
status (*active* boolean field). Since plugins are configured
asynchronously, a freshly started plugin may not appear immediately.

On error, the reason why the action could not be taken upon the
plugin is returned.

AUTHOR
------

Antoine Poinsot <<darosior@protonmail.com>> is mainly responsible.

RESOURCES
---------

Main web site: <https://github.com/ElementsProject/lightning>
