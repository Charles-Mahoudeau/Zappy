import os
import time

import tensordict as td_lib
import torch
from tensordict.nn import TensorDictModule
from torch import nn
from torchrl.envs.utils import ExplorationType, set_exploration_type
from torchrl.modules import MaskedCategorical, ProbabilisticActor

import env as env_module

BASE = os.path.dirname(os.path.abspath(__file__))


def _load_policy(device, obs_size, n_actions):
    weights = os.path.join(BASE, "ppo_policy.pth")
    if not os.path.exists(weights):
        weights = os.path.join(BASE, "savedModels", "ppo_policy.pth")
    if not os.path.exists(weights):
        raise SystemExit("No trained policy found (ppo_policy.pth)")

    actor_net = nn.Sequential(
        nn.LazyLinear(256, device=device), nn.Tanh(),
        nn.LazyLinear(256, device=device), nn.Tanh(),
        nn.LazyLinear(256, device=device), nn.Tanh(),
        nn.LazyLinear(n_actions, device=device),
    )
    policy = TensorDictModule(actor_net, in_keys=["observation"], out_keys=["logits"])
    dummy = td_lib.TensorDict(
        {
            "observation": torch.zeros(1, obs_size),
            "action_mask": torch.ones(1, n_actions, dtype=torch.bool),
        },
        batch_size=[1],
    )
    policy(dummy)
    policy = ProbabilisticActor(
        module=policy,
        in_keys={"logits": "logits", "mask": "action_mask"},
        out_keys=["action"],
        distribution_class=MaskedCategorical,
        return_log_prob=False,
    )
    policy.load_state_dict(torch.load(weights, map_location=device))
    policy.eval()
    return policy


def run(host, port, team):
    device = torch.device("cpu")
    obs_size = env_module.OBSERVATION_SIZE
    n_actions = len(env_module.COMMANDS)
    policy = _load_policy(device, obs_size, n_actions)

    environment = env_module.ZappyEnv(host=host, port=port, team=team)
    try:
        while True:
            td = environment.reset()
            with set_exploration_type(ExplorationType.DETERMINISTIC), torch.no_grad():
                while True:
                    out = policy(td.unsqueeze(0))
                    td.set("action", out["action"].squeeze(0))
                    nxt = environment.step(td)
                    if bool(nxt["next", "done"].item()):
                        break
                    td = nxt["next"]
            time.sleep(1.0)
    except KeyboardInterrupt:
        environment.close()
