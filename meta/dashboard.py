#!/usr/bin/env python3

from flask import Flask
import report

app = Flask(__name__)

@app.route("/")
def dashboard():
    return "<pre>" + report.table() + "</pre>"

if __name__ == "__main__":
    app.run()

