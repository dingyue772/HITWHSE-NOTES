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

// 进行霍夫变换
void houghTransform(const Image& edges, vector<pair<int, int>>& lines, int threshold) {
    const int maxTheta = 180;
    const double pi = 3.1;
    int maxDistance = static_cast<int>(sqrt(pow(edges.rows, 2) + pow(edges.cols, 2)));
    const int SIZE = 2 * maxDistance + 100;
    const int OFFSET = SIZE / 2;
    // 构建霍夫累加器
    vector<vector<int>> accumulator(SIZE, vector<int>(maxTheta, 0));
    // 遍历边缘图，进行霍夫变换
    for (int y = 0; y < edges.rows; ++y) {
        for (int x = 0; x < edges.cols; ++x) {
            if (edges.data[y][x] > 0) {
                for (int theta = 0; theta < maxTheta; ++theta) {
                    double radians = theta * pi / 180.0;
                    int distance = static_cast<int>(x * cos(radians) + y * sin(radians));
                    accumulator[distance+OFFSET][theta]++;
                }
            }
        }
    }
    // 寻找霍夫空间中的最大值
    for (int i = 0; i < SIZE; ++i) {
        for (int theta = 0; theta < maxTheta; ++theta) {
            if (accumulator[i][theta] > threshold) {
                int distance = i - OFFSET;
                lines.push_back({ distance, theta });
            }
        }
    }
}

// 在原图上绘制检测到的直线
void drawLinesOnImage(const Image& image, const vector<pair<int, int>>& lines, Image& result) {
    const double pi = 3.1;
    for (const auto& line : lines) {
        int distance = line.first;
        int theta = line.second;
        double radians = theta * pi / 180.0;
        for (int x = 0; x < image.cols; ++x) {
            int y = static_cast<int>((distance - x * cos(radians)) / sin(radians));
            if (y >= 0 && y < image.rows) {
                result.data[y][x] = 255;  // 将检测到的直线在原图上用白色表示
            }
        }
    }
}

// 释放图像内存
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}

