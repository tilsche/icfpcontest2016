#!/usr/bin/env python3

from sys import argv
import subprocess
import sqlite3
import time
import tempfile
import pathlib

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
    result = cursor.execute("INSERT INTO runs (id, task, version, path, score) VALUES(NULL, ?, ?, ?, ?)", (task_id, version_id, solutionPath, score))
    conn.commit()
    conn.close()

def quick(versionPath):
    children = []
    tasks = get_tasks()
    for t in tasks:
       children.append(subprocess.Popen(["./execute.py", versionPath, t[1]], universal_newlines=True, stdout=subprocess.PIPE))
    print("done with creating threads")
    #sequentially waiting, bad for already using early results 
    for c,t in zip(children, tasks):
        out = c.communicate()[0]
        solution = tempfile.NamedTemporaryFile(dir="./.icfpc/solutions", delete=False)
        solution.write(bytes(out, "utf-8"))
        solution.close()
        score = subprocess.check_output(["./evaluate.py", t[1], solution.name], universal_newlines=True)
        print("task:\t" + t[1])
        print("score:\t" + score)
        solutionPath = pathlib.Path(solution.name).relative_to(pathlib.Path().resolve())
        save_run(get_version(versionPath)[0], t[0], str(solutionPath), score)

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

