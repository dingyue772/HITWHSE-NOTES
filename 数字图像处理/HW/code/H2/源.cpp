#include <iostream>
#include <fstream>

#define WORD short
#define DWORD int
#define LONG long

#pragma pack(2)

// BMP文件头结构体
typedef struct tagBITMAPFILEHEADER {
    WORD   	bfType; //文件类型，必须是“BM”
    DWORD	bfSize; //文件大小，包含文件头的大小
    WORD	bfReserved1; //保留字，必须设置为0
    WORD	bfReserved2; //保留字，必须设置为0
    DWORD	bfOffBits; //从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

// BMP图像信息头结构体
typedef struct tagBITMAPINFOHEADER {
    DWORD   biSize; //该结构的长度，为40
    LONG	    biWidth; //图像宽度
    LONG	    biHeight; //图像高度
    WORD	    biPlanes; //位平面数，必须为1
    WORD	    biBitCount; //颜色位数
    DWORD   biCompression; //是否压缩,，0表示不压缩
    DWORD   biSizeImage; //实际位图数据占用的字节数
    LONG	    biXPelsPerMeter;//目标设备水平分辨率,像素每米
    LONG	    biYPelsPerMeter;//目标设备垂直分辨率,像素每米
    DWORD   biClrUsed;//实际使用的颜色数
    DWORD   biClrImportant;//图像中重要的颜色数
}BITMAPINFOHEADER;

// 计算图像的直方图函数
void calculateHistogram(unsigned char* image, int* histogram,int size) {
    for (int i = 0; i < size; ++i) {
        histogram[image[i]]++;
    }
}

//直方图均衡化函数
void equalizeHistogram(unsigned char* image, int* histogram,int size) {
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

int main() {
    std::ifstream input("lena.bmp", std::ios::binary); 
    std::ofstream output_histogram("src_hist.bmp", std::ios::binary);
    std::ofstream output_equalized_histogram("dest_hist.bmp", std::ios::binary);
    std::ofstream output_equalized("dest.bmp", std::ios::binary);

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
    output_histogram.write(reinterpret_cast<char*>(palette),1024);
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

    return 0;
}
