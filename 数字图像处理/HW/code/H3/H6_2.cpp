#include <iostream>
#include <fstream>
#include <vector>
#include "bmp.h"

using namespace std;

// 定义图像结构体
struct Image {
    int rows;
    int cols;
    unsigned char** data;
};

// 定义区域结构体
struct Region {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
};

// 计算区域的平均灰度值
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

// 计算分割区域内部像素的一致性
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

// 区域分裂函数
void regionSplit(Image& img, const Region& region, int threshold) {
    const int winsize = 15; // 定义最小分割窗口边长为8
    int width = region.bottomRightX - region.topLeftX;
    int height = region.bottomRightY - region.topLeftY;
    if (isSimilar(img, region, threshold) || (width<winsize && height < winsize)) {
        return;
    }
    // 否则，继续分裂
    int midX = (region.topLeftX + region.bottomRightX) / 2;
    int midY = (region.topLeftY + region.bottomRightY) / 2;
    // 标识分割界限
    for (int i = region.topLeftX; i <= region.bottomRightX; i++) {
        img.data[midY][i] = 0;
    }
    for (int i = region.topLeftY; i <= region.bottomRightY; i++) {
        img.data[i][midX] = 0;
    }
    // 分裂成四个子区域
    Region subregions[4] = {
        {region.topLeftX, region.topLeftY, midX-1, midY-1},
        {midX + 1, region.topLeftY, region.bottomRightX, midY-1},
        {region.topLeftX, midY + 1, midX-1, region.bottomRightY},
        {midX + 1, midY + 1, region.bottomRightX, region.bottomRightY}
    };

    // 递归对每个子区域进行分裂
    for (int i = 0; i < 4; ++i) {
        regionSplit(img,subregions[i], threshold);
    }
}

// 释放图像内存
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}

int main() {
    ifstream input("6-2.bmp", ios::binary);
    ofstream output("split.bmp", ios::binary);
    // 读写图像文件头和信息头
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));
    // 读写图像调色板
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    // 创建原始图像
    Image img;
    img.cols = width;
    img.rows = height;
    // 图像数据存储
    img.data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        img.data[i] = new unsigned char[width];
    }
    // 读入数据
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    // 定义初始区域为整个图像
    Region initialRegion = { 0, 0, img.cols - 1, img.rows - 1 };
    // 设置方差阈值
    int threshold = 60;
    // 调用区域分裂函数进行图像分割
    regionSplit(img, initialRegion, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    // 释放图像内存
    releaseImage(img);
    return 0;
}
