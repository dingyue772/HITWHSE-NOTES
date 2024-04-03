#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath> 
#include <set>
#include <string.h>
#include <cstdlib> // 包含 system 函数的头文件
#include "bmp.h"
using namespace std;

typedef unsigned char BYTE;
// H1 BMP文件处理
//全局变量声明
//全局变量声明

FILE* fpSrcBmpfile;

FILE* fpDestBmpfile; // 转换得到的8位灰度图结果

FILE* fpDestBmpfile1; // 灰度图反色结果

FILE* fpDestBmpfile2[3]; //  通道分离得到的3个灰度图 

void GetBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader) {
    fread(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpSrcBmpfile);
    fread(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpSrcBmpfile);
}

void ChangeBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader, WORD wType) {
    pbiheader->biBitCount = wType; // 8位灰度图需将其值设为8 
    pbiheader->biClrUsed = (wType == 24) ? 0 : 256; // 灰度图需要使用256色调色板，需设置为256 
    pbfheader->bfOffBits = 54 + pbiheader->biClrUsed * sizeof(RGBQUAD); // bfOffBits指的是从位图头跳过多少字节可以直接到达位图数据部分 
    pbiheader->biSizeImage = ((((pbiheader->biWidth * pbiheader->biBitCount) + 31) & ~31) / 8) * pbiheader->biHeight;
    pbfheader->bfSize = pbfheader->bfOffBits + pbiheader->biSizeImage;
}

void SetBmpHeader(const PBITMAPFILEHEADER pbfheader, const PBITMAPINFOHEADER pbiheader) {
    fwrite(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpDestBmpfile);
    fwrite(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpDestBmpfile);

    fwrite(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpDestBmpfile1);
    fwrite(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpDestBmpfile1);

    fwrite(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpDestBmpfile2[0]);
    fwrite(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpDestBmpfile2[0]);

    fwrite(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpDestBmpfile2[1]);
    fwrite(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpDestBmpfile2[1]);

    fwrite(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpDestBmpfile2[2]);
    fwrite(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpDestBmpfile2[2]);
}

void SetRGBQUAD() {
    int i;
    RGBQUAD rgbquad[256];
    for (i = 0; i < 256; i++) {
        rgbquad[i].rgbBlue = i;
        rgbquad[i].rgbGreen = i;
        rgbquad[i].rgbRed = i;
        rgbquad[i].rgbReserved = 0;
    }
    // 将调色板写入位图文件中 
    fwrite(rgbquad, 256 * sizeof(RGBQUAD), 1, fpDestBmpfile);

    fwrite(rgbquad, 256 * sizeof(RGBQUAD), 1, fpDestBmpfile1);

    fwrite(rgbquad, 256 * sizeof(RGBQUAD), 1, fpDestBmpfile2[0]);
    fwrite(rgbquad, 256 * sizeof(RGBQUAD), 1, fpDestBmpfile2[1]);
    fwrite(rgbquad, 256 * sizeof(RGBQUAD), 1, fpDestBmpfile2[2]);
}

int RgbToGray()
{
    LONG w, h;
    BYTE r, g, b;
    BYTE gray, xgray;
    BYTE count24, count8; // count24为24位真彩色图像每行的填充字节数， count8为8位灰度图像每行的填充字节数；为了保证图像的每行字节数为4的倍数 
    BYTE Bmpnul = 0; // 一个字节，用于填充8位灰度图像每行的填充字节 

    const char* SrcBmpfile = "images//H1//rgb.bmp";
    const char* DestBmpfile = "images//H1//dst1.bmp";
    const char* DestBmpfile1 = "images//H1//dst2.bmp";
    const char DestBmpfile2[3][256] = {"images//H1//dst3.bmp", "images//H1//dst4.bmp", "images//H1//dst5.bmp"};

    BITMAPFILEHEADER bmfh; // bmp文件头

    BITMAPINFOHEADER bmih; // 位图信息头

    BYTE* data;

    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));//内存初始化

    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
    data = (BYTE*)malloc(3 * sizeof(BYTE));
    if (!data)
    {
        printf("Error:Can not allocate memory .\n");
        free(data);
        return -1;
    }

    if ((fpSrcBmpfile = fopen(SrcBmpfile, "rb")) == NULL || (fpDestBmpfile = fopen(DestBmpfile, "wb")) == NULL || (fpDestBmpfile1 = fopen(DestBmpfile1, "wb")) == NULL || (fpDestBmpfile2[0] = fopen(DestBmpfile2[0], "wb")) == NULL || (fpDestBmpfile2[1] = fopen(DestBmpfile2[1], "wb")) == NULL || (fpDestBmpfile2[2] = fopen(DestBmpfile2[2], "wb")) == NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }

    rewind(fpSrcBmpfile); // 使用rewind()函数将文件指针设置到文件开头 
    GetBmpHeader(&bmfh, &bmih);

    ChangeBmpHeader(&bmfh, &bmih, 8);
    SetBmpHeader(&bmfh, &bmih);
    SetRGBQUAD();
    count24 = (4 - (bmih.biWidth * 3) % 4) % 4;
    count8 = (4 - (bmih.biWidth) % 4) % 4;

    // 从图像底部向顶部遍历每一行像素点 
    for (h = bmih.biHeight - 1; h >= 0; h--)
    {
        for (w = 0; w < bmih.biWidth; w++)
        {
            fread(data, 3, 1, fpSrcBmpfile);
            if (feof(fpSrcBmpfile))
            {
                printf("Error:Read Pixel data failed!\n");
                free(data);
                return -1;
            }
            b = *data;
            g = *(data + 1);
            r = *(data + 2);
            gray = (299 * r + 587 * g + 114 * b) / 1000;
            //if(gray>120)gray=250;
            // 反色灰度图的像素点值
            xgray = 255 - gray;

            fwrite(&gray, sizeof(gray), 1, fpDestBmpfile);
            fwrite(&xgray, sizeof(xgray), 1, fpDestBmpfile1);

            fwrite(&b, sizeof(b), 1, fpDestBmpfile2[0]);
            fwrite(&g, sizeof(g), 1, fpDestBmpfile2[1]);
            fwrite(&r, sizeof(r), 1, fpDestBmpfile2[2]);
        }
        fseek(fpSrcBmpfile, count24, SEEK_CUR); // 将源位图文件指针向后移动count24个字节 
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile1);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[0]);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[1]);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[2]);
    }
    free(data);//释放内存空间

    fclose(fpDestBmpfile);//关闭文件指针
    fclose(fpSrcBmpfile);
    fclose(fpDestBmpfile1);
    for (int i = 0; i < 3; i++)
        fclose(fpDestBmpfile2[i]);
    return 0;
}


// H2 直方图处理
// 计算图像的直方图函数
void calculateHistogram(unsigned char* image, int* histogram, int size) {
    for (int i = 0; i < size; ++i) {
        histogram[image[i]]++;
    }
}
//直方图均衡化函数
void equalizeHistogram(unsigned char* image, int* histogram, int size) {
    int totalPixels = size;
    float cumulativeProbability[256] = { 0 };
    unsigned char mapping[256] = { 0 };
    // 计算累积概率分布
    cumulativeProbability[0] = (float)histogram[0] / totalPixels;
    for (int i = 1; i < 256; ++i) {
        cumulativeProbability[i] = cumulativeProbability[i - 1] + (float)histogram[i] / totalPixels;
    }
    // 映射新的灰度级
    for (int i = 0; i < 256; ++i) {
        mapping[i] = (unsigned char)(255 * cumulativeProbability[i] + 0.5);
    }
    // 应用映射
    for (int i = 0; i < totalPixels; ++i) {
        image[i] = mapping[image[i]];
    }
}
//创建直方图
void createHistogramImage(const int histogram[256], unsigned char imageData[256][256]) {
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
}
int histogram() {
    ifstream input("images//H2//lena.bmp", ios::binary);
    ofstream output_histogram("images//H2//src_hist.bmp", ios::binary);
    ofstream output_equalized_histogram("images//H2//dest_hist.bmp", ios::binary);
    ofstream output_equalized("images//H2//dest.bmp", ios::binary);

    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info_header;

    // 读取BMP文件头
    input.read((char*)&header, sizeof(header));

    // 读取BMP图像信息头
    input.read((char*)&info_header, sizeof(info_header));

    // 输出BMP文件头和图像信息头到输出文件
    output_equalized.write((char*)&header, sizeof(header));
    output_equalized.write((char*)&info_header, sizeof(info_header));

    // 写入调色板数据
    for (int i = 0; i < 256; i++) {
        output_equalized.put(i);
        output_equalized.put(i);
        output_equalized.put(i);
        output_equalized.put(0);
    }

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

    //图像数据存储
    unsigned char* image = new unsigned char[info_header.biWidth * info_header.biHeight];

    // 初始化直方图
    int histogram[256] = { 0 };

    //初始化直方图输出
    unsigned char imageData[256][256];

    //读入数据
    input.read((char*)image, info_header.biWidth * info_header.biHeight);

    // 计算\绘制直方图
    calculateHistogram(image, histogram, info_header.biWidth * info_header.biHeight);
    createHistogramImage(histogram, imageData);
    output_histogram.write(reinterpret_cast<char*>(bmpFileHeader), 14);
    output_histogram.write(reinterpret_cast<char*>(bmpInfoHeader), 40);
    output_histogram.write(reinterpret_cast<char*>(palette), 1024);
    for (int y = 255; y >= 0; --y) {
        output_histogram.write(reinterpret_cast<char*>(imageData[y]), 256);
    }

    // 执行直方图均衡化
    equalizeHistogram(image, histogram, info_header.biWidth * info_header.biHeight);

    // 计算\绘制直方图
    calculateHistogram(image, histogram, info_header.biWidth * info_header.biHeight);
    createHistogramImage(histogram, imageData);
    output_equalized_histogram.write(reinterpret_cast<char*>(bmpFileHeader), 14);
    output_equalized_histogram.write(reinterpret_cast<char*>(bmpInfoHeader), 40);
    output_equalized_histogram.write(reinterpret_cast<char*>(palette), 1024);
    for (int y = 255; y >= 0; --y) {
        output_equalized_histogram.write(reinterpret_cast<char*>(imageData[y]), 256);
    }

    // 保存处理后的图像数据
    output_equalized.write((char*)image, info_header.biWidth * info_header.biHeight);

    // 释放内存
    delete[] image;

    // 关闭文件
    input.close();
    output_equalized.close();
    output_histogram.close();
    output_equalized_histogram.close();
}



// H3 空间滤波
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
    for (int i = 0; i < end - 1; i++) {
        int k = i;
        for (int j = i + 1; j < end; j++) {
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
    ifstream input("images//H3//lena.bmp", ios::binary);
    ofstream output("images//H3//median_dest.bmp", ios::binary); // 均值滤波结果图像
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
    ifstream input("images//H3//noise.bmp", ios::binary);
    ofstream output("images//H3//media_dest.bmp", ios::binary); // 中值滤波结果图像
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




// H4 图像变换
// 声明一个函数指针类型
typedef void (*translateFunction)(unsigned char*, unsigned char*, int, int);
void move(unsigned char* image, unsigned char* outimage, int width, int height) {
    int pre_i, pre_j;// 定义缩放前后对应的像素点坐标
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pre_i = i - 50;
            pre_j = j - 50;
            if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
                outimage[i * width + j] = image[pre_i * width + pre_j];
            else
                outimage[i * width + j] = 0;
        }
    }
}
void horizontalMirror(unsigned char* image, unsigned char* outimage, int width, int height) { // 左右镜像变换
    int pre_i, pre_j;// 定义镜像前后对应的像素点坐标
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pre_i = i;
            pre_j = width - 1 - j;
            if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
                outimage[i * width + j] = image[pre_i * width + pre_j];
            else
                outimage[i * width + j] = 0;
        }
    }
}
void verticalMirror(unsigned char* image, unsigned char* outimage, int width, int height) { // 垂直镜像变换
    int pre_i, pre_j;// 定义镜像前后对应的像素点坐标
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pre_i = height - 1 - i;
            pre_j = j;
            if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
                outimage[i * width + j] = image[pre_i * width + pre_j];
            else
                outimage[i * width + j] = 0;
        }
    }
}
void rotate(unsigned char* image, unsigned char* outimage, int width, int height) { // 旋转图像
    double degree = 30;
    double radian = degree * 3.14 / 180;
    int pre_i, pre_j;// 定义旋转前后对应的像素点坐标
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pre_i = j * sin(radian) + i * cos(radian);
            pre_j = j * cos(radian) - i * sin(radian);
            if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
                outimage[i * width + j] = image[pre_i * width + pre_j];
            else
                outimage[i * width + j] = 0;
        }
    }
}
void translateImage(translateFunction translate, string fileName) {
    ifstream input("images//H4//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);

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
    translate(image, outImage, width, height);
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    output.write((char*)outImage, width * height);
    output.close();
    input.close();
    delete[] image;
    delete[] outImage;
}
void bigAndSmall(float val, string fileName) { // 传入参数为放大或者缩小的倍数
    ifstream input("images//H4//lena.bmp", ios::binary);
    ofstream output1(fileName, ios::binary); // 图像缩放的实现
    // 读写图像文件头和信息头
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));

    // 设置输出图像的文件头和信息头
    BITMAPFILEHEADER fileHeader1 = fileHeader;
    BITMAPINFOHEADER infoHeader1 = infoHeader;

    // 读写图像调色板
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    //图像数据存储
    unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    //读入数据
    input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
    // 图像缩放处理
    float ExpScalValue = val; // 缩放倍数
    int outWidth = (int)(ExpScalValue * infoHeader.biWidth);
    int outHeight = (int)(ExpScalValue * infoHeader.biHeight);
    // 求取输出图像的行字节数
    int lineByte = (outWidth * infoHeader.biBitCount / 8 + 3) / 4 * 4;
    infoHeader1.biWidth = outWidth;
    infoHeader1.biHeight = outHeight;
    infoHeader1.biSizeImage = lineByte * outHeight;
    fileHeader1.bfSize = infoHeader1.biSizeImage + fileHeader1.bfOffBits;
    output1.write((char*)&fileHeader1, sizeof(fileHeader1));
    output1.write((char*)&infoHeader1, sizeof(infoHeader1));
    output1.write((char*)palette, 1024);

    unsigned char* outImage = new unsigned char[lineByte * outHeight];
    int pre_i, pre_j;// 定义缩放前后对应的像素点坐标
    for (int i = 0; i < outHeight; i++) {
        for (int j = 0; j < outWidth; j++) {
            pre_i = (int)(i / ExpScalValue);
            pre_j = (int)(j / ExpScalValue);
            if (pre_i >= 0 && pre_i < infoHeader.biWidth && pre_j >= 0 && pre_j < infoHeader.biHeight)
                outImage[i * lineByte + j] = image[pre_i * infoHeader.biWidth + pre_j];
        }
    }
    output1.write((char*)outImage, lineByte * outHeight);
    output1.close();
    input.close();
    delete[] image;
    delete[] outImage;
}




// H5 阈值分割
typedef int (*getThreshold)(unsigned char*, int width, int height);
void translate(unsigned char* image, unsigned char* outImage, int width, int height, int thre) {
    for (int i = 0; i < width * height; i++)
        if (image[i] <= thre)
            outImage[i] = 0;
        else
            outImage[i] = 255;
}
void drawHist(unsigned char* image, int width, int height, int thre, string fileName) {
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
    for (int i = 0; i < width * height; ++i) {
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
    string histFile = fileName + "_hist.bmp";
    ofstream hist(histFile, ios::binary);
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
    double new_thre = (0 + 255) * 0.5;
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
    return old_thre;
}
int givenMethod(unsigned char* image, int width, int height) {
    return 100;
}
void ThresholdSegmentation(getThreshold method, string fileName) {
    ifstream input("images//H5//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);

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
    drawHist(image, width, height, thre, fileName);
    // 处理后得到阈值分割后的图像
    translate(image, outImage, width, height, thre);
    // 数据写入
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    output.write((char*)outImage, width * height);
    output.close();
    input.close();
    delete[] image;
    delete[] outImage;
}


// H6 区域分割
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
// 区域生长分割算法定义
void regionGrowing(const Image& img, Image& result) {
    // 将图像全部设置为黑
    result.rows = img.rows;
    result.cols = img.cols;
    result.data = new unsigned char* [result.rows];
    for (int i = 0; i < result.rows; ++i) {
        result.data[i] = new unsigned char[result.cols];
        for (int j = 0; j < result.cols; j++)
            result.data[i][j] = 0;
    }
    // 种子点集
    vector<Point> seeds;
    // 8连接生长方向
    int growDirections[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1} };
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (img.data[i][j] > 250) {
                result.data[i][j] = 255;
                Point p = { i,j };
                seeds.push_back(p);
            }
        }
    }
    // 开始生长
    while (!seeds.empty()) {
        // 取出一个种子点作为现在循环的初始种子点
        Point seed_current = seeds.back();
        seeds.pop_back();

        // 遍历各生长方向的邻点
        for (int i = 0; i < 8; i++) {
            Point neighborPoint = { seed_current.x + growDirections[i][0], seed_current.y + growDirections[i][1] }; // 邻点
            if (neighborPoint.x < 0 || neighborPoint.y < 0 || neighborPoint.x > img.rows - 1 || neighborPoint.y > img.cols - 1) { // 邻点超出范围
                continue;
            }
            if (result.data[neighborPoint.x][neighborPoint.y] == 0 && img.data[neighborPoint.x][neighborPoint.y] > 220) {
                // 设置为种子点
                result.data[neighborPoint.x][neighborPoint.y] = 80; // 设置为灰色
                seeds.push_back(neighborPoint); // 压入种子集
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
void regionBaseGrowth() {
    ifstream input("images//H6//before.bmp", ios::binary);
    ofstream output("images//H6//growth.bmp", ios::binary);
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
    // 创建结果图像
    Image result;
    // 调用区域生长函数进行图像分割
    regionGrowing(img, result);
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // 释放图像内存
    releaseImage(img);
    releaseImage(result);
}
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
    if (isSimilar(img, region, threshold) || (width < winsize && height < winsize)) {
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
        {region.topLeftX, region.topLeftY, midX - 1, midY - 1},
        {midX + 1, region.topLeftY, region.bottomRightX, midY - 1},
        {region.topLeftX, midY + 1, midX - 1, region.bottomRightY},
        {midX + 1, midY + 1, region.bottomRightX, region.bottomRightY}
    };

    // 递归对每个子区域进行分裂
    for (int i = 0; i < 4; ++i) {
        regionSplit(img, subregions[i], threshold);
    }
}
void regionBaseSplit() {
    ifstream input("images//H6//before.bmp", ios::binary);
    ofstream output("images//H6//split.bmp", ios::binary);
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
}



// H7 边缘检测
// 计算差分
int computeDifference(const Image& input, int i, int j, const int mask[3][3]) {
    int result = 0;
    for (int m = -1; m <= 1; ++m) {
        for (int n = -1; n <= 1; ++n) {
            result += input.data[i + m][j + n] * mask[m + 1][n + 1];
        }
    }
    return result;
}
// 计算梯度幅值
void computeGradient(const Image& input, Image& output, const int maskX[3][3], const int maskY[3][3], int thre) {
    for (int i = 1; i < input.rows - 1; ++i) {
        for (int j = 1; j < input.cols - 1; ++j) {
            int gradientX = abs(computeDifference(input, i, j, maskX));
            int gradientY = abs(computeDifference(input, i, j, maskY));
            // 计算梯度幅值
            int gradient = gradientX + gradientY;
            if (gradient >= thre)
                output.data[i][j] = 255;
            else
                output.data[i][j] = 0;
        }
    }
}
// 计算差分
int computeDifference(const Image& input, int i, int j, const int mask[5][5]) {
    int result = 0;
    for (int m = -2; m <= 2; ++m) {
        for (int n = -2; n <= 2; ++n) {
            result += input.data[i + m][j + n] * mask[m + 2][n + 2];
        }
    }
    return result;
}
// 计算梯度幅值的重载函数
void computeGradient(const Image& input, Image& output, const int mask[5][5], int thre) {
    for (int i = 2; i < input.rows - 2; ++i) {
        for (int j = 2; j < input.cols - 2; ++j) {
            int gradient = computeDifference(input, i, j, mask);
            if (gradient >= thre)
                output.data[i][j] = 255;
            else
                output.data[i][j] = 0;
        }
    }
}
// 边缘检测主函数
void edgeDetection1(const int maskX[3][3], const int maskY[3][3], string fileName) {
    ifstream input("images//H7//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);
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
    // 创建结果图像
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

    // 设置阈值
    int threshold = 230;
    computeGradient(img, result, maskX, maskY, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // 释放图像内存
    releaseImage(img);
    releaseImage(result);
}
void edgeDetection2(const int mask[5][5], string fileName) {
    ifstream input("images//H7//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);
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
    // 创建结果图像
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

    // 设置阈值
    int threshold = 230;
    computeGradient(img, result, mask, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // 释放图像内存
    releaseImage(img);
    releaseImage(result);
}


// H8 Hough变换
typedef unsigned char BYTE;
#pragma pack(1)   //全紧凑模式
using namespace std;

typedef struct
{
    unsigned char bfType[2];
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} bitmapFileHeader; //位图文件头

typedef struct
{
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPixPerMeter;
    unsigned int biYPixPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} bitmapInfoHeader; //位图信息头

typedef struct
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} rgbQUAD; //调色板

typedef struct
{
    bitmapFileHeader bfHeader;
    bitmapInfoHeader biHeader;
    //rgbQUAD palette[256];
    unsigned char* imgData;
} bmp;
void houghTransform() {
    FILE* fp = fopen("images//H8//before.bmp", "rb");
    if (fp == 0)
        return ;
    bitmapFileHeader fileHead;
    fread(&fileHead, sizeof(bitmapFileHeader), 1, fp);

    bitmapInfoHeader infoHead;
    fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);
    int bmpsize = infoHead.biSizeImage;
    int width = infoHead.biWidth;
    int height = infoHead.biHeight;
    int biCount = infoHead.biBitCount;
    int lineByte = (biCount * width / 8 + 3) / 4 * 4;
    rgbQUAD* pColorTable;
    pColorTable = new rgbQUAD[256];
    fread(pColorTable, sizeof(rgbQUAD), 256, fp);

    unsigned char* pBmpBuf;
    pBmpBuf = new unsigned char[lineByte * height];
    fread(pBmpBuf, lineByte * height, 1, fp);
    fclose(fp);
    const char* savePath;


    unsigned char* pBmpBuf2;
    // 初始化
    pBmpBuf2 = new unsigned char[lineByte * height];

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {

            *(pBmpBuf2 + i * lineByte + j) = *(pBmpBuf + i * lineByte + j);
        }
    }

    savePath = "images//H8//after.bmp";
    FILE* fop = fopen(savePath, "wb");
    if (fop == 0)
        return ;
    fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
    fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);


    int rho;//到原点距离
    int d = sqrt(width * width + height * height);
    int platoon_bit = bmpsize / height - width * (biCount / 8);
    int* A = new int[d * 2 * 180];//
    int* theta = new int[180];//
    stack<int> lineAIndexStack;
    int lineAIndexLength = 0;
    for (int i = 0; i < 180; i++)
    {
        theta[i] = i;
    }
    for (int i = 0; i < 180 * d * 2; i++)
    {
        A[i] = 0;
    }


    for (int i = 1; i < height; i++)
    {
        for (int j = 1; j < width; j++)
        {
            if (*(pBmpBuf2 + i * lineByte + j) == 255)//白色
            {
                for (int k = 0; k < 180; k++)
                {
                    rho = int(j * cos(theta[k] * 3.14 / 180) + i * sin(theta[k] * 3.14 / 180)) % d;
                    rho = rho + d;//防止为负数
                    A[rho * 180 + k]++;
                }
            }
        }
    }

    for (int i = 0; i < d * 2; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            if (A[i * 180 + j] >= 80) // 设定阈值，哪些点对应的直线被视作是存在的
            {
                lineAIndexStack.push(i * 180 + j);
                lineAIndexLength++;

            }
        }
    }

    int* lineAIndexArray = new int[lineAIndexLength];
    int i = 0;
    while (!lineAIndexStack.empty())
    {
        lineAIndexArray[i] = lineAIndexStack.top();
        lineAIndexStack.pop();
        i++;
    }

    BYTE gbquadIndex;
    int k;
    int mh[10];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            gbquadIndex = 0;
            //判断这个像素点是否在某条直线上面
            for (int i = 0; i < lineAIndexLength; i++)
            {
                int t = lineAIndexArray[i] % 180;//theta
                int r = lineAIndexArray[i] / 180 - d;//rho

                //xcos(a)+ysin(a)=r
                if ((abs(column * cos(t * 3.14 / 180) + row * sin(t * 3.14 / 180) - r) <= 0.0001))
                    gbquadIndex = 255;

                if (t == 90 && row == r)
                    gbquadIndex = 255;

            }
            fwrite(&gbquadIndex, sizeof(BYTE), 1, fop);
        }
        BYTE zero = 0;
        fwrite(&zero, sizeof(BYTE) * platoon_bit, 1, fop);
    }

    fclose(fop);
}


// H9 区域标记
int write(const char savePath[], bmp b)
{
    FILE* f = fopen(savePath, "wb");
    if (f == NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader, sizeof(bitmapFileHeader), 1, f);
    fwrite(&b.biHeader, sizeof(bitmapInfoHeader), 1, f);
    fwrite(b.imgData, sizeof(unsigned char) * b.biHeader.biSizeImage, 1, f);
    fclose(f);

}
int regionLabeling() {
    FILE* fp;
    if ((fp = fopen("images//H9//before.bmp", "rb")) == NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader, 14, 1, fp);//读位图文件头
    bitmapInfoHeader biHeader;
    fread(&biHeader, 40, 1, fp);//读位图信息头
    int imSize = biHeader.biSizeImage;//文件大小
    int width = biHeader.biWidth;
    int height = biHeader.biHeight;
    int bitCount = biHeader.biBitCount;//颜色位数
    int lineBytes = (width * bitCount + 31) / 32 * 4;//行字节数

    fseek(fp, bfHeader.bfOffBits, SEEK_SET);
    unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
    int* Flag = (int*)malloc(imSize * sizeof(int));
    memset(Flag, 0, imSize * sizeof(int)); // 标记数组
    fread(imageData, imSize * sizeof(unsigned char), 1, fp);//读位图数据
    fclose(fp);

    bmp b;
    int c = 1;
    memcpy(&(b.bfHeader), &bfHeader, sizeof(bfHeader));
    memcpy(&(b.biHeader), &biHeader, sizeof(biHeader));
    b.imgData = (unsigned char*)malloc(sizeof(unsigned char) * b.biHeader.biSizeImage);
    memset(b.imgData, 0, sizeof(unsigned char) * b.biHeader.biSizeImage);
    int equal_value[300][300] = { 0 };
    const char* savePath = "images//H9//after.bmp";

    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            if (imageData[lineBytes * i + j * 3] != 0)//不是黑色
            {
                if (i == height-1 && j == 0)//第一行第一个点
                {
                    Flag[lineBytes * i + j * 3] = c++;
                }
                int count = 0;
                int a[4] = { 0 };
                for (int n = i; n <= i + 1; n++)
                {
                    if (n < 0 || n >= height)//防止越界
                        continue;
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m < 0 || m >= width)//防止越界
                            continue;
                        if (n == i && m > j - 1)//只判断四个像素点 左、左上、上、右上
                            continue;

                        if (Flag[lineBytes * n + m * 3] != 0)
                        {
                            Flag[lineBytes * i + j * 3] = Flag[lineBytes * n + m * 3]; // 这里已经处理了4邻域块中有一个标记的情况

                            a[count] = Flag[lineBytes * n + m * 3];//如果有两个标记的情况
                            count++;
                        }

                    }
                }
                if (count == 0)//如果都是背景
                {
                    Flag[lineBytes * i + j * 3] = c++;
                }
                if (count == 2 && a[0] != a[1])
                {
                    equal_value[a[0]][a[1]] = equal_value[a[1]][a[0]] = 1;//进等价表
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (Flag[lineBytes * i + j * 3] != 0)
            {
                bool flag = true;
                while (flag)
                {
                    flag = false;
                    int n = Flag[lineBytes * i + j * 3];
                    for (int m = n - 1; m >= 0; m--)//合并区域,找最小标记
                    {
                        if (equal_value[n][m] == 1) // 将标记值更改一次就继续寻找，看是否有最小的
                        {
                            Flag[lineBytes * i + j * 3] = m;
                            flag = true;
                        }
                    }
                }
                b.imgData[lineBytes * i + j * 3] = Flag[lineBytes * i + j * 3];
                b.imgData[lineBytes * i + j * 3 + 1] = Flag[lineBytes * i + j * 3];
                b.imgData[lineBytes * i + j * 3 + 2] = Flag[lineBytes * i + j * 3];

                int k = Flag[lineBytes * i + j * 3] % 3;//0-2
                b.imgData[lineBytes * i + j * 3 + k] = 255;
            }
        }
    }

    write(savePath, b);
    free(imageData);
    free(b.imgData);
}


// H10 轮廓提取
int contourExtraction() {
    FILE* fp;
    if ((fp = fopen("images//H10//before.bmp", "rb")) == NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader, 14, 1, fp);//读位图文件头
    bitmapInfoHeader biHeader;
    fread(&biHeader, 40, 1, fp);//读位图信息头
    int imSize = biHeader.biSizeImage;//文件大小
    int width = biHeader.biWidth;
    int height = biHeader.biHeight;
    int bitCount = biHeader.biBitCount;//颜色位数
    int lineBytes = (width * bitCount + 31) / 32 * 4;//行字节数

    fseek(fp, bfHeader.bfOffBits, SEEK_SET);
    unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
    int* Flag = (int*)malloc(imSize * sizeof(int));
    memset(Flag, 0, imSize * sizeof(int));
    fread(imageData, imSize * sizeof(unsigned char), 1, fp);//读位图数据
    fclose(fp);

    bmp b;

    memcpy(&(b.bfHeader), &bfHeader, sizeof(bfHeader));
    memcpy(&(b.biHeader), &biHeader, sizeof(biHeader));
    b.imgData = (unsigned char*)malloc(sizeof(unsigned char) * b.biHeader.biSizeImage);
    memset(b.imgData, 0, sizeof(unsigned char) * b.biHeader.biSizeImage);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            b.imgData[lineBytes * i + j * 3] = imageData[lineBytes * i + j * 3];
            b.imgData[lineBytes * i + j * 3 + 1] = imageData[lineBytes * i + j * 3 + 1];
            b.imgData[lineBytes * i + j * 3 + 2] = imageData[lineBytes * i + j * 3 + 2];
        }
    }

    const char* savePath = "images//H10//after.bmp";

    //如果周围八个点为黑/内部点则为内部点
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (b.imgData[lineBytes * i + j * 3] == 0) //如果是黑色
            {
                int count = 0;
                for (int n = i - 1; n <= i + 1; n++)
                {
                    if (n < 0 || n >= height)//防止越界
                    {
                        count++;
                        continue;
                    }
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m < 0 || m >= width)//防止越界
                        {
                            count++;
                            continue;
                        }
                        if (b.imgData[lineBytes * n + m * 3] == 0 || Flag[lineBytes * n + m * 3] == 1)
                        {
                            count++;
                        }
                    }
                }
                if (count == 9)
                {
                    Flag[lineBytes * i + j * 3] = 1;
                    b.imgData[lineBytes * i + j * 3] = 255;
                    b.imgData[lineBytes * i + j * 3 + 1] = 255;
                    b.imgData[lineBytes * i + j * 3 + 2] = 255;
                }
            }
        }
    }

    write(savePath, b);

    free(imageData);
    free(b.imgData);
}




void processBMPFile() {
    // 处理BMP文件的代码
    RgbToGray();
    cout << "执行BMP文件处理的代码..." << endl;
}

void processHistogram() {
    // 直方图处理的代码
    histogram();
    cout << "执行直方图处理的代码..." << endl;
}

void processSpatialFiltering() {
    // 空间滤波的代码
    median();
    media();
    cout << "执行空间滤波的代码..." << endl;
}

void processImageTransformation() {
    // 图像变换的代码
    bigAndSmall(2, "images//H4//big.bmp"); // 放大两倍
    bigAndSmall(0.5, "images//H4//small.bmp");
    translateImage(move, "images//H4//move.bmp");
    translateImage(horizontalMirror, "images//H4//Hmirror.bmp");
    translateImage(verticalMirror, "images//H4//Vmirror.bmp");
    translateImage(rotate, "images//H4//rotate.bmp");
    cout << "执行图像变换的代码..." << endl;
}

void processThresholdSegmentation() {
    // 阈值分割的代码
    ThresholdSegmentation(givenMethod, "images//H5//threSegment1.bmp");
    ThresholdSegmentation(iterativeMethod, "images//H5//threSegment2.bmp");
    ThresholdSegmentation(otsuMethod, "images//H5//threSegment3.bmp");
    cout << "执行阈值分割的代码..." << endl;
}

void processRegionBasedSegmentation() {
    // 基于区域分割的代码
    regionBaseGrowth();
    regionBaseSplit();
    cout << "执行基于区域分割的代码..." << endl;
}

void processEdgeDetection() {
    // 边缘检测的代码
    // 定义Sobel算子
    const int sobelMaskX[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    const int sobelMaskY[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    // 定义Prewitt算子
    const int prewittMaskX[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    const int prewittMaskY[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    // 定义log算子
    const int log[5][5] = { {0,0,-1,0,0}, {0, -1, -2, -1, 0}, {-1, -2, 16, -2, -1}, {0, -1, -2, -1, 0}, {0,0,-1,0,0} };
    edgeDetection1(sobelMaskX, sobelMaskY, "images//H7//sobel.bmp");
    edgeDetection1(prewittMaskX, prewittMaskY, "images//H7//prewitte.bmp");
    edgeDetection2(log, "images//H7//log.bmp");
    cout << "执行边缘检测的代码..." << endl;
}

void processHoughTransform() {
    // Hough变换的代码
    houghTransform();
    cout << "执行Hough变换的代码..." << endl;
}

void processRegionLabeling() {
    // 区域标记的代码
    regionLabeling();
    cout << "执行区域标记的代码..." << endl;
}

void processContourExtraction() {
    // 轮廓提取的代码
    contourExtraction();
    cout << "执行轮廓提取的代码..." << endl;
}

void processAllFunctions() {
    // 实现所有功能的批处理
    processBMPFile();
    processHistogram();
    processSpatialFiltering();
    processImageTransformation();
    processThresholdSegmentation();
    processRegionBasedSegmentation();
    processEdgeDetection();
    processHoughTransform();
    processRegionLabeling();
    processContourExtraction();
    cout << "执行所有功能批处理的代码..." << endl;
}

int main() {
    int userInput;

    do {
        // 显示菜单
        cout << "===== 数字图像处理系统菜单 =====" << endl;
        cout << "  0. 所有功能批处理" << endl;
        cout << "  1. BMP文件处理" << endl;
        cout << "  2. 直方图处理" << endl;
        cout << "  3. 空间滤波" << endl;
        cout << "  4. 图像变换" << endl;
        cout << "  5. 阈值分割" << endl;
        cout << "  6. 基于区域分割" << endl;
        cout << "  7. 边缘检测" << endl;
        cout << "  8. Hough变换" << endl;
        cout << "  9. 区域标记" << endl;
        cout << " 10. 轮廓提取" << endl;
        cout << "=============================" << endl;
        cout << "请输入相应的数字进行选择 (输入-1退出): ";
        cin >> userInput;

        // 根据用户输入执行相应的功能
        switch (userInput) {
        case 0:
            processAllFunctions();
            break;
        case 1:
            processBMPFile();
            break;
        case 2:
            processHistogram();
            break;
        case 3:
            processSpatialFiltering();
            break;
        case 4:
            processImageTransformation();
            break;
        case 5:
            processThresholdSegmentation();
            break;
        case 6:
            processRegionBasedSegmentation();
            break;
        case 7:
            processEdgeDetection();
            break;
        case 8:
            processHoughTransform();
            break;
        case 9:
            processRegionLabeling();
            break;
        case 10:
            processContourExtraction();
            break;
        case -1:
            cout << "退出程序。" << endl;
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
        }
        // 清屏
        system("pause");
        system("cls");

    } while (userInput != -1);

    return 0;
}
