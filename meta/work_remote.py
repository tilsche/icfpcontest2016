#!/usr/bin/env python3

import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.client
import execute

from models import *

def get_work():
    """Get work from Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    return tuple(map(pickle.loads, proxy.get_work_pickled()))

def submit_work(task, version, constraint, seed, solution):
    """Submit work to Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    print(task, version, constraint, seed, solution)
    proxy.submit_work_pickled(*tuple(map(pickle.dumps, [task, version, constraint, seed, solution])))

PATH_GIT_REMOTE = ".icfpc/git_remote/"
REPO = "https://github.com/na-oma/test.git"
#"https://github.com/tilsche/icfpcontest2016.git"

def main():
    #pull work package
    print("pulling work package")
    task, version, constraint, seed = get_work()
    print("Work package " + str(task.path))
    #clone git
    path = PATH_GIT_REMOTE + version.reference + "/"
    if not pathlib.Path(path).exists():
        print("cloning into " + path)
        out = subprocess.check_output(["git", "clone", REPO, path], universal_newlines=True)
        print(out)
        #pull from git
        print("checkout " + version.reference)
        out = subprocess.check_output(["git", "-C", path, "checkout", version.reference], universal_newlines=True)
        print(out)
        #build
        print("building")
        out = subprocess.check_output(["./build.sh", path], universal_newlines=True)
        print(str(out))
    #execute
    print("executing " + path + "solver/build/solver")
    out = execute.execute(path + "solver/build/solver", task.path, constraint.runtime_ms, constraint.cores)
    print(out)

    submit_work(task, version, constraint, seed, out)

if __name__ == "__main__":
    main()

