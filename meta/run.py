#!/usr/bin/env python3

from sys import argv
import sys
import os
import subprocess
import sqlite3
import time
import tempfile
import pathlib
import itertools

def help():
    print("Executes multiple different combinations of versions and tasks in parallel.")
    print("Saves solutions in db")
    print("Available subcommands:")
    print("\tquick <version>\texecutes <version> to solve each task once")

def get_tasks():
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    result = cursor.execute("SELECT id, path FROM tasks").fetchall()
    conn.commit()
    conn.close()
    return result

def get_version(versionPath):
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    result = cursor.execute("SELECT id, path FROM versions WHERE path = ?", (versionPath,)).fetchone()
    conn.commit()
    conn.close()
    return result

def save_run(version_id, task_id, solutionPath, score):
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    cursor.execute("PRAGMA foreign_keys = ON")
    result = cursor.execute("INSERT INTO runs (id, task, version, path, score) VALUES(NULL, ?, ?, ?, ?)", (task_id, version_id, solutionPath, score))
    conn.commit()
    conn.close()

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

    children = []
    tasks = get_tasks()
    max_tasks = cpu_count()
    print("Found " + str(max_tasks) + " cores", file=sys.stderr)
    while True:
        while tasks and len(children) < max_tasks:
            task = tasks.pop()
            children.append((subprocess.Popen(["./execute.py", versionPath, task[1]], universal_newlines=True, stdout=subprocess.PIPE), task))
            print("Created a thread", file=sys.stderr)

        for ct in filter(done, children):
            children.remove(ct)
            c, t = ct
            with tempfile.NamedTemporaryFile(dir="./.icfpc/solutions", delete=False) as solution:
                solution.write(bytes(c.communicate()[0], "utf-8"))

            score = subprocess.check_output(["./evaluate.py", t[1], solution.name], universal_newlines=True)
            print("task:\t" + t[1])
            print("score:\t" + score)
            solutionPath = pathlib.Path(solution.name).relative_to(pathlib.Path().resolve())
            save_run(get_version(versionPath)[0], t[0], str(solutionPath), score)

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

