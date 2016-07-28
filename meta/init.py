#!/usr/bin/env python3

from sys import argv
import sqlite3
import pathlib

def help():
    "Initialize icfpc in your current directory"
    "Creates ./.icfp/ directory with database."

def create_dir():
    p = pathlib.Path("./.icfpc/")
    if not p.exists():
        p.mkdir()

def create_subdirs():
    for s in ["./.icfpc/solutions/"]:
        p = pathlib.Path(s)
        if not p.exists():
            p.mkdir()

def create_db():
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    cursor.execute("PRAGMA foreign_keys = ON")
    cursor.execute("CREATE table IF NOT EXISTS versions ("
                        "id INTEGER PRIMARY KEY,"
                        "path TEXT UNIQUE ON CONFLICT IGNORE)")
    cursor.execute("CREATE table IF NOT EXISTS tasks ("
                        "id INTEGER PRIMARY KEY,"
                        "path TEXT UNIQUE ON CONFLICT IGNORE)")
    cursor.execute("CREATE table IF NOT EXISTS runs("
                        "id INTEGER PRIMARY KEY,"
                        "version INTEGER,"
                        "task INTEGER,"
                        "path TEXT,"
                        "score REAL,"
                        "FOREIGN KEY(version) REFERENCES versions(id),"
                        "FOREIGN KEY(task) REFERENCES tasks(id))")
    conn.commit()
    conn.close()

def main():
    create_dir()
    create_subdirs()
    create_db()

if __name__ == "__main__":
    main()

