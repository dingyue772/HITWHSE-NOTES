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

// ���л���任
void houghTransform(const Image& edges, vector<pair<int, int>>& lines, int threshold) {
    const int maxTheta = 180;
    const double pi = 3.1;
    int maxDistance = static_cast<int>(sqrt(pow(edges.rows, 2) + pow(edges.cols, 2)));
    const int SIZE = 2 * maxDistance + 100;
    const int OFFSET = SIZE / 2;
    // ���������ۼ���
    vector<vector<int>> accumulator(SIZE, vector<int>(maxTheta, 0));
    // ������Եͼ�����л���任
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
    // Ѱ�һ���ռ��е����ֵ
    for (int i = 0; i < SIZE; ++i) {
        for (int theta = 0; theta < maxTheta; ++theta) {
            if (accumulator[i][theta] > threshold) {
                int distance = i - OFFSET;
                lines.push_back({ distance, theta });
            }
        }
    }
}

// ��ԭͼ�ϻ��Ƽ�⵽��ֱ��
void drawLinesOnImage(const Image& image, const vector<pair<int, int>>& lines, Image& result) {
    const double pi = 3.1;
    for (const auto& line : lines) {
        int distance = line.first;
        int theta = line.second;
        double radians = theta * pi / 180.0;
        for (int x = 0; x < image.cols; ++x) {
            int y = static_cast<int>((distance - x * cos(radians)) / sin(radians));
            if (y >= 0 && y < image.rows) {
                result.data[y][x] = 255;  // ����⵽��ֱ����ԭͼ���ð�ɫ��ʾ
            }
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

