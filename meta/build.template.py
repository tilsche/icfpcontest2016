#!/usr/bin/env python3

from sys import argv

def help():
    print("Calls your build system")
    print("Needs one argument: <path_to_project>.")
    print("Prints two lines: <path_to_build_directory> and <exe_relative_to_build_dir>.")

def build(path):
    #has to be implemented according to current ICFPC
    raise NotImplementedError("NYI")

def main():
    if len(argv) < 2:
        help()
        return
    path = argv[1]
    build(path)

if __name__ == "__main__":
    main()

