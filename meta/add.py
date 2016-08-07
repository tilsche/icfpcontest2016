#!/usr/bin/env python3

from sys import argv
from models import *
from os import listdir
from os.path import isfile, join

def help():
    print("Adds a task, a version or a constraint to the database.")
    print("subcommands:")
    print("\ttask <path>...\t\t\tadds tasks")
    print("\ttask_dir <path>\t\t add all files in <path> as task")
    print("\tversion <path>...\t\tadds version")
    print("\tconstraint <name> <runtime_ms> <cores>\tadds constraint")
    print("\twork all <reference> <constraintName> <count> <priority> for all lvls")
    print("\twork imperfect <reference> <constraintName> <count> <priority> for all imperfect lvls")

def add_task(paths):
    connect()
    for p in paths:
        Task.create(path=p)
    close()

def add_task_dir(paths):
    connect()
    for p in paths:
        tasks = set([t.path for t in Task.select()])
        print("Found", len(tasks), "task files.")

        files = [str(f) for f in listdir(p) if isfile(join(p, f)) and f not in tasks]
        print("Found", len(files), " tasks in DB")
        files_dict = [{"path": f} for f in files]

        with database.atomic():
           for idx in range(0, len(files_dict), 100):
               Task.insert_many(files_dict[idx:idx+100]).execute()
    close()

def add_version(paths):
    connect()
    for p in paths:
        Version.create(reference=p)
    close()

def add_constraint(name, runtime_ms, cores):
    connect()
    Constraint.create(runtime_ms=runtime_ms, cores=cores, name=name)
    close()

def add_work(args):
    if args[0] == "imperfect":
        help() if len(args) < 5 else add_work_imperfect(args[1], args[2], args[3], args[4])
    elif args[0] == "all":
        help() if len(args) < 5 else add_work_all(args[1], args[2], args[3], args[4])
    else:
        help()

def add_work_imperfect(reference, constraint, count, priority):
    print("JOIN DOES NOT WORK YET :(")
    connect()
    version=Version.get(reference=reference)
    constraint=Constraint.get(name=constraint)
    #we use list() to make faster?
    tasks = Task.select().where(Run.score != 1.0 or Run << None).group_by(Task).join(Run)
    print("tasks.count", tasks.count())
    return
    Work.enque(tasks, version, constraint, priority, count)
    close()

def add_work_all(reference, constraint, count, priority):
    connect()
    version=Version.get(reference=reference)
    constraint=Constraint.get(name=constraint)
    tasks = Task.select()
    print("tasks.count", tasks.count())
    Work.enque(tasks, version, constraint, priority, count)
    #we use list() to make faster?
    #for task in list(Task.select()):
        #Work.enque(task, version=version, constraint=constraint, priority=priority, count=count)
    close()

def add(args):
    if args[0] == "task":
        add_task(args[1:])
    elif args[0] == "task_dir":
        add_task_dir(args[1:])
    elif args[0] == "version":
        add_version(args[1:])
    elif args[0] == "constraint":
        help() if len(args) < 4 else add_constraint(args[1], args[2], args[3])
    elif args[0] == "work":
        add_work(args[1:])
    else:
        help()

def main():
    if len(argv) < 3:
        help()
        return
    add(argv[1:])

if __name__ == "__main__":
    main()

