#!/usr/bin/env python3

from sys import argv
from models import *
from tabulate import tabulate

def help():
    print("Reports information about tasks, versions, runs, scores")

def explanation():
    return "\n".join(["In each row, you can see one of your versions, in each column you see how it did on some task over multiple runs",
        "Format is",
        "\tMinimum Score / Average Score / Maximum Score",
        "over all runs of this Version\Task combination.",])

def table():
    connect()
    runs = (Run.select(Run, Task, Version, fn.Min(Run.score).alias("min"), fn.Avg(Run.score).alias("avg"), fn.Max(Run.score).alias("max"))
            .group_by(Run.task, Run.version)
            .join(Task, on=(Task.id == Run.task)).join(Version, on=(Version.id == Run.version))
            .order_by(Run.version, Run.task))

    #might find something faster
    seen = set()
    headers = ["Vers\\Task"] + [run.task.path for run in runs if run.task_id not in seen and not seen.add(run.task_id)]

    seenVersions = set()
    rows = []

    #depends heavily on ordering in query
    for run in runs:
        if run.version_id not in seenVersions:
            seenVersions.add(run.version_id)
            rows.append([run.version.reference] + [""] * (len(headers) - 1))
        rows[-1][headers.index(run.task.path)] = str(run.min) + "/" + str(run.avg) + "/" + str(run.max)
    out = tabulate(rows, headers, tablefmt="fancy_grid")
    close()
    return out

def report():
    return "\n".join([explanation(), table(),])

def main():
    if len(argv) > 1:
        help()
        return

    print(report())

if __name__ == "__main__":
    main()

