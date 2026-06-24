import time

from brain import HeuristicAI
from client import ZappyClient


def run(host, port, team):
    while True:
        client = ZappyClient(host=host, port=port, team=team)
        try:
            HeuristicAI(client).run()
        except KeyboardInterrupt:
            client._die()
            return
        time.sleep(1.0)
