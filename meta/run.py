#!/usr/bin/env python3

from sys import argv
import sys
import os
import subprocess
import time
import tempfile
import pathlib
import itertools
from models import *

def help():
    print("Executes multiple different combinations of versions and tasks in parallel.")
    print("Saves solutions in db")
    print("Available subcommands:")
    print("\tquick <version>\texecutes <version> to solve each task once")

#http://code.activestate.com/recipes/577376-simple-way-to-execute-multiple-process-in-parallel/
def cpu_count():
    ''' Returns the number of CPUs in the system
    '''
    num = 1
    if sys.platform == 'win32':
        try:
            num = int(os.environ['NUMBER_OF_PROCESSORS'])
        except (ValueError, KeyError):
            pass
    elif sys.platform == 'darwin':
        try:
            num = int(os.popen('sysctl -n hw.ncpu').read())
        except ValueError:
            pass
    else:
        try:
            num = os.sysconf('SC_NPROCESSORS_ONLN')
        except (ValueError, OSError, AttributeError):
            pass

    return num

def quick(versionPath):
    def done(c):
        return c[0].poll() is not None

    connect()
    version = Version.get(Version.path == versionPath)
    tasks = list(Task.select())
    close()

    children = []
    max_tasks = cpu_count()
    print("Found " + str(max_tasks) + " cores", file=sys.stderr)
    while True:
        while tasks and len(children) < max_tasks:
            task = tasks.pop()
            children.append((subprocess.Popen(["./execute.py", versionPath, task.path], universal_newlines=True, stdout=subprocess.PIPE), task))
            print("Created a thread for task", task.path, file=sys.stderr)

        for ct in filter(done, children):
            children.remove(ct)
            child, task = ct
            with tempfile.NamedTemporaryFile(dir="./.icfpc/solutions", delete=False) as solution:
                solution.write(bytes(child.communicate()[0], "utf-8"))

            score = subprocess.check_output(["./evaluate.py", task.path, solution.name], universal_newlines=True)
            print("task:\t" + task.path)
            print("score:\t" + score)
            solutionPath = pathlib.Path(solution.name).relative_to(pathlib.Path().resolve())

            connect()
            Run.create(task=task, version=version, path=str(solutionPath), score=score)
            close()

        if not children and not tasks:
            break
        else:
            time.sleep(0.05)#dunno if that's reasonable

def main():
    if len(argv) < 3:
        help()
        return

    command = argv[1]
    if command == "quick":
        help() if len(argv) < 3 else quick(argv[2])
    else:
        help()

if __name__ == "__main__":
    main()

