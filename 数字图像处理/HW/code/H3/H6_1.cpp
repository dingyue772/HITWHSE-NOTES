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
	cout << "初始种子数为：" << seeds.size() << endl; // 初始12个种子
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
			if (result.data[neighborPoint.x][neighborPoint.y]==0 && img.data[neighborPoint.x][neighborPoint.y] > 220) {
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

int main() {
	ifstream input("6-2.bmp", ios::binary);
	ofstream output("growth.bmp", ios::binary);
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
	cout << "图像宽度为：" << width << endl;
	cout << "图像高度为：" << height << endl;
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
	return 0;
}

