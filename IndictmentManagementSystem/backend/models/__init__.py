from datetime import datetime
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class BaseModel(db.Model):
    """
    普通模型基类
    """
    __abstract__ = True
    create_time = db.Column(db.DateTime, name="create_time", default=datetime.now, comment="创建时间")
    update_time = db.Column(db.DateTime, name="update_time", default=datetime.now, onupdate=datetime.now,comment="更新时间")