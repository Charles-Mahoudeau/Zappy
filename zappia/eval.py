##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## eval
##

import torch
import agent as DQNAgent
import yaml

with open("zappia/configs/model.yml", "r") as f:
    config = yaml.safe_load(f)
PATH = config["PATH"]

model = DQNAgent()
model.load_state_dict(torch.load(PATH, weights_only=True))
model.eval()