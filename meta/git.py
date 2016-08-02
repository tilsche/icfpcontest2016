#!/usr/bin/env python3

from sys import argv
import subprocess
import pathlib
import time

from models import *

def help():
    print("git repository helper")
    print("subcommands:")
    print("\twatch\tmonitors a git repository, when finding new tag:")
    print("\t\t\tcheckout, build, copy build, add to db, run quick")

PATH_GIT = ".icfpc/git/"

def init():
    #currently assumes previous
    #git clone https://github.com/na-oma/test.git .icfpc/git/
    raise NotImplementedError("NYI")

def new_tag(tag):
    print("Found new tag:", tag)
    #check out new tag
    out = subprocess.check_output(["git", "-C", PATH_GIT, "checkout", "tags/" + tag], universal_newlines=True)
    #build
    out = subprocess.check_output(["./build.py", PATH_GIT], universal_newlines=True)
    #copy build to versions
    oldpath, oldexe = out.splitlines()
    print(oldpath, oldexe)
    print(str(pathlib.Path(oldpath).resolve()))
    path = ".icfpc/versions/" + "tag_" + tag + "/"
    out = subprocess.check_output(["cp", "-rd", str(pathlib.Path(oldpath).resolve()), path], universal_newlines=True)
    #add to db
    print("Version:", path + oldexe)
    version = Version.create(path=path + oldexe)
    #run quick
    out = subprocess.check_output(["./run.py", "quick", version.path], universal_newlines=True)
    print(out)
    Tag.create(name=tag)

def watch():
    while(True):
        #we still need to provide credentials
        out = subprocess.check_output(["git", "-C", PATH_GIT, "fetch", "--tags"], universal_newlines=True)
        tags = subprocess.check_output(["git", "-C", PATH_GIT, "tag"], universal_newlines=True)
        #find some tags
        for tag in tags.splitlines():
            if not Tag.select().where(Tag.name == tag).exists():
                new_tag(tag)

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

