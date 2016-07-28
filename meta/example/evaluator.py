#!/usr/bin/python3

from sys import argv
from itertools import zip_longest, starmap

def quantify(iterable, pred=bool):
    "Count how many times the predicate is true"
    return sum(map(pred, iterable))

def help():
    print("has to give two args: task and solution")

def score_line(expected, actual):
    "compute the difference of each element"
    if actual is None:
        actual = [0] * len(expected)
    return - sum(starmap(lambda x, y : abs(x - y), zip_longest(expected, actual, fillvalue = 0)))

def evaluate(task, solution):
    #has to be implemented according to current ICFPC
    #the contest this year is to sort lists of integers :)
    with open(task) as t, open(solution) as s:
        score = 0
        for lT, lS in zip_longest(t, s):
            expected = sorted([int(i) for i in lT.split()])
            if lS is None:
                actual = [0] * len(expected)
            else:
                actual = [int(i) for i in lS.split()]
            score += score_line(expected, actual)
        return score

def main():
    if len(argv) < 3:
        help()
        return
    task = argv[1]
    solution = argv[2]
    print(evaluate(task, solution))

if __name__ == "__main__":
    main()

