from sqlalchemy import LargeBinary, PrimaryKeyConstraint

from models import db, BaseModel


class Picture(db.Model):
    __tablename__ = "t_picture"
    __table_args__ = ({"comment": "起诉书图片信息"})
    indictment_name = db.Column(db.String(64), nullable=True, comment="起诉书名称")
    picture_name = db.Column(db.String(64), nullable = True, comment="起诉书图片名称")
    file_data = db.Column(LargeBinary, primary_key=True, nullable=True, comment='图片信息')