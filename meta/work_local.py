#!/usr/bin/env python3

import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.server
import submit
import time

from models import *

def get_work(cores_max):
    """Returns Task, Version, Constraint, seed"""

    connect()
    #try queue
    print(cores_max)
    work = Work.deque(cores_max)
    if work is not None:
        close()
        return work.task, work.version, work.constraint, random.randrange(1e10)
    #randomly select task, constraint, version
    print("Selecting random Work")
    task = Task.select().order_by(fn.Random()).limit(1).get()
    version = Version.select().order_by(fn.Random()).limit(1).get()
    constraint = Constraint.select().order_by(fn.Random()).limit(1).get()
    close()
    return task, version, constraint, random.randrange(1e10)

def submit_work(task, version, constraint, seed, solution):
    """Accepts Task, Version, Constraint, seed, solution
    Saves the solution to DB
    """

    with tempfile.NamedTemporaryFile(dir=".icfpc/solutions", delete=False) as f:
        f.write(bytes(solution, "utf-8"))
    solutionPath = pathlib.Path(f.name).relative_to(pathlib.Path().resolve())

    #score = subprocess.check_output(["./evaluate.py", "../tasks/" + task.path, str(solutionPath)], universal_newlines=True)

    print("version:\t" + version.reference)
    print("task:\t" + task.path)
    #print("score:\t" + score)
    print("score will be NULL, we evaluate later")

    connect()
    Run.create(task=task, version=version, constraint=constraint, seed=seed, path=solutionPath, score=None)
    close()

    submit.submit(task.path[:-4], str(solutionPath))
    time.sleep(1)

def serve():
    """Starts a server listening for Slaves requesting or submitting work"""

    def get_work_pickled(cores_max):
        return tuple(map(pickle.dumps, get_work(cores_max)))
    def submit_work_pickled(*args):
        submit_work(*tuple(map(pickle.loads, args)))
        return True
    #pickling over xml over rpc, yeah
    #we need to pickle because xmlrpcserver only understands few types
    #Dunno if this server can serve multiple requests concurrently
    server = xmlrpc.server.SimpleXMLRPCServer(("localhost", 8000), use_builtin_types=True)
    print("Listening on port 8000...")
    server.register_function(get_work_pickled, "get_work_pickled")
    server.register_function(submit_work_pickled, "submit_work_pickled")
    server.serve_forever()

if __name__ == "__main__":
    serve()

