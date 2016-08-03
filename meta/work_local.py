import tempfile
import pathlib
import random
import subprocess

from models import *

def get_work():
    connect()
    #let's just randomly select task, constraint, version
    #we can add queue later
    task = Task.select().order_by(fn.Random()).limit(1).get()
    version = Version.select().order_by(fn.Random()).limit(1).get()
    constraint = Constraint.select().order_by(fn.Random()).limit(1).get()
    close()
    return task, version, constraint, random.randrange(1e10)

def submit_work(task, version, constraint, seed, solution):
    #save solution
    #evaluate solution
    solutionPath = save_solution(solution)
    score = subprocess.check_output(["./evaluate.py", task.path, str(solutionPath)], universal_newlines=True)
    print("task:\t" + task.path)
    print("score:\t" + score)

    Run.create(task=task, version=version, constraint=constraint, seed=seed, path=solutionPath, score=score)

def save_solution(solution):
    with tempfile.NamedTemporaryFile(dir=".icfpc/solutions", delete=False) as f:
        f.write(bytes(solution, "utf-8"))
    return pathlib.Path(f.name).relative_to(pathlib.Path().resolve())

