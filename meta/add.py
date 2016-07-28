#!/usr/bin/env python3

from sys import argv
import sqlite3

def help():
    print("Adds a task or a version to the database.")
    print("subcommands:")
    print("\ttask <path>...\t\tadds tasks")
    print("\tversion <path>...\tadds version")

def add_task(paths):
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    for p in paths:
        cursor.execute("INSERT INTO tasks (id, path) VALUES (NULL, ?)", (p,))
    conn.commit()
    conn.close()

def add_version(paths):
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    for p in paths:
        cursor.execute("INSERT INTO versions (id, path) VALUES (NULL, ?)", (p,))
    conn.commit()
    conn.close()

def add(args):
    if args[0] == "task":
        add_task(args[1:])
    elif args[0] == "version":
        add_version(args[1:])
    else:
        help()

def main():
    if len(argv) < 3:
        help()
        return
    add(argv[1:])

if __name__ == "__main__":
    main()

