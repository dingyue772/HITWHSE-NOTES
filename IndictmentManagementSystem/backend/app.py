from flask import Flask, request
from config import getConfig
from controllers import R
from controllers.role_controller import role
from controllers.user_controller import user
from controllers.indictment_controller import indictment
from controllers.criminal_controller import criminal
from models import db

app = Flask(__name__)

# 注册用户模块
app.register_blueprint(user)
# 注册角色模块
app.register_blueprint(role)
app.register_blueprint(indictment)
app.register_blueprint(criminal)

# 从配置对象中加载
app.config.from_object(getConfig())

# 初始化db
db.init_app(app)


# @app.before_request
def auth():
    # 简单处理一下，非白名单路由，提示登录
    if request.path not in app.config['WHITE_LIST']:
        return "请先登录"


@app.route("/ex/test")
def ex_test():
    """
    测试其他异常
    :return:
    """
    a = 3 / 0
    return a


@app.route("/db/test")
def db_test():
    cursor = db.session.execute('select * from t_user')
    result = cursor.fetchall()
    if len(result) > 0:
        u = result[0]
        return f"id:{u.id},user_name:{u.user_name}"
    return R.fail("无记录")


@app.route("/config/test")
def config_test():
    """
    配置测试
    :return:
    """
    return R.data({
        'APP_AUTHOR': app.config['APP_AUTHOR'],
        'ENV':app.config['ENV']
    })


@app.errorhandler(404)
def error(e):
    """
    404异常处理
    :param e:
    :return:
    """
    return R.fail("请求地址不存在")


@app.errorhandler(Exception)
def error(e):
    """
    其他异常处理
    :param e: 异常
    :return:
    """
    print(e)
    return R.fail(str(e))


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000)