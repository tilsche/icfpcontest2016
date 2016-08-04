#!/usr/bin/env python3

from sys import argv
import subprocess
import pathlib
import time

from models import *

def help():
    print("git repository helper")
    print("subcommands:")
    print("\twatch <constraintName> <count> <priority>\tmonitors new git tags:")
    print("\t\tadds <count> reps w/ <constraintName> for each level, w/ <prio>")

PATH_GIT = ".icfpc/git/"

def init():
    #currently assumes previous
    #git clone https://github.com/na-oma/test.git .icfpc/git/
    raise NotImplementedError("NYI")

def new_tag(tag, constraint, count, priority):
    print("Found new tag:", tag)
    connect()
    with database.atomic():
        version = Version.create(reference=tag)

        for task in Task.select():
            a = Work.enque(task, version=version, constraint=constraint, priority=priority, count=count)
    Tag.create(name=tag)
    close()

def watch(constraintName, count, priority):
    connect()
    try:
        constraint = Constraint.select().where(Constraint.name == constraintName).get()
    except Constraint.DoesNotExist:
        print("Constraint " + constraintName + " not found")
        return
    finally:
        close()

    while(True):
        #we still need to provide credentials
        out = subprocess.check_output(["git", "-C", PATH_GIT, "fetch", "--tags"], universal_newlines=True)
        tags = subprocess.check_output(["git", "-C", PATH_GIT, "tag"], universal_newlines=True)
        #find some tags
        for tag in tags.splitlines():
            if not Tag.select().where(Tag.name == tag).exists():
                new_tag(tag, constraint, count, priority)

        time.sleep(10)#10 sec interval: too heavy/may get blocked?

def main():
    if len(argv) < 2:
        help()
        return
    command = argv[1]
    if command == "watch":
        help() if len(argv) < 5 else watch(argv[2], argv[3], argv[4])
    elif command == "init":
        init()
    else:
        help()

if __name__ == "__main__":
    main()

