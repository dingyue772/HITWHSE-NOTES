#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "bmp.h"

using namespace std;

// 定义图像结构体
struct Image {
    int rows;
    int cols;
    unsigned char** data;
};
// 定义点结构体
struct Point {
    int x;
    int y;
};
// 释放图像内存
void releaseImage(Image& img) {
    for (int i = 0; i < img.rows; ++i) {
        delete[] img.data[i];
    }
    delete[] img.data;
}
bool isLabeling(const Image& img, Point p, vector<int>& label) {
    int neighbors[4][2] = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
    for (int i = 0; i < 4; i++) {
        Point neighborPoint = { p.x + neighbors[i][0], p.y + neighbors[i][1] };
        if (neighborPoint.x < 0 || neighborPoint.y < 0)
            continue;
        int neighborLabel = img.data[neighborPoint.x][neighborPoint.y];
        if (neighborLabel > 0 && find(label.begin(), label.end(), neighborLabel) == label.end()) {
            label.push_back(neighborLabel);
        }
    }
    return !label.empty(); // label为空，表示没有标记
}
void componentLabeling(Image& img) {
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (img.data[i][j]>200)
                img.data[i][j] = 255;
            else
                img.data[i][j] = 0;
        }
    }
    // 编写区域标记的逻辑 10为公差的等差数列进行数字标记
    int label = 200; // 标识当前的标记值
    // 等价表
    set<pair<int, int>> equalSet;
    // 第一轮扫描
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            vector<int> temp; // 存放当前关注点的周围4个点标记情况
            bool flag = isLabeling(img, Point{ i, j }, temp);
            if (img.data[i][j] == 255 && !flag) { // 未标记的点
                img.data[i][j] = label;
                if (label <= 245)
                    label += 10;
            }
            else if (flag) {
                if (temp.size() == 1)
                    img.data[i][j] = temp[0];
                else {
                    /*
                    for (int k = 0; k < temp.size(); k++)
                        cout << temp[k] << " ";
                    cout << endl;
                    */
                    img.data[i][j] = temp[0];
                    pair<int, int> currentPair = { temp[0], temp[1] };
                    // 添加到等价集合前先检查是否已经存在
                    if (equalSet.find(currentPair) == equalSet.end()) {
                        equalSet.insert(currentPair);
                    }
                }
            }
        }
    }
    cout << label << endl;
    cout << equalSet.size() << endl;
    // 第二轮扫描
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            for (const auto& currentPair : equalSet) {
                if (img.data[i][j] == currentPair.first || img.data[i][j] == currentPair.second)
                    img.data[i][j] = min(currentPair.first, currentPair.second);
            }
        }
    }
    
}
int main() {
    ifstream input("9-1.bmp", ios::binary);
    ofstream output("label.bmp", ios::binary);
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
    // 读入数据 这是一个二值图
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    componentLabeling(img);
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    releaseImage(img);
    return 0;
}




