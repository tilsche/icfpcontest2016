#!/usr/bin/env python3

from sys import argv
import pathlib
import models

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
    models.connect()
    models.create_tables()
    models.close()

def main():
    create_dir()
    create_subdirs()
    create_db()

if __name__ == "__main__":
    main()

