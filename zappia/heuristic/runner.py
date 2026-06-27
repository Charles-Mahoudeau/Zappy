import threading
import time

from brain import HeuristicAI
from client import ZappyClient


def _play(client):
    try:
        HeuristicAI(client).run()
    except Exception:
        pass
    finally:
        client._die()


def run(host, port, team):
    print(f"Connecting to {host}:{port} as team '{team}'...")
    try:
        while True:
            client = ZappyClient(host=host, port=port, team=team)
            if client.connect():
                threading.Thread(target=_play, args=(client,), daemon=True).start()
                if client.slots > 0:
                    continue
            time.sleep(1.0)
    except KeyboardInterrupt:
        print("Exiting...")
        return
