#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "bmp.h"
using namespace std;
typedef int (*getThreshold)(unsigned char*, int width, int height);
void translate(unsigned char* image, unsigned char* outImage, int width, int height, int thre) {
    for (int i = 0; i < width * height; i++)
        if (image[i] <= thre)
            outImage[i] = 0;
        else
            outImage[i] = 255;
}
void drawHist(unsigned char* image, int width, int height, int thre) {
    // 直方图创建BMP文件头
    unsigned char bmpFileHeader[14] = {
        'B','M',  // 文件类型
        54 + 1024, 0, 0, 0,  // 文件大小
        54, 0, 0, 0  // 数据偏移
    };

    // 直方图创建BMP信息头
    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,  // 信息头长度
        0, 1, 0, 0,  // 图像宽度
        0, 1, 0, 0,  // 图像高度
        1, 0, 8, 0,  // 颜色平面数和位深
        0, 0, 0, 0,  // 无压缩
        0, 0, 0, 0,  // 图像数据大小
        0, 0, 0, 0,  // 水平分辨率
        0, 0, 0, 0,  // 垂直分辨率
        0, 0, 0, 0,  // 颜色索引数
        0, 0, 0, 0  // 重要颜色数
    };

    // 直方图创建调色板
    unsigned char palette[1024];
    for (int i = 0; i < 256; ++i) {
        palette[i * 4] = palette[i * 4 + 1] = palette[i * 4 + 2] = i;  // 灰度图像，RGB值相同
        palette[i * 4 + 3] = 0;
    }
    // 初始化直方图
    int histogram[256] = { 0 };
    for (int i = 0; i < width*height; ++i) {
        histogram[image[i]]++;
    }
    // 找到直方图中的最大值
    int maxFrequency = 0, histogram_[256] = { 0 };
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxFrequency) {
            maxFrequency = histogram[i];
        }
    }
    //归一化
    for (int i = 0; i < 256; ++i) {
        histogram_[i] = 255 - histogram[i] * 255 / maxFrequency;
    }
    unsigned char imageData[256][256];
    //创建图像数据
    for (int y = 0; y < 256; ++y) {
        for (int x = 0; x < 256; ++x) {
            if (histogram_[x] != 0 && y <= histogram_[x]) {
                imageData[y][x] = static_cast<unsigned char>(255);
            }
            else {
                imageData[y][x] = static_cast<unsigned char>(0);
            }
        }
    }
    for (int i = 0; i < 256; i++)
        imageData[i][thre] = 100;
    ofstream hist("src_hist.bmp", ios::binary);
    hist.write(reinterpret_cast<char*>(bmpFileHeader), 14);
    hist.write(reinterpret_cast<char*>(bmpInfoHeader), 40);
    hist.write(reinterpret_cast<char*>(palette), 1024);
    for (int y = 255; y >= 0; --y)
        hist.write(reinterpret_cast<char*>(imageData[y]), 256);
}
int otsuMethod(unsigned char* image, int width, int height) {
    int Otsu_Threshold = 0; //阈值
    double variance;   //类间方差
    double maxVariance = 0, w1 = 0, w2 = 0;
    double u1 = 0, u2 = 0;
    int size = width * height;
    //生成灰度直方图
    double histgram[256] = { 0 };
    for (int i = 0; i < width * height; ++i) {
        histgram[image[i]]++;
    }
    for (int i = 0; i < 256; i++)
       histgram[i] /= size;
    //遍历找出类间方差最大对应的阈值
    for (int i = 0; i <= 255; i++) {
        w1 = 0;
        w2 = 0;
        u1 = 0;
        u2 = 0;
        for (int j = 0; j <= i; j++) {
            w1 += histgram[j];
            u1 += histgram[j] * j;
        }
        u1 /= w1;
        w2 = 1 - w1;
        if (i == 255) {
            u2 = 0;
        }
        else {
            for (int j = i + 1; j <= 255; j++)
                u2 += histgram[j] * j;
            u2 /= w2;
        }
        variance = w1 * w2 * (u1 - u2) * (u1 - u2);
        if (variance > maxVariance) {
            maxVariance = variance;
            Otsu_Threshold = i;
        }
    }
    cout << "求得的otus阈值为：" << Otsu_Threshold << endl;
    return Otsu_Threshold;
}
int iterativeMethod(unsigned char* image, int width, int height) {
    // 初始化直方图
    int histogram[256] = { 0 };
    for (int i = 0; i < width * height; ++i) {
        histogram[image[i]]++;
    }
    // 初始阈值 图像中灰度级最大值和最小值取平均
    int max_hist = 255, min_hist = 0;
    while (!histogram[max_hist])
        max_hist--;
    while (!histogram[min_hist])
        min_hist++;
    double old_thre = (max_hist + min_hist) * 0.5;
    double new_thre = (0+255)*0.5;
    double m1, m2;
    int delta = 1; // 设定终止条件变化值
    while (abs(old_thre - new_thre) > delta) {
        // 计数变量声明
        int G1 = 0;
        int count_G1 = 0;
        int G2 = 0;
        int count_G2 = 0;
        int i = 0;
        // 阈值之下的统计
        for (i = 0; i <= old_thre; i++) {
            count_G1 += histogram[i];
            G1 += histogram[i] * i;
        }
        // 阈值之上的统计
        for (; i < 256; i++) {
            count_G2 += histogram[i];
            G2 += histogram[i] * i;
        }
        m1 = G1 / count_G1;
        m2 = G2 / count_G2;
        old_thre = new_thre;
        new_thre = (m1 + m2) * 0.5;
    }
    cout << "迭代计算得到的阈值为： " << old_thre << endl;
    return old_thre;
}
int givenMethod(unsigned char* image, int width, int height) {
    return 100;
}
void ThresholdSegmentation(getThreshold method) {
    ifstream input("lena.bmp", ios::binary);
    ofstream output("threSegment.bmp", ios::binary);

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
    //图像数据存储
    unsigned char* image = new unsigned char[width * height];
    //读入数据
    input.read((char*)image, width * height);
    unsigned char* outImage = new unsigned char[width * height];
    //得到阈值
    int thre = method(image, width, height);
    // 直方图绘制，带阈值
    drawHist(image, width, height, thre);
    // 处理后得到阈值分割后的图像
    translate(image, outImage, width, height, thre);
    // 数据写入
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    output.write((char*)outImage, width * height);
    cout << "阈值分割完成！" << endl;
    output.close();
    input.close();
    delete[] image;
    delete[] outImage;
}

int main() {
    int choice;

    do {
        cout << "Image Segmentation Methods:\n";
        cout << "1. Given Threshold T\n";
        cout << "2. Iterative Threshold Method\n";
        cout << "3. Otsu Method\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ThresholdSegmentation(givenMethod);
            break;
        case 2:
            ThresholdSegmentation(iterativeMethod);
            break;
        case 3:
            ThresholdSegmentation(otsuMethod);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
        }

    } while (choice != 0);

    return 0;
	return 0;
}