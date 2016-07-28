#!/usr/bin/env python3

from sys import argv
import subprocess

def help():
    print("Executes a program on some task, given some constraints.")
    print("Needs two arguments: programFile and taskPath.")
    print("Prints solution to stdout.")

def execute(programFile, taskFile):
    return subprocess.check_output([programFile, taskFile], timeout=60*2, universal_newlines=True)

def main():
    if len(argv) < 3:
        help()
        return
    programFile = argv[1]
    taskFile = argv[2]
    print(execute(programFile, taskFile), end = "")

if __name__ == "__main__":
    main()

