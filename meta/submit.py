#!/usr/bin/env python3

import time
import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.server
import json

from models import *
import sys


def submit_with_evaluate():
    while True:
        # for run in list(Run.select().where(Run.submitted == False).join(Task).order_by(Run.score.desc()).limit(1)):
        # for run in list(Run.select().where(Run.submitted == False).join(Task).order_by(Run.score.desc()).limit(1)):

        for task in list(Task.select()):
            submitted_runs = Run.select().where(Run.task == task and Run.submitted == True).count()

            if submitted_runs > 0:
                continue

            run = Run.select().where(Run.submitted == False).order_by(Run.score.desc()).limit(1).get()

            id = task.path[:-4]
            print("id:", id)
            print("sol", run.path)
            print(run.score)

            result = submit(id, run.path)
            if result is not None:
                if result != run.score:
                    print("submit score not equal local score")
                with database.atomic():
                    for r in Run.select().where(Run.task == task):
                        r.submitted = True
                        r.save()
                    run.submitted = True
                    run.save()
            print()
            time.sleep(1.1)


def submit_all():
    #connect()
    while True:
        for run in list(Run.select().where(Run.score == None)):
            id = run.task.path[:-4]
            print("id:", id)
            print("sol", run.path)
            result = submit(id, run.path)
            if result is not None:
                run.score = result
                run.submitted = True
                run.save()
            print()
            time.sleep(1.1)
    #close()
    print("Done submitting all Runs with score NULL")

def submit_and_save(problemId, solutionPath, run):
    result = submit(problemId, solutionPath)
    if result is not None:
        run.score = result
        run.save()

def submit(problemId, solutionPath):
    while True:
        out = subprocess.check_output(["curl", "--compressed", "-L", "-H", "Expect:", "-H", 'X-API-Key: 191-d1229cf42703fee94dd0df16779b0074', "-F", 'problem_id=' + str(problemId) + '', "-F", 'solution_spec=@' + solutionPath + '', 'http://2016sv.icfpcontest.org/api/solution/submit'])
        out_str = str(out)
        print(out_str)
        if "Error" in out_str:
            time.sleep(10)
            continue
        j = json.loads(str(out, "utf-8"))
        if j["ok"] == True:
            print("Resemblance:", j["resemblance"])
            return j["resemblance"]
        else:
            print("Error: ", j)
            if "own problem" in j["error"]:
                print("own problem, saving -1.0")
                return -1.0
            elif "Invalid solution" in j["error"]:
                print("Invalid solution, saving -2.0")
                return -2.0
        time.sleep(1.1)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        submit_with_evaluate()
    elif len(sys.argv) == 3:
        submit(sys.argv[1], sys.argv[2])
    else:
        print("To submit single problem: args: problemId +  solutionPath")
