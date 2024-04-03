#include <iostream>
#include <fstream>
#include <vector>
#include <set>
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
// �ͷ�ͼ���ڴ�
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
    return !label.empty(); // labelΪ�գ���ʾû�б��
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
    // ��д�����ǵ��߼� 10Ϊ����ĵȲ����н������ֱ��
    int label = 200; // ��ʶ��ǰ�ı��ֵ
    // �ȼ۱�
    set<pair<int, int>> equalSet;
    // ��һ��ɨ��
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            vector<int> temp; // ��ŵ�ǰ��ע�����Χ4���������
            bool flag = isLabeling(img, Point{ i, j }, temp);
            if (img.data[i][j] == 255 && !flag) { // δ��ǵĵ�
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
                    // ��ӵ��ȼۼ���ǰ�ȼ���Ƿ��Ѿ�����
                    if (equalSet.find(currentPair) == equalSet.end()) {
                        equalSet.insert(currentPair);
                    }
                }
            }
        }
    }
    cout << label << endl;
    cout << equalSet.size() << endl;
    // �ڶ���ɨ��
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
    // ����ԭʼͼ��
    Image img;
    img.cols = width;
    img.rows = height;
    // ͼ�����ݴ洢
    img.data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        img.data[i] = new unsigned char[width];
    }
    // �������� ����һ����ֵͼ
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




