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

// ��������ṹ��
struct Region {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
};

// ���������ƽ���Ҷ�ֵ
int calculateRegionMean(const Image& img, const Region& region) {
    int sum = 0;
    int count = 0;
    for (int i = region.topLeftY; i <= region.bottomRightY; ++i) {
        for (int j = region.topLeftX; j <= region.bottomRightX; ++j) {
            sum += img.data[i][j];
            count++;
        }
    }
    return sum / count;
}

// ����ָ������ڲ����ص�һ����
bool isSimilar(const Image& img, const Region& region, int thre) {
    int mean = calculateRegionMean(img, region);
    for (int i = region.topLeftY; i <= region.bottomRightY; ++i) {
        for (int j = region.topLeftX; j <= region.bottomRightX; ++j) {
            if (abs(img.data[i][j] - mean) > thre)
                return false;
        }
    }
    return true;
}

// ������Ѻ���
void regionSplit(Image& img, const Region& region, int threshold) {
    const int winsize = 15; // ������С�ָ�ڱ߳�Ϊ8
    int width = region.bottomRightX - region.topLeftX;
    int height = region.bottomRightY - region.topLeftY;
    if (isSimilar(img, region, threshold) || (width<winsize && height < winsize)) {
        return;
    }
    // ���򣬼�������
    int midX = (region.topLeftX + region.bottomRightX) / 2;
    int midY = (region.topLeftY + region.bottomRightY) / 2;
    // ��ʶ�ָ����
    for (int i = region.topLeftX; i <= region.bottomRightX; i++) {
        img.data[midY][i] = 0;
    }
    for (int i = region.topLeftY; i <= region.bottomRightY; i++) {
        img.data[i][midX] = 0;
    }
    // ���ѳ��ĸ�������
    Region subregions[4] = {
        {region.topLeftX, region.topLeftY, midX-1, midY-1},
        {midX + 1, region.topLeftY, region.bottomRightX, midY-1},
        {region.topLeftX, midY + 1, midX-1, region.bottomRightY},
        {midX + 1, midY + 1, region.bottomRightX, region.bottomRightY}
    };

    // �ݹ��ÿ����������з���
    for (int i = 0; i < 4; ++i) {
        regionSplit(img,subregions[i], threshold);
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
    ifstream input("6-2.bmp", ios::binary);
    ofstream output("split.bmp", ios::binary);
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
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    // �����ʼ����Ϊ����ͼ��
    Region initialRegion = { 0, 0, img.cols - 1, img.rows - 1 };
    // ���÷�����ֵ
    int threshold = 60;
    // ����������Ѻ�������ͼ��ָ�
    regionSplit(img, initialRegion, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    // �ͷ�ͼ���ڴ�
    releaseImage(img);
    return 0;
}
