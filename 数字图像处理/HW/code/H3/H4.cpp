#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "bmp.h"

using namespace std;
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
			pre_j = width-1-j;
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
			pre_i = height-1-i;
			pre_j = j;
			if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
				outimage[i * width + j] = image[pre_i * width + pre_j];
			else
				outimage[i * width + j] = 0;
		}
	}
}
void rotate(unsigned char* image, unsigned char* outimage, int width, int height) { // 旋转图像
	cout << "请输入你想要旋转的角度" << endl;
	double degree;
	cin >> degree;
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

void translateImage(translateFunction translate) {
	ifstream input("lena.bmp", ios::binary);
	ofstream output("change.bmp", ios::binary);

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
	input.read((char*)image, width*height);
	unsigned char* outImage = new unsigned char[width*height];
	translate(image, outImage, width, height);
	output.write((char*)&fileHeader, sizeof(fileHeader));
	output.write((char*)&infoHeader, sizeof(infoHeader));
	output.write((char*)palette, 1024);
	output.write((char*)outImage, width * height);
	cout << "图像转换完成！" << endl;
	output.close();
	input.close();
	delete[] image;
	delete[] outImage;
}
void bigAndSmall() {
	ifstream input("lena.bmp", ios::binary);
	ofstream output1("change.bmp", ios::binary); // 图像缩放的实现
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
	float ExpScalValue = 0; // 缩放倍数
	cout << "请输入要缩放的倍数：" << endl;
	cin >> ExpScalValue;
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
	cout << "图像转换完成！" << endl;
	output1.close();
	input.close();
	delete[] image;
	delete[] outImage;
}

int main() {
	int choice;

	do {
		cout << "Menu:\n";
		cout << "1. Image Scaling\n";
		cout << "2. Image Translation\n";
		cout << "3. Horizontal Mirror\n";
		cout << "4. Vertical Mirror\n";
		cout << "5. Image Rotation\n";
		cout << "0. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			bigAndSmall();
			break;
		case 2:
			translateImage(move);
			break;
		case 3:
			translateImage(horizontalMirror);
			break;
		case 4:
			translateImage(verticalMirror);
			break;
		case 5:
			translateImage(rotate);
			break;
		case 0:
			std::cout << "Exiting...\n";
			break;
		default:
			std::cout << "Invalid choice. Please enter a valid option.\n";
		}
		system("cls");

	} while (choice != 0);

	return 0;
}