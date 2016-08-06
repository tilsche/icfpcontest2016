#!/usr/bin/env python3

from flask import Flask
from models import *
import report
import playhouse.shortcuts

app = Flask(__name__)

@app.route("/")
def dashboard():
    return "<pre>" + report.table() + "</pre>"

@app.route("/best/")
def best():
    r = Run.select(Run, Task.path, fn.MAX(Run.score).alias("max")).join(Task).group_by(Run.task).get()
    return str(playhouse.shortcuts.model_to_dict(r))

if __name__ == "__main__":
    app.run()

