from peewee import *
from playhouse.shortcuts import RetryOperationalError

class MyRetryDB(RetryOperationalError, SqliteDatabase):
    pass

DATABASE = '.icfpc/db.db'

database = MyRetryDB(DATABASE)

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
    name = TextField(unique=True)

class Work(BaseModel):
    task = ForeignKeyField(Task, related_name='work')
    version = ForeignKeyField(Version, related_name='work')
    constraint = ForeignKeyField(Constraint, related_name='work')
    priority = IntegerField()
    count = IntegerField()
    def deque(cores_max):
        try:
            with database.atomic():
                work = (Work.select().join(Constraint)
                .where(Constraint.cores <= cores_max)
                .order_by(Work.priority).limit(1).get())
                if work.count > 1:
                    work.count -= 1
                    work.save()
                else:
                    work.delete_instance()
                return work
        except Work.DoesNotExist:
            return None
    def enque(task, version, constraint, priority, count):
        return Work.create(task=task, version=version, constraint=constraint, priority=priority, count=count)

class Run(BaseModel):
    task = ForeignKeyField(Task, related_name='runs')
    version = ForeignKeyField(Version, related_name='runs')
    constraint = ForeignKeyField(Constraint, related_name='runs')
    path = TextField(unique=True)
    score = FloatField(null=True)

class Tag(BaseModel):
    name = TextField(unique=True)

def connect():
    database.connect()

def close():
    database.close()

def create_tables():
    database.create_tables([Task, Version, Constraint, Run, Work, Tag, ])

