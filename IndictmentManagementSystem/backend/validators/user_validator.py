from wtforms import StringField, validators, PasswordField

from validators import BaseForm


class UserForm(BaseForm):
    """
    用户表单校验类
    """
    userName = StringField("用户名", [validators.DataRequired(message="用户名不能为空")])
    password = PasswordField("密码", [validators.DataRequired(message="密码不能为空")])