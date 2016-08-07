import json
import time
import subprocess
import os

j = json.load(open("snapshot.last.txt"))

problems = j["problems"]

def load(hash, id):
    padded = '%05d' % id
    print(padded)


    mypath = "../tasks/"
    myfile = mypath + padded + ".txt"
    print(myfile)
    if os.path.isfile(myfile):
        print("file already exists")
        return None 
    try:
        out = subprocess.check_output(["curl", "--compressed", "-L", "-H", "Expect:", "-H", 'X-API-Key: 191-d1229cf42703fee94dd0df16779b0074', 'http://2016sv.icfpcontest.org/api/blob/' + str(hash)], universal_newlines=True)
        print(out)
        if "Error" in out:
            return false
        with open(myfile, "w+") as f:
            f.write(out)
        return True
    except:
        return False

for p in problems:
    hash = p["problem_spec_hash"]
    id = p["problem_id"]
    print(hash, id)
    correct = False
    while not correct:
        correct = load(hash, id)
        if correct is None:
            correct = True
            continue
        elif not correct:
            print("retry", hash, id)
        time.sleep(1.2)
