##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## app
##

import torch
import agent as DQNAgent
import yaml
import os
import sys
import socket

def getArg(arg_name):
    for i in range(len(sys.argv)):
        if sys.argv[i] == arg_name:
            return sys.argv[i + 1]
    return None

def connectToServer(host, port, name):
    print(f"Connecting to server at {host}:{port} with name {name}")

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, int(port)))
    s.sendall(f"{name}\n".encode())
    return s


if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(-1)
    port = getArg("-p")
    if port is None:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(-1)
    name = getArg("-n")
    if name is None:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(-1)
    host = getArg("-h")
    if host is None:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(-1)
    if not os.path.exists("zappia/configs/model.yml"):
        print("Model config file not found")
        sys.exit(-1)
    with open("zappia/configs/model.yml", "r") as f:
        config = yaml.safe_load(f)
    PATH = config["PATH"]

    s = connectToServer(host, port, name)

    model = DQNAgent()

