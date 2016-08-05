#!/usr/bin/env python3

from sys import argv
import subprocess

def help():
    print("Executes a program on some task, given some constraints.")
    print("\t Arguments: <programFile> <taskPath> <runtime_ms> <cores>")
    print("Prints solution to stdout.")

def execute(programFile, taskFile, runtime_ms, cores, seed):
    #change timeout to current ICFPC rules
    #if your program does not print just one solution at the end
    #  but continually spits out solutions, you should just print the best solution
    return subprocess.check_output([programFile, taskFile, "--runtime", str(runtime_ms), "--cores", str(cores)], timeout=runtime_ms/1000, universal_newlines=True)

def main():
    if len(argv) < 6:
        help()
        return
    print(execute(argv[1], argv[2], argv[3], argv[4], argv[5]), end = "")

if __name__ == "__main__":
    main()

