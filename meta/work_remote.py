#!/usr/bin/env python3

import time
import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.client
import execute
import sys
import os

from models import *

#http://code.activestate.com/recipes/577376-simple-way-to-execute-multiple-process-in-parallel/
def cpu_count():
    """Returns the number of CPUs in the system"""
    num = 1
    if sys.platform == 'win32':
        try:
            num = int(os.environ['NUMBER_OF_PROCESSORS'])
        except (ValueError, KeyError):
            pass
    elif sys.platform == 'darwin':
        try:
            num = int(os.popen('sysctl -n hw.ncpu').read())
        except ValueError:
            pass
    else:
        try:
            num = os.sysconf('SC_NPROCESSORS_ONLN')
        except (ValueError, OSError, AttributeError):
            pass

    return num

def get_work(cores_max):
    """Get work from Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    return tuple(map(pickle.loads, proxy.get_work_pickled(cores_max)))

def submit_work(task, version, constraint, seed, solution, resemblance):
    """Submit work to Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    print(task, version, constraint, seed, solution)
    proxy.submit_work_pickled(*tuple(map(pickle.dumps, [task, version, constraint, seed, solution, resemblance])))

PATH_GIT_REMOTE = ".icfpc/git_remote/"
#REPO = "https://github.com/na-oma/test.git"
REPO = "git@github.com:tilsche/icfpcontest2016.git"

def clone_and_build(version):
    #clone git + build
    path = PATH_GIT_REMOTE + version.reference + "/"
    if not pathlib.Path(path).exists():
        print("cloning into " + path)
        out = subprocess.check_output(["git", "clone", REPO, path], universal_newlines=True)
        print(out)
        print("checkout " + version.reference)
        out = subprocess.check_output(["git", "-C", path, "checkout", version.reference], universal_newlines=True)
        print(out)
        print("building")
        print(path)
        out = subprocess.check_output(["./build.sh", str(path)], universal_newlines=True)
        print(str(out))
    return path

def main():
    def done(c):
        return c[0].poll() is not None

    children = []
    cores_max = cpu_count()
    cores_used = 0
    while True:
        if cores_used >= cores_max:
            #check if something completed
            print("checking for completed threads")
            for cw in filter(done, children):
                children.remove(cw)
                cores_used -= 1
                child, work = cw
                task, version, constraint, seed = work
                if child.poll() == 0:
                    out = child.communicate()[0]

                    resemblance = None

                    solution = ""

                    for line in out.split("\n"):
                        if resemblance is None:
                            resemblance = line
                        else:
                            solution += "\n" + line

                    print(out)
                    print("Submit")
                    submit_work(task, version, constraint, seed, solution, Float(resemblance))
                else:
                    print("child did not complete!, not submitting")
                    #print("\t\t\t\tcalcing einheitsquadrat")
                    #print("\t\t\t\tcalcing einheitsquadrat")
                    #print("\t\t\t\tcalcing einheitsquadrat")
                    #print("\t\t\t\tcalcing einheitsquadrat")
                    #print("\t\t\t\tcalcing einheitsquadrat")
                    #children.append((subprocess.Popen(["./execute.py", PATH_GIT_REMOTE + "3937fe9f7d3c185f26f0f105a97875c2df6102a5" + "/" + "solver/build/solver", "../tasks/" + task.path, str(constraint.runtime_ms), str(constraint.cores), str(seed)], universal_newlines=True, stdout=subprocess.PIPE), work))
                    #cores_used += 1

            #wait till something completes
            time.sleep(1)
        else:
            print("pulling work package")
            work = get_work(cores_max)
            task, version, constraint, seed = work

            print("Work package " + str(task.path))
            path = clone_and_build(version)

            print("executing " + path + "solver/build/solver")
            print("Constraint: " + str(constraint.runtime_ms) + ", " + str(constraint.cores))

            children.append((subprocess.Popen(["./execute.py", path + "solver/build/solver", "../tasks/" + task.path, str(constraint.runtime_ms), str(constraint.cores), str(seed)], universal_newlines=True, stdout=subprocess.PIPE), work))
            print("Created a thread for task", task.path, file=sys.stderr)
            print(children[0])
            cores_used += 1

if __name__ == "__main__":
    main()
