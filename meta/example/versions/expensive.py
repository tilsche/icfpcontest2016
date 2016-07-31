#!/usr/bin/env python3

from sys import argv
from random import shuffle

def help():
    print("solves the list sorting problem")
    print("needs taskFile arg")

def solve(taskFile):
    "this solution randomly shuffles the input arrays until sorted"
    with open(taskFile) as t:
        solutions = []
        for l in t:
            int_array = [int(i) for i in l.split()]
            while not all(int_array[i] <= int_array[i+1]
                    for i in range(len(int_array)-1)):
                shuffle(int_array)
            solutions.append(" ".join(map(str, sorted(int_array))))
        return "\n".join(solutions)

def main():
    if len(argv) < 2:
        help()
        return
    taskFile = argv[1]
    print(solve(taskFile), end = "")

if __name__ == "__main__":
    main()

