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

// ������
int computeDifference(const Image& input, int i, int j, const int mask[3][3]) {
    int result = 0;
    for (int m = -1; m <= 1; ++m) {
        for (int n = -1; n <= 1; ++n) {
            result += input.data[i + m][j + n] * mask[m + 1][n + 1];
        }
    }
    return result;
}

// �����ݶȷ�ֵ
void computeGradient(const Image& input, Image& output, const int maskX[3][3], const int maskY[3][3], int thre) {
    for (int i = 1; i < input.rows - 1; ++i) {
        for (int j = 1; j < input.cols - 1; ++j) {
            int gradientX = abs(computeDifference(input, i, j, maskX));
            int gradientY = abs(computeDifference(input, i, j, maskY));
            // �����ݶȷ�ֵ
            int gradient = gradientX + gradientY;
            if (gradient >= thre)
                output.data[i][j] = 255;
            else 
                output.data[i][j] = 0;
        }
    }
}

// ������
int computeDifference(const Image& input, int i, int j, const int mask[5][5]) {
    int result = 0;
    for (int m = -2; m <= 2; ++m) {
        for (int n = -2; n <= 2; ++n) {
            result += input.data[i + m][j + n] * mask[m + 2][n + 2];
        }
    }
    return result;
}

// �����ݶȷ�ֵ�����غ���
void computeGradient(const Image& input, Image& output, const int mask[5][5], int thre){
    for (int i = 2; i < input.rows - 2; ++i) {
        for (int j = 2; j < input.cols - 2; ++j) {
            int gradient = -computeDifference(input, i, j, mask);
            if (gradient >= thre)
                output.data[i][j] = 255;
            else
                output.data[i][j] = 0;
        }
    }
}




// �ͷ�ͼ���ڴ�
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}

int main() {
    ifstream input("lena.bmp", ios::binary);
    ofstream output("sobel.bmp", ios::binary);
    // ��дͼ���ļ�ͷ����Ϣͷ
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));
    // ��дͼ���ɫ��
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
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
    // �������ͼ��
    Image result;
    result.rows = img.rows;
    result.cols = img.cols;
    result.data = new unsigned char* [width];
    for (int i = 0; i < height; ++i) {
        result.data[i] = new unsigned char[width];
    }
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);

    // ������ֵ
    int threshold = 230;
    // ����Sobel����
    const int sobelMaskX[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    const int sobelMaskY[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    // ����Prewitt����
    const int prewittMaskX[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    const int prewittMaskY[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    const int log[5][5] = { {0,0,1,0,0}, {0, -1, -2, -1, 0}, {-1, -2, 16, -2, -1}, {0, -1, -2, -1, 0}, {0,0,1,0,0} };
    //computeGradient(img, result, prewittMaskX, prewittMaskY, threshold);
    computeGradient(img, result, sobelMaskX, sobelMaskY, threshold);
    //computeGradient(img, result, log, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // �ͷ�ͼ���ڴ�
    releaseImage(img);
    releaseImage(result);

	return 0;
}
