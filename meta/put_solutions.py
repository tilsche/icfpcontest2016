from models import *
from shutil import copyfile

runs = Run.select(Run, Version).where(Version.reference == "8ff68f9ec1e8f3552e48fa50cea4dd2adea976f7").join(Version)



print(runs)
for run in runs:
    print(run.path)
    print(run.task.path)
    padded = run.task.path.zfill(9)
    print(padded)
    copyfile(run.path, "../solutions/" + padded)
     


