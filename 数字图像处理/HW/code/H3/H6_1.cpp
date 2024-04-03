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

// �����ṹ��
struct Point {
	int x;
	int y;
};
// ���������ָ��㷨����
void regionGrowing(const Image& img, Image& result) {
	// ��ͼ��ȫ������Ϊ��
	result.rows = img.rows;
	result.cols = img.cols;
	result.data = new unsigned char* [result.rows];
	for (int i = 0; i < result.rows; ++i) {
		result.data[i] = new unsigned char[result.cols];
		for (int j = 0; j < result.cols; j++)
			result.data[i][j] = 0;
	}
	// ���ӵ㼯
	vector<Point> seeds;
	// 8������������
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
	cout << "��ʼ������Ϊ��" << seeds.size() << endl; // ��ʼ12������
	// ��ʼ����
	while (!seeds.empty()) {
		// ȡ��һ�����ӵ���Ϊ����ѭ���ĳ�ʼ���ӵ�
		Point seed_current = seeds.back();
		seeds.pop_back();

		// ����������������ڵ�
		for (int i = 0; i < 8; i++) {
			Point neighborPoint = { seed_current.x + growDirections[i][0], seed_current.y + growDirections[i][1] }; // �ڵ�
			if (neighborPoint.x < 0 || neighborPoint.y < 0 || neighborPoint.x > img.rows - 1 || neighborPoint.y > img.cols - 1) { // �ڵ㳬����Χ
				continue;
			}
			if (result.data[neighborPoint.x][neighborPoint.y]==0 && img.data[neighborPoint.x][neighborPoint.y] > 220) {
				// ����Ϊ���ӵ�
				result.data[neighborPoint.x][neighborPoint.y] = 80; // ����Ϊ��ɫ
				seeds.push_back(neighborPoint); // ѹ�����Ӽ�
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

int main() {
	ifstream input("6-2.bmp", ios::binary);
	ofstream output("growth.bmp", ios::binary);
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
	cout << "ͼ����Ϊ��" << width << endl;
	cout << "ͼ��߶�Ϊ��" << height << endl;
	// ����ԭʼͼ��
	Image img;
	img.cols = width;
	img.rows = height;
	// ͼ�����ݴ洢
	img.data = new unsigned char* [height];
	for (int i = 0; i < height; ++i) {
		img.data[i] = new unsigned char[width];
	}
	// ��������
	for (int i = 0; i < height; ++i) {
		input.read(reinterpret_cast<char*>(img.data[i]), width);
	}
	// �������ͼ��
	Image result;
	// ��������������������ͼ��ָ�
	regionGrowing(img, result);
	output.write((char*)&fileHeader, sizeof(fileHeader));
	output.write((char*)&infoHeader, sizeof(infoHeader));
	output.write((char*)palette, 1024);
	for (int i = 0; i < height; ++i) {
		output.write(reinterpret_cast<char*>(result.data[i]), width);
	}
	// �ͷ�ͼ���ڴ�
	releaseImage(img);
	releaseImage(result);
	return 0;
}

