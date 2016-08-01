#!/usr/bin/env python3

from sys import argv
import sqlite3
import subprocess
import pathlib
import time

def help():
    print("git repository helper")
    print("subcommands:")
    print("\twatch\tmonitors a git repository, when finding new tag:")
    print("\t\t\tcheckout, build, copy build, add to db, run quick")

PATH_GIT = "./.icfpc/git/"
def init():
    #currently assumes previous
    #git clone https://github.com/na-oma/test.git ./.icfpc/git/
    raise NotImplementedError("NYI")

def watch():
    while(True):
        #we still need to provide credentials
        out = subprocess.check_output(["git", "-C", PATH_GIT, "pull"], universal_newlines=True)
        print(out)
        out = subprocess.check_output(["git", "-C", PATH_GIT, "tag"], universal_newlines=True)
        print(out)
        #find some tags
        #check out new tag
        #build
        #copy build to versions
        #add to db
        #run quick
        time.sleep(10)#10 sec interval: too heavy/may get blocked?

def main():
    if len(argv) < 2:
        help()
        return
    command = argv[1]
    if command == "watch":
        watch()
    elif command == "init":
        init()
    else:
        help()

if __name__ == "__main__":
    main()

