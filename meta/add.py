#!/usr/bin/env python3

from sys import argv
from models import *

def help():
    print("Adds a task or a version to the database.")
    print("subcommands:")
    print("\ttask <path>...\t\tadds tasks")
    print("\tversion <path>...\tadds version")

def add_task(paths):
    connect()
    for p in paths:
        Task.create(path=p)
    close()

def add_version(paths):
    connect()
    for p in paths:
        Version.create(path=p)
    close()

def add(args):
    if args[0] == "task":
        add_task(args[1:])
    elif args[0] == "version":
        add_version(args[1:])
    else:
        help()

def main():
    if len(argv) < 3:
        help()
        return
    add(argv[1:])

if __name__ == "__main__":
    main()

