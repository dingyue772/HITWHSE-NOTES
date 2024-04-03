from flask import Blueprint

role = Blueprint('role', __name__, url_prefix="/sys/role")


@role.route("/detail", methods=['POST'])
def role_detail():
    return "role_detail"


@role.route("/page", methods=['POST'])
def role_page():
    return "role_page"


@role.route("/save", methods=['POST'])
def role_save():
    return "role_save"


@role.route("/update", methods=['POST'])
def role_update():
    return "role_update"


@role.route("/remove", methods=['POST'])
def role_remove():
    return "role_remove"