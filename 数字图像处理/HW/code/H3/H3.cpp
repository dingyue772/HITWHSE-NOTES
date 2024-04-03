#include <iostream>
#include <fstream>
#include "bmp.h"

using namespace std;
// 掩膜窗口求均值函数
int getaverage(int* window, int winsize) {
    int sum = 0;
    int end = winsize * winsize;
    for (int i = 0; i < end; i++)
        sum += window[i];
    return sum / end;
}
// 均值滤波函数
void mean_filter(unsigned char* inbuffer, int width, int height, unsigned char* outbuffer) {
    int mid = 0;
    int winsize = 3;
    int* window = new int[winsize * winsize];
    int pos = (winsize - 1) / 2;
    memcpy(outbuffer, inbuffer, width * height);
    for (int m = pos; m < height - pos; m++) {
        for (int n = pos; n < width - pos; n++) { // (m,n)为每一窗口中心位置
            int winpos = 0;
            for (int i = -pos; i < (winsize - pos); i++) {
                for (int j = -pos; j < (winsize - pos); j++)
                    window[winpos++] = inbuffer[(m + i) * width + n + j];
                mid = getaverage(window, winsize);
                outbuffer[m * width + n] = mid;
            }
        }
    }
    delete[] window;
    window = NULL;
}
// 排序函数
void sort(int* window, int size) {
    int end = size * size;
    for (int i = 0; i < end-1; i++) {
        int k = i;
        for (int j = i+1; j < end; j++) {
            if (window[j] < window[k])
                k = j;
        }
        int temp = window[i];
        window[i] = window[k];
        window[k] = temp;
    }
}
// 中值滤波函数
void media_filter(unsigned char* inbuffer, int width, int height, unsigned char* outbuffer) {
    int mid = 0;
    int winsize = 3;//滤波窗口大小
    int* windows = new int[winsize * winsize];
    int pos = (winsize - 1) / 2;
    memcpy(outbuffer, inbuffer, width * height);
    for (int m = pos; m < height - pos; m++)
    {
        for (int n = pos; n < width - pos; n++)//（m，n）是窗口中心位置
        {
            //提取winsize*winsize的数据进入滤波窗
            int winpos = 0;
            for (int i = -pos; i < (winsize - pos); i++)
                for (int j = -pos; j < (winsize - pos); j++)
                    windows[winpos++] = inbuffer[(m + i) * width + n + j];
            //对滤波窗中的数据排序取中值
            sort(windows, winsize);
            mid = windows[(winsize * winsize) / 2];
            outbuffer[m * width + n] = mid;
        }
    }
    delete[] windows;
    windows = NULL;
}
void median() {
    ifstream input("lena.bmp", ios::binary);
    ofstream output("median_dest.bmp", ios::binary); // 均值滤波结果图像
    // 读写图像文件头和信息头
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));

    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    // 读写图像调色板
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    output.write((char*)palette, 1024);
    //图像数据存储
    unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    unsigned char* outimage = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    //读入数据
    input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
    mean_filter(image, infoHeader.biWidth, infoHeader.biHeight, outimage);
    output.write((char*)outimage, infoHeader.biWidth * infoHeader.biHeight);

    // 关闭文件和释放new内存
    input.close();
    output.close();
    delete[] image;
    delete[] outimage;
}
void media() {
    ifstream input("noise.bmp", ios::binary);
    ofstream output("media_dest.bmp", ios::binary); // 中值滤波结果图像
    // 读写图像文件头和信息头
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));

    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    // 调色板
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    output.write((char*)palette, 1024);
    //图像数据存储
    unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    unsigned char* outimage = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    //读入数据
    input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
    media_filter(image, infoHeader.biWidth, infoHeader.biHeight, outimage);
    output.write((char*)outimage, infoHeader.biWidth * infoHeader.biHeight); // 检查24位真彩色图转灰度图是否正确

    // 关闭文件和释放new内存
    input.close();
    output.close();
    delete[] image;
    delete[] outimage;
}
int main()
{
    median();
    media();
}

