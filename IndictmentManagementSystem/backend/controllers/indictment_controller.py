import base64
import os
import tempfile
import datetime as datetime1
from datetime import datetime
import re
import easyocr

from PIL import Image
from io import BytesIO
from flask import Blueprint, request, send_file
from controllers import R
from models import db
from models.indictment import Indictment
from models.picture import Picture
from models.criminal import Criminal



indictment = Blueprint('indictment', __name__, url_prefix="/indictment")
@indictment.route("/test", methods=['GET','POST'])
def get_pictures():
    file = request.files['files']
    print(type(file))
    return R.success("上传图片成功")
# 新增起诉书的接口
@indictment.route("/add", methods=['POST'])
def add_indictment():
    indictment_name = request.form.get('indictment_name')
    person_name = request.form.get('person_name')
    files = request.files.getlist('files')
    status = request.form.get('status')
    # print(indictment_name, person_name, status)
    # 进行非空验证
    if not indictment_name or not person_name or not files or not status:
        return R.fail("新增起诉书字段缺失")
    # 验证当前起诉书是否存在
    exsiting_indictment = Indictment.query.filter_by(indictment_name=indictment_name).first()
    if exsiting_indictment:
        return R.fail("当前起诉书已存在！")
    # 构造数据模型进行存储
    # 存储起诉书信息
    new_indictment = Indictment(
        indictment_name = indictment_name,
        person_name = person_name,
        status = status
    )
    db.session.add(new_indictment)
    # 存储起诉书图片信息
    for file in files:
        new_picture = Picture(
            indictment_name=indictment_name,
            file_data=file.read(),
            picture_name=file.filename
        )
        db.session.add(new_picture)
    # 数据库commit
    db.session.commit()
    return R.success("新增起诉书成功！")

# 删除起诉书的接口
@indictment.route("/delete", methods=['POST'])
def delete_indictment():
    indictment_name = request.get_json().get('indictment_name')
    # 删除t_indictment表中对应数据
    Indictment.query.filter_by(indictment_name=indictment_name).delete()
    # 删除t_picture表中对应数据
    Picture.query.filter(Picture.indictment_name == indictment_name).delete()
    db.session.commit()
    return R.success("删除起诉书成功")

# 修改起诉书的接口
@indictment.route("/update", methods=['POST'])
def update_indictment():
    # 如果请求为GET请求，则展示原有数据
    # if request.method == 'GET':
    #     pass
    # # 如果为POST请求，则更新相应数据
    # else:
    indictment_name = request.form.get('indictment_name')
    person_name = request.form.get('person_name')
    files = request.files.getlist('files')
    status = request.form.get('status')
    if not indictment_name or not person_name or not files or not status:
        return R.fail("error:Missing field!")

    Indictment.query.filter_by(indictment_name=indictment_name).update({
        Indictment.indictment_name: indictment_name,
        Indictment.person_name: person_name,
        Indictment.status: status
    })
    Picture.query.filter(Picture.indictment_name == indictment_name).delete()
    for file in files:
        new_picture = Picture(
            indictment_name=indictment_name,
            file_data=file.read(),
            picture_name=file.filename
        )
        db.session.add(new_picture)
    db.session.commit()
    return R.success("修改起诉书成功！")

# 查询全部起诉书数据
@indictment.route("/all", methods=['GET'])
def get_all_indictments():
    page = request.args.get('page', 1, type=int)  # 获取页码，默认为第1页
    per_page = request.args.get('per_page', 10, type=int)  # 获取每页记录数，默认为10条

    # 查询t_indictment数据库中的所有记录，并按照创建时间倒序排列,每条记录返回一个data，对应的是多张起诉书图片
    inds = Indictment.query.order_by(Indictment.create_time.desc()).paginate(page=page, per_page=per_page)
    # 构造返回的数据结构
    data = {
        'recordCount': inds.total,  # 总记录数
        'totalPage': inds.pages,  # 总页数
        'current_page': inds.page,  # 当前页码
        'per_page': inds.per_page,  # 每页记录数
        'rows': []  # 记录列表
    }
    for ind in inds.items:
        # 构造每条记录的数据结构
        per_row = {
            'indictment_name': ind.indictment_name,
            'person_name': ind.person_name,
            'status': ind.status,
            'create_time': ind.create_time.strftime('%Y-%m-%d %H:%M:%S'),
            'update_time': ind.update_time.strftime('%Y-%m-%d %H:%M:%S'),
            'files': []
        }
        # 根据此起诉书的名称在t_pictur表中查询对应的起诉书图片
        pictures = Picture.query.filter(Picture.indictment_name==ind.indictment_name).paginate(page=page, per_page=per_page)
        for file in pictures.items:
            # 将数据库中存储的起诉书图片的二进制数据进行base64编码传回前端
            encoded_file = base64.b64encode(file.file_data).decode('utf-8')
            per_row['files'].append(encoded_file)
        data['rows'].append(per_row)
    return R.data(data)

# 根据前端给出的条件进行查询
@indictment.route('/query', methods=['POST'])
def query_data():
    data = request.get_json()
    print(data)
    indictment_name = data.get('indictment_name')
    person_name = data.get('person_name')
    # None处理了start_time和end_time在前端可能为undefined的情况
    start_time = data.get('start_time', None)
    end_time = data.get('end_time', None)
    # page = data.get('page')  # 获取页码，默认为第1页
    # per_page = data.get('per_page')  # 获取每页记录数，默认为10条

    print(indictment_name,person_name)
    query = Indictment.query
    if indictment_name:
        query = query.filter_by(indictment_name=indictment_name)

    if person_name:
        query = query.filter_by(person_name=person_name)

    if start_time and end_time:
        if isinstance(start_time, str):
            start_time = datetime.strptime(start_time, '%Y-%m-%dT%H:%M:%S.%fZ')
        if isinstance(end_time, str):
            end_time = datetime.strptime(end_time, '%Y-%m-%dT%H:%M:%S.%fZ')
        start_time += datetime1.timedelta(hours=8)
        end_time += datetime1.timedelta(hours=8)
        query = query.filter(Indictment.create_time.between(start_time, end_time))

    inds = query.all()
    print(inds)
    # 构造返回的数据结构
    data = {
        'recordCount': len(inds),  # 总记录数
        # 'current_page': inds.page,  # 当前页码
        # 'per_page': inds.per_page,  # 每页记录数
        'rows': []  # 记录列表
    }
    for ind in inds:
        # 构造每条记录的数据结构
        print(ind)
        per_row = {
            'indictment_name': ind.indictment_name,
            'person_name': ind.person_name,
            'status': ind.status,
            'create_time': ind.create_time.strftime('%Y-%m-%d %H:%M:%S'),
            'update_time': ind.update_time.strftime('%Y-%m-%d %H:%M:%S'),
            'picList': [],
            'picNameList':[]
        }
        print(per_row)
        # 根据此起诉书的名称在t_pictur表中查询对应的起诉书图片
        pictures = Picture.query.filter(Picture.indictment_name == ind.indictment_name).all()
        for file in pictures:
            # 将数据库中存储的起诉书图片的二进制数据进行base64编码传回前端
            encoded_file = base64.b64encode(file.file_data).decode()
            per_row['picList'].append(encoded_file)
            per_row['picNameList'].append(file.picture_name)
        data['rows'].append(per_row)
    return R.data(data)


# 按照起诉书名称进行查询
@indictment.route("/get_by_name", methods=['GET'])
def get_indictments_by_name():
    page = request.args.get('page', 1, type=int)  # 获取页码，默认为第1页
    per_page = request.args.get('per_page', 10, type=int)  # 获取每页记录数，默认为10条
    indictment_name = request.args.get('indictment_name')

    # 查询数据库中的所有记录，并按照创建时间倒序排列
    inds = Indictment.query.filter(Indictment.indictment_name==indictment_name).order_by(Indictment.create_time.desc()).paginate(page=page, per_page=per_page)
    # 构造返回的数据结构
    data = {
        'recordCount': inds.total,  # 总记录数
        'totalPage': inds.pages,  # 总页数
        'current_page': inds.page,  # 当前页码
        'per_page': inds.per_page,  # 每页记录数
        'rows': []  # 记录列表
    }
    for ind in inds.items:
        # 构造每条记录的数据结构
        per_row = {
            'indictment_name': ind.indictment_name,
            'person_name': ind.person_name,
            'status': ind.status,
            'create_time': ind.create_time.strftime('%Y-%m-%d %H:%M:%S'),
            'update_time': ind.update_time.strftime('%Y-%m-%d %H:%M:%S'),
            'picList': []
        }
        # 根据此起诉书的名称在t_pictur表中查询对应的起诉书图片
        pictures = Picture.query.filter(Picture.indictment_name == ind.indictment_name).order_by(
            Picture.create_time.desc()).paginate(page=page, per_page=per_page)
        for file in pictures.items:
            # 对起诉书图片进行编码
            encoded_file = base64.b64encode(file).decode('utf-8')
            per_row['picList'].append(encoded_file)
        data['picList'].append(per_row)
    return R.data(data)

# 下载起诉书图片
@indictment.route("/getPic", methods=['GET'])
def get_picture():
    picName = request.args.get('picName')
    indictment_name = request.args.get('indictment_name')
    picture = Picture.query.filter(Picture.picture_name==picName).filter(Picture.indictment_name==indictment_name).first()
    if picture:
        # 创建临时文件再传输
        with tempfile.NamedTemporaryFile(delete=False) as tmp_file:
            tmp_file.write(picture.file_data)
        # 将临时文件发送到前端供下载
        return send_file(
            tmp_file.name,
            mimetype='image/jpg',
            attachment_filename=picName,
            as_attachment=True
        )
    else:
        return "Picture not found"


# 识别起诉书接口
@indictment.route("/recognize_text", methods=['POST'])
def recognize_text():
    # 获取当前文件的绝对路径
    current_file_path = os.path.abspath(__file__)

    # 获取当前文件所在的文件夹路径
    current_folder_path = os.path.dirname(current_file_path)
    reader = easyocr.Reader(['ch_sim','en'], model_storage_directory=current_folder_path,
                            user_network_directory=current_folder_path,
                            download_enabled=False, gpu=False)
    print(reader)

    data = request.get_json()
    indictment_name = data.get("indictment_name")
    # print(indictment_name)
    indictment = Indictment.query.filter_by(indictment_name=indictment_name).first()
    if indictment:
        indictment.status = 1
        db.session.commit()
    pictures=Picture.query.filter(Picture.indictment_name==indictment_name).all()
    data={
        "picList":[],
        "textList":[]
    } # 定义的返回前端的数据
    temp=[] # 用于处理easyocr识别结果的临时数据结构
    for picture in pictures:
        text = '' # 构造一张图片的识别数据
        picture_data=picture.file_data
        image=Image.open(BytesIO(picture_data))
        temp.append(reader.readtext(image))
        for result in temp:
            for sen in result:
                text += sen[1] + '\n'
        # text = pytesseract.image_to_string(image, lang='chi_sim')
        data['textList'].append(text)
        encoded_file = base64.b64encode(picture_data).decode('utf-8')
        data['picList'].append(encoded_file)
        text = text.replace('\n', '')
        # print(text)
        # 提取犯罪嫌疑人的信息
        # pattern = r"犯罪嫌疑人(.*?)"
        # matches = re.findall(pattern, text, re.DOTALL)
        # print(matches)
        # 逐个提取每个犯罪嫌疑人的相关信息
        # for match in matches:
        name = re.search(r"犯罪嫌疑人(.*?) \(曾用名", text).group(1)
        ethnicity = re.search(r"民族:  (.*?),", text).group(1)
        id_number = re.search(r"居民身份证号码:(.*?),", text).group(1)
        education = re.search(r"文化程度:  (.*?),", text).group(1)
        political_status = re.search(r"政治面貌:  (.*?)。", text).group(1)
        domicile = re.search(r"户籍地:  (.*?),", text).group(1)
        residence = re.search(r"现居地:  (.*?),", text).group(1)
        workplace = re.search(r"工作单位:  (.*?)。", text).group(1)
        crime = re.search(r"因涉嫌(.*?)罪", text).group(1)
        new_criminal = Criminal(
            indictment_name = indictment_name,
            criminal_name=name,
            race = ethnicity,
            id_card = id_number,
            educational_level = education,
            political_status = political_status,
            home_place = domicile,
            now_place = residence,
            job_company = workplace,
            crime_facts = crime
        )
        print(new_criminal)
        # 判断当前识别得到的嫌疑人是否已经存在于数据库中
        # cri = Criminal.query.filter_by()
        db.session.add(new_criminal)
        db.session.commit()
    return R.data(data)


