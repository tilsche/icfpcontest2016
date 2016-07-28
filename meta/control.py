#!/usr/bin/python3

from sys import argv
import subprocess
import sqlite3
import time
import tempfile

def help():
    print("Executes multiple different combinations of versions and tasks.")
    print("Saves solutions in db")
    print("Available subcommands:")
    print("\tquick <version>\texecutes <version> to solve each task once")

def tasks():
    conn = sqlite3.connect("db.db")
    cursor = conn.cursor()
    result = cursor.execute("SELECT rowid, task FROM tasks").fetchall()
    conn.commit()
    conn.close()
    return result

def save_run(versionPath, task, solution, score):
    conn = sqlite3.connect("db.db")
    cursor = conn.cursor()
    result = cursor.execute("INSERT INTO runs (task, version, solution, score) VALUES(?, ?, ?, ?)", (task, versionPath, solution, score))
    conn.commit()
    conn.close()

def quick(versionPath):
    children = []
    for t in tasks():
       children.append((subprocess.Popen(["./executor.py", versionPath, t[1]], universal_newlines=True, stdout=subprocess.PIPE), t))
    print("done with creating threads")
    #sequentially waiting, bad for already using early results 
    for c in children:
        print(c[0].wait())
        out = c[0].communicate()[0]
        solution = tempfile.NamedTemporaryFile(dir="./solutions", delete=False)
        solution.write(bytes(out, "utf-8"))
        solution.close()
        score = subprocess.check_output(["./evaluator.py", c[1][1], solution.name], universal_newlines=True)
        print("score:" + score)
        save_run(versionPath, c[1][1], solution.name, score)

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

