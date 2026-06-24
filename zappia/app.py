import os
import sys

BASE = os.path.dirname(os.path.abspath(__file__))


def get_arg(name):
    for i, a in enumerate(sys.argv):
        if a == name and i + 1 < len(sys.argv) and not sys.argv[i + 1].startswith("-"):
            return sys.argv[i + 1]
    return None


def usage(code=84):
    print("USAGE: ./zappy_ai -p port -n name -h machine [--rl]")
    sys.exit(code)


def main():
    port = get_arg("-p")
    name = get_arg("-n")
    host = get_arg("-h") or "localhost"
    if port is None or name is None:
        usage()
    try:
        port = int(port)
    except ValueError:
        usage()

    folder = "rl" if "--rl" in sys.argv else "heuristic"
    sys.path.insert(0, os.path.join(BASE, folder))
    import runner

    runner.run(host, port, name)


if __name__ == "__main__":
    main()
