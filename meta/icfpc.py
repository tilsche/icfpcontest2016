#!/usr/bin/python3

from sys import argv

def help():
    print("ICFPC submission and evaluation framework.")
    print("Available subcommands for icfpc:")
    print("\tadd task <path>...\tadds tasks")
    print("\tadd version <path>...\tadds program versions")
    print("\thelp\t\t\tshows this help")
    print("\tquick <version>\t\texecutes <version> to solve all tasks once")
    print("\treport\t\t\tshows summary information about tasks, solutions and scores")

def report():
    raise NotImplementedError("NYI")

def quick(version):
    raise NotImplementedError("NYI")

def add(args):
    raise NotImplementedError("NYI")

def main():
    if len(argv) < 2:
        help()
        return

    command = argv[1]
    if command == "add":
        help() if len(argv) < 4 else add(argv[2:])
    elif command == "help":
        help()
    elif command == "quick":
        help() if len(argv) < 3 else quick(argv[2])
    elif command == "report":
        report()
    else:
        help()

if __name__ == "__main__":
    main()

