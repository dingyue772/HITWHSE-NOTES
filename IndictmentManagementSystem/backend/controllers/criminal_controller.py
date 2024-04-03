from flask import Blueprint, request
from controllers import R
from models import db
from models.criminal import Criminal
import random
from faker import Faker

criminal = Blueprint('criminal',__name__, url_prefix='/criminal')

# 根据前端给出的条件进行查询
@criminal.route('/query', methods=['POST'])
def query_data():
    data = request.get_json()
    print(data)
    indictment_name = data.get('indictment_name')
    criminal_name = data.get('criminal_name')
    crime_facts = data.get('crime_facts')
    # page = data.get('page')  # 获取页码，默认为第1页
    # per_page = data.get('per_page')  # 获取每页记录数，默认为10条

    query = Criminal.query
    if indictment_name:
        query = query.filter_by(indictment_name=indictment_name)
    if criminal_name:
        query = query.filter_by(criminal_name=criminal_name)
    if crime_facts:
        query = query.filter(Criminal.crime_facts.ilike(f'{crime_facts}'))
    cris = query.all()
    print(cris)
    # 构造返回的数据结构
    data = {
        'recordCount': len(cris),  # 总记录数
        # 'current_page': cris.page,  # 当前页码
        # 'per_page': cris.per_page,  # 每页记录数
        'rows': []  # 记录列表
    }
    for ind in cris:
        # 构造每条记录的数据结构
        print(ind)
        per_row = {
            'indictment_name': ind.indictment_name,
            'criminal_name': ind.criminal_name,
            'race': ind.race,
            'educational_level':ind.educational_level,
            'political_status':ind.political_status,
            'crime_facts':ind.crime_facts,
            'id_card':ind.id_card,
            'home_place':ind.home_place,
            'now_place':ind.now_place,
            'job_company':ind.job_company
        }
        print(per_row)
        data['rows'].append(per_row)
        # 为了测试前端的分页功能，通过faker制造假数据返回到前端
        # if len(data['rows']) < 20:
        #     fake = Faker()
        #     for _ in range(20):
        #         criminal = {
        #             'indictment_name': fake.word(),
        #             'criminal_name': fake.name(),
        #             'race': fake.word(),
        #             'educational_level': fake.word(),
        #             'political_status': fake.word(),
        #             'crime_facts': fake.word(),
        #             'id_card': fake.unique.random_number(digits=18),
        #             'home_place': fake.address(),
        #             'now_place': fake.address(),
        #             'job_company': fake.company(),
        #         }
        #         data['rows'].append(criminal)
        #     data['recordCount'] += 20
    return R.data(data)