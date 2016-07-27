#!/usr/bin/python3

from sys import argv
import sqlite3

def help():
    print("Adds a task or a version to the database.")
    print("subcommands:")
    print("\ttask <path>...\tadds tasks")
    print("\tversion <path>...\tadds version")

def add_task(paths):
    conn = sqlite3.connect("db.db")
    cursor = conn.cursor()
    cursor.executemany("INSERT INTO tasks (task) VALUES (?)", paths)
    conn.commit()
    conn.close()

def add_version(paths):
    conn = sqlite3.connect("db.db")
    cursor = conn.cursor()
    cursor.executemany("INSERT INTO versions (version) VALUES (?)", paths)
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

