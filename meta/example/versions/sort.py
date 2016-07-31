#!/usr/bin/env python3

from sys import argv

def help():
    print("solves the list sorting problem")
    print("needs taskFile arg")

def solve(taskFile):
    "this solution sorts the input arrays"
    with open(taskFile) as t:
        solutions = []
        for l in t:
            int_array = [int(i) for i in l.split()]
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

