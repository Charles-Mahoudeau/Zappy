##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## app
##

import torch
import yaml
import os
import sys
import socket
from zappia.ppo_agent import PPOAgent

def getArg(arg_name):
    for i in range(len(sys.argv)):
        if sys.argv[i] == arg_name:
            if i + 1 < len(sys.argv) and not sys.argv[i + 1].startswith("-"):
                return sys.argv[i + 1]
            return None
    return None

def connectToServer(host, port, name):
    print(f"Connecting to server at {host}:{port} with name {name}")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    try:
        s.connect((host, port))
        s.sendall(f"{name}\n".encode())
        return s
    except OSError as exc:
        s.close()
        raise ConnectionError(f"Unable to connect to {host}:{port}") from exc

def printUsage():
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    sys.exit(-1)

if __name__ == "__main__":
    if len(sys.argv) != 7:
        printUsage()
    port = getArg("-p")
    if port is None:
        printUsage()
    name = getArg("-n")
    if name is None:
        printUsage()
    host = getArg("-h")
    if host is None:
        printUsage()
    if not os.path.exists("zappia/configs/model.yml"):
        print("Model config file not found")
        sys.exit(-1)
    with open("zappia/configs/model.yml", "r") as f:
        config = yaml.safe_load(f)
    PATH = config["PATH"]

    s = connectToServer(host, port, name)
    s.close()
    model = PPOAgent()

