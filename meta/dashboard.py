#!/usr/bin/env python3

from flask import Flask
from models import *
import report
import playhouse.shortcuts
import json

app = Flask(__name__)

@app.route("/")
def dashboard():
    return "<pre>" + report.table() + "</pre>"

@app.route("/best/")
def best():
    #runs = Run.select()
    runs = (Run.select(Run, Task, fn.Max(Run.score).alias("score"))
    #        .where(Run.score != None)
            .group_by(Run.task)
            .join(Task))
    result = []
    for r in runs:
        result.append(playhouse.shortcuts.model_to_dict(r, fields_from_query=runs))
    return json.dumps(result)

if __name__ == "__main__":
    app.run()

