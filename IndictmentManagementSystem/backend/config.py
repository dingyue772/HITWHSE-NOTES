import os
from urllib import parse


class BaseConfig(object):
    """
    基础配置
    """
    DEBUG = True  # 调试模式
    APP_AUTHOR = "dy"   # 作者
    WHITE_LIST = ['/user/get', '/user/list', '/ex/test', '/ex/other', "/db/test", "/config/test"]   # 权限白名单
    DB_HOST = "localhost"   # 数据库ip
    DB_PORT = "3306"    # 数据库端口
    DB_NAME = "flaskapp"    # 数据库名称
    DB_USER = "root"    # 数据库用户
    DB_PASSWORD = "1234" # 数据库密码
    # 防止密码中有特殊字符，需要使用parse.quote_plus进行转义
    SQLALCHEMY_DATABASE_URI = f'mysql+pymysql://{DB_USER}:{parse.quote_plus(DB_PASSWORD)}@{DB_HOST}:{DB_PORT}/{DB_NAME}?charset=utf8mb4'
    SQLALCHEMY_TRACK_MODIFICATIONS = True
    SQLALCHEMY_RECORD_QUERIES = True
    SQLALCHEMY_ENGINE_OPTIONS = {
        'pool_size': 5,
        'pool_timeout': 90,
        'pool_recycle': 7200,
        'max_overflow': 1024
    }
    # 数据库相关配置结束
    JSON_AS_ASCII = False  # 禁止中文转义


class Development(BaseConfig):
    PORT = 5000
    DEBUG = True
    ENV = "dev"
    TESTING = True
    SQLALCHEMY_ECHO = True  # 打印SQL

class Test(BaseConfig):
    PORT = 5000
    DEBUG = True
    ENV = "test"
    TESTING = True

class Production(BaseConfig):
    PORT = 5000
    DEBUG = False
    ENV = "prod"
    TESTING = False

def getConfig():
    # 从环境变量中加载ENV
    env = os.getenv("ENV", "dev")
    if env == "test":
        return Test
    elif env == "prod" or env == "production":
        return Production
    else:
        return Development