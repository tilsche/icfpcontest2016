import tempfile
import pathlib
import pickle
import random
import subprocess
import xmlrpc.client

from models import *

def get_work():
    """Get work from Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    return tuple(map(pickle.loads, proxy.get_work_pickled()))

def submit_work(task, version, constraint, seed, solution):
    """Submit work to Master, see work_local.py"""

    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/", use_builtin_types=True)
    print(task, version, constraint, seed, solution)
    proxy.submit_work_pickled(*tuple(map(pickle.dumps, [task, version, constraint, seed, solution])))

