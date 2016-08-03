from peewee import *

DATABASE = '.icfpc/db.db'

database = SqliteDatabase(DATABASE)

class BaseModel(Model):
    class Meta:
        database = database

class Task(BaseModel):
    path = TextField(unique=True)

class Version(BaseModel):
    reference = TextField(unique=True)

class Constraint(BaseModel):
    runtime_ms = IntegerField()
    cores = IntegerField()

class Run(BaseModel):
    task = ForeignKeyField(Task, related_name='runs')
    version = ForeignKeyField(Version, related_name='runs')
    constraint = ForeignKeyField(Constraint, related_name='runs')
    path = TextField(unique=True)
    score = FloatField()

def connect():
    database.connect()

def close():
    database.close()

def create_tables():
    database.create_tables([Task, Version, Constraint, Run, ])

