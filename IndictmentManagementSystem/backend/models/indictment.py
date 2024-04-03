from models import db, BaseModel


class Indictment(BaseModel):
    __tablename__ = "t_indictment"
    __table_args__ = ({"comment": "起诉书信息"})
    indictment_name = db.Column(db.String(32), primary_key=True,unique=True, name="indictment_name", nullable=False, comment='起诉书名称')
    person_name = db.Column(db.String(64), unique=False,name="person_name", nullable=False, comment='操作人')
    status = db.Column(db.Integer, unique=False,name="status", nullable=False, comment='识别状态:1=已识别,0=未识别')
