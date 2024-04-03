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
// 释放图像内存
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}
// 辅助函数，检查坐标是否越界
bool isValidCoordinate(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows&& y >= 0 && y < cols;
}

// 提取轮廓的函数
void extractContours(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 1; j < img.cols; ++j) {
            if (img.data[i][j] > 200)
                img.data[i][j] = 255;
            else
                img.data[i][j] = 0;
        }
    }
    // 定义8邻域的坐标增量
    int neighbors[8][2] = { {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
    // 遍历图像像素
    for (int i = 1; i < img.rows-1; ++i) {
        for (int j = 1; j < img.cols-1; ++j) {
            if (img.data[i][j] == 0) { // 如果当前像素是对象（或区域）内的点
                int count = 0;
                // 检查8邻域
                for (int k = 0; k < 8; ++k) {
                    int neighborX = i + neighbors[k][0];
                    int neighborY = j + neighbors[k][1];
                    // 检查邻居是否在图像内且不属于同一对象
                  
                    if (isValidCoordinate(neighborX, neighborY, img.rows, img.cols) &&
                        img.data[neighborX][neighborY] == 255) {
                        count++;
                    }
                }
                // 什么情况能够保留该像素点的黑色？至少一个邻居为255？不可行似乎
                if (count < 5) {
                    img.data[i][j] = 255; // 设置轮廓图像中的边界点
                }
            }
        }
    }
}
int main() {
    ifstream input("10-2.bmp", ios::binary);
    ofstream output("extract2.bmp", ios::binary);
    // 读写图像文件头和信息头
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));
    cout << infoHeader.biBitCount << endl;
    // 读写图像调色板
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    int width = (((infoHeader.biWidth * infoHeader.biBitCount) + 31) & ~31) / 8;
    int height = infoHeader.biHeight;
    cout << width << endl;
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
    // 输出数据
    // 图像数据存储
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    extractContours(img);
    cout << "图像提取边缘成功！" << endl;
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    releaseImage(img);
    return 0;
}