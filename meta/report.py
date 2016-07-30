#!/usr/bin/env python3

from sys import argv
import sqlite3
from tabulate import tabulate

def help():
    print("Reports information about tasks, versions, runs, scores")

def get_statistics():
    conn = sqlite3.connect("./.icfpc/db.db")
    cursor = conn.cursor()
    result = cursor.execute("SELECT "
        "version, versions.path, task, tasks.path, MIN(score), AVG(score), MAX(score) "
        "FROM runs, versions, tasks "
        "WHERE runs.version = versions.id AND runs.task = tasks.id "
        "GROUP BY version, task "
        "ORDER BY version, task").fetchall()
    conn.commit()
    conn.close()
    return result
    
def report():
    statistics = get_statistics()

    seen = set();
    headers = ["Vers\\Task"] + [s[3] for s in statistics if s[2] not in seen and not seen.add(s[2])]

    seenVersions = set()
    rows = []

    for s in statistics:
        if s[0] not in seenVersions:
            seenVersions.add(s[0])
            rows.append([s[1]] + [""] * (len(headers) - 1))
        #this line should be more robust than the next 
        #in case no run exists for some version and task
        rows[-1][headers.index(s[3]):] = [str(s[4]) + "/" + str(s[5]) + "/" + str(s[6])]
        #rows[-1].append(str(s[4]) + "/" + str(s[5]) + "/" + str(s[6]))

    print("In each row, you can see one of your versions, in each column you see how it did on some task over multiple runs")
    print("Format is")
    print("\tMinimum Score / Average Score / Maximum Score")
    print("over all runs of this Version\Task combination.")
    print(tabulate(rows, headers, tablefmt="fancy_grid")) 

def main():
    if len(argv) > 1:
        help()
        return

    report()

if __name__ == "__main__":
    main()

