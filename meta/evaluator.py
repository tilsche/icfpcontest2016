#!/usr/bin/python3

from sys import argv

def help():
    print("Evaluates a solution for some task.")
    print("Needs two arguments: taskFile and solutionFile.")
    print("Prints score to stdout.")

def evaluate(task, solution):
    #has to be implemented according to current ICFPC
    raise NotImplementedError("NYI")

def main():
    if len(argv) < 3:
        help()
        return
    task = argv[1]
    solution = argv[2]
    print(evaluate(task, solution))

if __name__ == "__main__":
    main()

