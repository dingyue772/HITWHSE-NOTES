#include <iostream>
#include <fstream>
#include <vector>
#include "bmp.h"

using namespace std;

// ����ͼ��ṹ��
struct Image {
    int rows;
    int cols;
    unsigned char** data;
};
// �ͷ�ͼ���ڴ�
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}
// ������������������Ƿ�Խ��
bool isValidCoordinate(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows&& y >= 0 && y < cols;
}

// ��ȡ�����ĺ���
void extractContours(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 1; j < img.cols; ++j) {
            if (img.data[i][j] > 200)
                img.data[i][j] = 255;
            else
                img.data[i][j] = 0;
        }
    }
    // ����8�������������
    int neighbors[8][2] = { {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
    // ����ͼ������
    for (int i = 1; i < img.rows-1; ++i) {
        for (int j = 1; j < img.cols-1; ++j) {
            if (img.data[i][j] == 0) { // �����ǰ�����Ƕ��󣨻������ڵĵ�
                int count = 0;
                // ���8����
                for (int k = 0; k < 8; ++k) {
                    int neighborX = i + neighbors[k][0];
                    int neighborY = j + neighbors[k][1];
                    // ����ھ��Ƿ���ͼ�����Ҳ�����ͬһ����
                  
                    if (isValidCoordinate(neighborX, neighborY, img.rows, img.cols) &&
                        img.data[neighborX][neighborY] == 255) {
                        count++;
                    }
                }
                // ʲô����ܹ����������ص�ĺ�ɫ������һ���ھ�Ϊ255���������ƺ�
                if (count < 5) {
                    img.data[i][j] = 255; // ��������ͼ���еı߽��
                }
            }
        }
    }
}
int main() {
    ifstream input("10-2.bmp", ios::binary);
    ofstream output("extract2.bmp", ios::binary);
    // ��дͼ���ļ�ͷ����Ϣͷ
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));
    cout << infoHeader.biBitCount << endl;
    // ��дͼ���ɫ��
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    int width = (((infoHeader.biWidth * infoHeader.biBitCount) + 31) & ~31) / 8;
    int height = infoHeader.biHeight;
    cout << width << endl;
    // ����ԭʼͼ��
    Image img;
    img.cols = width;
    img.rows = height;
    // ͼ�����ݴ洢
    img.data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        img.data[i] = new unsigned char[width];
    }
    // ��������
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    // �������
    // ͼ�����ݴ洢
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    extractContours(img);
    cout << "ͼ����ȡ��Ե�ɹ���" << endl;
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    releaseImage(img);
    return 0;
}