#!/usr/bin/env python3
"""
This plugin is used to test the `rpc_command` hook.
"""
import json
from lightning import Plugin

plugin = Plugin(dynamic=False)


@plugin.hook("rpc_command")
def on_rpc_command(plugin, rpc_command, **kwargs):
    if rpc_command["method"] == "invoice":
        # Replace part of this command
        rpc_command["params"] = json.loads(rpc_command["params"])
        rpc_command["params"]["description"] = "A plugin modified this description"
        return {"replace": rpc_command}
    elif rpc_command["method"] == "listfunds":
        # Return a custom result to the command
        return {"return": {"result": ["Custom result"]}}
    elif rpc_command["method"] == "sendpay":
        # Don't allow this command to be executed
        return {"return": {"error": {"code": -1, "message": "You cannot do this"}}}
    return "continue"


plugin.run()
