#!/usr/bin/env python3

import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.server

from models import *
import sys

def submit(problemId, solutionPath):
    out = subprocess.check_output(["curl", "--compressed", "-L", "-H", "Expect:", "-H", 'X-API-Key: 191-d1229cf42703fee94dd0df16779b0074', "-F", 'problem_id=' + str(problemId) + '', "-F", 'solution_spec=@' + solutionPath + '', 'http://2016sv.icfpcontest.org/api/solution/submit'])
    print(out)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("problemId +  solutionPath")
    else:
        submit(sys.argv[1], sys.argv[2]) 
