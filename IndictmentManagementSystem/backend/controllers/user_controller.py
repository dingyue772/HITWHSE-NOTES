from flask import Blueprint,request
from controllers import R
from models import db
from models.user import User
from validators import BasePageForm
from validators.id_validator import IdsForm
from validators.user_validator import UserForm

user = Blueprint('user', __name__, url_prefix="/user")

@user.route("/login", methods=['POST'])
def login():
    """
    通过用户输入的用户名和密码查询数据库，返回结果
    :return:
    """
    # form = UserForm()
    # form.validate_for_api()
    data = request.json
    print(data)
    u = User.query.filter_by(user_name=data.get('userName')).first()
    print(u)
    if u is not None and u.password == data.get('password'):
        return R.success("登录成功")
    else:
        return R.fail("用户名或密码不正确")

@user.route("/list", methods=['POST'])
def user_list():
    """
    分页查询用户列表
    :return:
    """
    form = BasePageForm()
    form.validate_for_api()
    # print(form.data)
    user_obj = User.query.filter().paginate(form.pageNum.data, form.pageSize.data, False)
    rows = []
    for u in user_obj.items:
        rows.append({
            "id": u.id,
            "userName": u.user_name
        })
    return R.data({
        "recordCount": user_obj.total, # 记录总数
        "totalPage": int((user_obj.total - 1) / user_obj.per_page) + 1, # 分页的总页数
        "pageSize": user_obj.per_page, # 每页记录数
        "pageNum": user_obj.page, # 当前页数
        "rows": rows # 返回记录数据
    })


@user.route("/register", methods=['POST'])
def register():
    """
    用户提交注册表单完成用户注册
    :return:
    """
    data = request.json
    # form = UserForm()
    # form.validate_for_api()
    userName=data.get("userName")
    existing_user = User.query.filter_by(user_name=userName).first()
    if existing_user:
        return R.fail("用户名已存在")

    u = User()
    u.user_name = data.get("userName")

    u.password = data.get("password")
    db.session.add(u)
    db.session.commit()
    return R.success("用户注册成功")


@user.route("/update", methods=['POST'])
def user_update():
    """
    修改用户
    :return:
    """
    form = UserForm()
    form.validate_for_api()
    # print(form.data)
    User.query.filter_by(id=form.id.data).update({
        User.user_name: form.userName.data,
        User.password: form.password.data
    })
    db.session.commit()
    return R.success("修改用户成功")


@user.route("/delete", methods=['POST'])
def user_delete():
    """
    删除用户
    :return:
    """
    form = IdsForm()
    form.validate_for_api()
    # print(form.data)
    User.query.filter(User.id.in_(form.ids.data)).delete()
    db.session.commit()
    return R.success("删除用户成功")


