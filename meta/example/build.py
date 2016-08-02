#!/usr/bin/env python3

from sys import argv
import pathlib
import subprocess

def help():
    print("Calls your build system")
    print("Needs one argument: <path_to_project>.")
    print("Prints two lines: <path_to_build_directory> and <exe_relative_to_build_dir>.")

def build(path):
    #has to be implemented according to current ICFPC
    p = pathlib.Path(path + "build/")
    if not p.exists():
        p.mkdir()
    print(p)
    out = subprocess.check_output(["cp", "example/versions/shuffle.py", str(p)], universal_newlines=True)
    print("shuffle.py")

def main():
    if len(argv) < 2:
        help()
        return
    path = argv[1]
    build(path)

if __name__ == "__main__":
    main()

