from models import db, BaseModel


class User(BaseModel):
    __tablename__ = "t_user"
    __table_args__ = ({"comment": "用户"})
    # id = db.Column(db.BigInteger, name="id", primary_key=True, comment="主键")
    user_name = db.Column(db.String(32), name="user_name", primary_key=True, unique=True, nullable=False, comment="用户名")
    password = db.Column(db.String(64), name="password", unique=False, nullable=False, comment="密码")