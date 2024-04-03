from models import db, BaseModel


class Criminal(BaseModel):
    __tablename__ = "t_criminal"
    __table_args__ = ({"comment": "犯罪嫌疑人信息"})
    indictment_name = db.Column(db.String(64), primary_key=True,unique=False, name="indictment_name", nullable=False, comment='起诉书名称')
    criminal_name = db.Column(db.String(64), primary_key=True, unique=False,name="criminal_name", nullable=False, comment='嫌疑人姓名')
    # used_name = db.Column(db.String(64), unique=False,name="person_name", nullable=True, comment='曾用名')
    race = db.Column(db.String(64), unique=False,name="race", nullable=True, comment='民族')
    educational_level = db.Column(db.String(64), unique=False,name="educational_level", nullable=True, comment='文化程度')
    political_status = db.Column(db.String(64), unique=False,name="political_status", nullable=True, comment='政治面貌')
    crime_facts = db.Column(db.String(750), unique=False,name="crime_facts", nullable=True, comment='犯罪事实')
    id_card = db.Column(db.String(64), unique=False,name="id_card", nullable=True, comment='身份证号码')
    home_place = db.Column(db.String(200), unique=False,name="home_place", nullable=True, comment='户籍地')
    now_place = db.Column(db.String(200), unique=False,name="now_place", nullable=True, comment='现居地')
    job_company = db.Column(db.String(200), unique=False,name="job_company", nullable=True, comment='工作单位')