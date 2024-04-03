from PIL import Image
import easyocr
import os


# 获取当前文件的绝对路径
current_file_path = os.path.abspath(__file__)

    # 获取当前文件所在的文件夹路径
current_folder_path = os.path.dirname(current_file_path)
reader = easyocr.Reader(['ch_sim','en'], model_storage_directory=current_folder_path,
                            user_network_directory=current_folder_path,
                            download_enabled=False)
image=Image.open('1.jpg')
result = reader.readtext(image)
text = ''
for sen in result:
    text += sen[1] + '\n'

print(text)