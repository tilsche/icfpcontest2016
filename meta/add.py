#!/usr/bin/env python3

from sys import argv
from models import *

def help():
    print("Adds a task, a version or a constraint to the database.")
    print("subcommands:")
    print("\ttask <path>...\t\t\tadds tasks")
    print("\tversion <path>...\t\tadds version")
    print("\tconstraint <name> <runtime_ms> <cores>\tadds constraint")

def add_task(paths):
    connect()
    for p in paths:
        Task.create(path=p)
    close()

def add_version(paths):
    connect()
    for p in paths:
        Version.create(reference=p)
    close()

def add_constraint(name, runtime_ms, cores):
    connect()
    Constraint.create(runtime_ms=runtime_ms, cores=cores, name=name)
    close()

def add(args):
    if args[0] == "task":
        add_task(args[1:])
    elif args[0] == "version":
        add_version(args[1:])
    elif args[0] == "constraint":
        help() if len(args) < 4 else add_constraint(args[1], args[2], args[3])
    else:
        help()

def main():
    if len(argv) < 3:
        help()
        return
    add(argv[1:])

if __name__ == "__main__":
    main()

