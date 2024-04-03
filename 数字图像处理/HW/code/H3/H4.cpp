#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "bmp.h"

using namespace std;
// ����һ������ָ������
typedef void (*translateFunction)(unsigned char*, unsigned char*, int, int);

void move(unsigned char* image, unsigned char* outimage, int width, int height) {
	int pre_i, pre_j;// ��������ǰ���Ӧ�����ص�����
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
void horizontalMirror(unsigned char* image, unsigned char* outimage, int width, int height) { // ���Ҿ���任
	int pre_i, pre_j;// ���徵��ǰ���Ӧ�����ص�����
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
void verticalMirror(unsigned char* image, unsigned char* outimage, int width, int height) { // ��ֱ����任
	int pre_i, pre_j;// ���徵��ǰ���Ӧ�����ص�����
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
void rotate(unsigned char* image, unsigned char* outimage, int width, int height) { // ��תͼ��
	cout << "����������Ҫ��ת�ĽǶ�" << endl;
	double degree;
	cin >> degree;
	double radian = degree * 3.14 / 180;
	int pre_i, pre_j;// ������תǰ���Ӧ�����ص�����
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

	// ��дͼ���ļ�ͷ����Ϣͷ
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	input.read((char*)&fileHeader, sizeof(fileHeader));
	input.read((char*)&infoHeader, sizeof(infoHeader));
	// ��дͼ���ɫ��
	unsigned char palette[1024];
	input.read((char*)palette, 1024);
	int width = infoHeader.biWidth;
	int height = infoHeader.biHeight;
	//ͼ�����ݴ洢
	unsigned char* image = new unsigned char[width * height];
	//��������
	input.read((char*)image, width*height);
	unsigned char* outImage = new unsigned char[width*height];
	translate(image, outImage, width, height);
	output.write((char*)&fileHeader, sizeof(fileHeader));
	output.write((char*)&infoHeader, sizeof(infoHeader));
	output.write((char*)palette, 1024);
	output.write((char*)outImage, width * height);
	cout << "ͼ��ת����ɣ�" << endl;
	output.close();
	input.close();
	delete[] image;
	delete[] outImage;
}
void bigAndSmall() {
	ifstream input("lena.bmp", ios::binary);
	ofstream output1("change.bmp", ios::binary); // ͼ�����ŵ�ʵ��
	// ��дͼ���ļ�ͷ����Ϣͷ
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	input.read((char*)&fileHeader, sizeof(fileHeader));
	input.read((char*)&infoHeader, sizeof(infoHeader));

	// �������ͼ����ļ�ͷ����Ϣͷ
	BITMAPFILEHEADER fileHeader1 = fileHeader;
	BITMAPINFOHEADER infoHeader1 = infoHeader;

	// ��дͼ���ɫ��
	unsigned char palette[1024];
	input.read((char*)palette, 1024);
	//ͼ�����ݴ洢
	unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
	//��������
	input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
	// ͼ�����Ŵ���
	float ExpScalValue = 0; // ���ű���
	cout << "������Ҫ���ŵı�����" << endl;
	cin >> ExpScalValue;
	int outWidth = (int)(ExpScalValue * infoHeader.biWidth);
	int outHeight = (int)(ExpScalValue * infoHeader.biHeight);
	// ��ȡ���ͼ������ֽ���
	int lineByte = (outWidth * infoHeader.biBitCount / 8 + 3) / 4 * 4;
	infoHeader1.biWidth = outWidth;
	infoHeader1.biHeight = outHeight;
	infoHeader1.biSizeImage = lineByte * outHeight;
	fileHeader1.bfSize = infoHeader1.biSizeImage + fileHeader1.bfOffBits;
	output1.write((char*)&fileHeader1, sizeof(fileHeader1));
	output1.write((char*)&infoHeader1, sizeof(infoHeader1));
	output1.write((char*)palette, 1024);

	unsigned char* outImage = new unsigned char[lineByte * outHeight];
	int pre_i, pre_j;// ��������ǰ���Ӧ�����ص�����
	for (int i = 0; i < outHeight; i++) {
		for (int j = 0; j < outWidth; j++) {
			pre_i = (int)(i / ExpScalValue);
			pre_j = (int)(j / ExpScalValue);
			if (pre_i >= 0 && pre_i < infoHeader.biWidth && pre_j >= 0 && pre_j < infoHeader.biHeight)
				outImage[i * lineByte + j] = image[pre_i * infoHeader.biWidth + pre_j];
		}
	}
	output1.write((char*)outImage, lineByte * outHeight);
	cout << "ͼ��ת����ɣ�" << endl;
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