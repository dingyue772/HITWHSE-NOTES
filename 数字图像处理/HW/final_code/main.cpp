#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath> 
#include <set>
#include <string.h>
#include <cstdlib> // ���� system ������ͷ�ļ�
#include "bmp.h"
using namespace std;

typedef unsigned char BYTE;
// H1 BMP�ļ�����
//ȫ�ֱ�������
//ȫ�ֱ�������

FILE* fpSrcBmpfile;

FILE* fpDestBmpfile; // ת���õ���8λ�Ҷ�ͼ���

FILE* fpDestBmpfile1; // �Ҷ�ͼ��ɫ���

FILE* fpDestBmpfile2[3]; //  ͨ������õ���3���Ҷ�ͼ 

void GetBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader) {
    fread(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpSrcBmpfile);
    fread(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpSrcBmpfile);
}

void ChangeBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader, WORD wType) {
    pbiheader->biBitCount = wType; // 8λ�Ҷ�ͼ�轫��ֵ��Ϊ8 
    pbiheader->biClrUsed = (wType == 24) ? 0 : 256; // �Ҷ�ͼ��Ҫʹ��256ɫ��ɫ�壬������Ϊ256 
    pbfheader->bfOffBits = 54 + pbiheader->biClrUsed * sizeof(RGBQUAD); // bfOffBitsָ���Ǵ�λͼͷ���������ֽڿ���ֱ�ӵ���λͼ���ݲ��� 
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
    // ����ɫ��д��λͼ�ļ��� 
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
    BYTE count24, count8; // count24Ϊ24λ���ɫͼ��ÿ�е�����ֽ����� count8Ϊ8λ�Ҷ�ͼ��ÿ�е�����ֽ�����Ϊ�˱�֤ͼ���ÿ���ֽ���Ϊ4�ı��� 
    BYTE Bmpnul = 0; // һ���ֽڣ��������8λ�Ҷ�ͼ��ÿ�е�����ֽ� 

    const char* SrcBmpfile = "images//H1//rgb.bmp";
    const char* DestBmpfile = "images//H1//dst1.bmp";
    const char* DestBmpfile1 = "images//H1//dst2.bmp";
    const char DestBmpfile2[3][256] = {"images//H1//dst3.bmp", "images//H1//dst4.bmp", "images//H1//dst5.bmp"};

    BITMAPFILEHEADER bmfh; // bmp�ļ�ͷ

    BITMAPINFOHEADER bmih; // λͼ��Ϣͷ

    BYTE* data;

    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));//�ڴ��ʼ��

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

    rewind(fpSrcBmpfile); // ʹ��rewind()�������ļ�ָ�����õ��ļ���ͷ 
    GetBmpHeader(&bmfh, &bmih);

    ChangeBmpHeader(&bmfh, &bmih, 8);
    SetBmpHeader(&bmfh, &bmih);
    SetRGBQUAD();
    count24 = (4 - (bmih.biWidth * 3) % 4) % 4;
    count8 = (4 - (bmih.biWidth) % 4) % 4;

    // ��ͼ��ײ��򶥲�����ÿһ�����ص� 
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
            // ��ɫ�Ҷ�ͼ�����ص�ֵ
            xgray = 255 - gray;

            fwrite(&gray, sizeof(gray), 1, fpDestBmpfile);
            fwrite(&xgray, sizeof(xgray), 1, fpDestBmpfile1);

            fwrite(&b, sizeof(b), 1, fpDestBmpfile2[0]);
            fwrite(&g, sizeof(g), 1, fpDestBmpfile2[1]);
            fwrite(&r, sizeof(r), 1, fpDestBmpfile2[2]);
        }
        fseek(fpSrcBmpfile, count24, SEEK_CUR); // ��Դλͼ�ļ�ָ������ƶ�count24���ֽ� 
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile1);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[0]);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[1]);
        fwrite(&Bmpnul, 1, count8, fpDestBmpfile2[2]);
    }
    free(data);//�ͷ��ڴ�ռ�

    fclose(fpDestBmpfile);//�ر��ļ�ָ��
    fclose(fpSrcBmpfile);
    fclose(fpDestBmpfile1);
    for (int i = 0; i < 3; i++)
        fclose(fpDestBmpfile2[i]);
    return 0;
}


// H2 ֱ��ͼ����
// ����ͼ���ֱ��ͼ����
void calculateHistogram(unsigned char* image, int* histogram, int size) {
    for (int i = 0; i < size; ++i) {
        histogram[image[i]]++;
    }
}
//ֱ��ͼ���⻯����
void equalizeHistogram(unsigned char* image, int* histogram, int size) {
    int totalPixels = size;
    float cumulativeProbability[256] = { 0 };
    unsigned char mapping[256] = { 0 };
    // �����ۻ����ʷֲ�
    cumulativeProbability[0] = (float)histogram[0] / totalPixels;
    for (int i = 1; i < 256; ++i) {
        cumulativeProbability[i] = cumulativeProbability[i - 1] + (float)histogram[i] / totalPixels;
    }
    // ӳ���µĻҶȼ�
    for (int i = 0; i < 256; ++i) {
        mapping[i] = (unsigned char)(255 * cumulativeProbability[i] + 0.5);
    }
    // Ӧ��ӳ��
    for (int i = 0; i < totalPixels; ++i) {
        image[i] = mapping[image[i]];
    }
}
//����ֱ��ͼ
void createHistogramImage(const int histogram[256], unsigned char imageData[256][256]) {
    // �ҵ�ֱ��ͼ�е����ֵ
    int maxFrequency = 0, histogram_[256] = { 0 };
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxFrequency) {
            maxFrequency = histogram[i];
        }
    }
    //��һ��
    for (int i = 0; i < 256; ++i) {
        histogram_[i] = 255 - histogram[i] * 255 / maxFrequency;
    }
    //����ͼ������
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

    // ��ȡBMP�ļ�ͷ
    input.read((char*)&header, sizeof(header));

    // ��ȡBMPͼ����Ϣͷ
    input.read((char*)&info_header, sizeof(info_header));

    // ���BMP�ļ�ͷ��ͼ����Ϣͷ������ļ�
    output_equalized.write((char*)&header, sizeof(header));
    output_equalized.write((char*)&info_header, sizeof(info_header));

    // д���ɫ������
    for (int i = 0; i < 256; i++) {
        output_equalized.put(i);
        output_equalized.put(i);
        output_equalized.put(i);
        output_equalized.put(0);
    }

    // ֱ��ͼ����BMP�ļ�ͷ
    unsigned char bmpFileHeader[14] = {
        'B','M',  // �ļ�����
        54 + 1024, 0, 0, 0,  // �ļ���С
        54, 0, 0, 0  // ����ƫ��
    };

    // ֱ��ͼ����BMP��Ϣͷ
    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,  // ��Ϣͷ����
        0, 1, 0, 0,  // ͼ����
        0, 1, 0, 0,  // ͼ��߶�
        1, 0, 8, 0,  // ��ɫƽ������λ��
        0, 0, 0, 0,  // ��ѹ��
        0, 0, 0, 0,  // ͼ�����ݴ�С
        0, 0, 0, 0,  // ˮƽ�ֱ���
        0, 0, 0, 0,  // ��ֱ�ֱ���
        0, 0, 0, 0,  // ��ɫ������
        0, 0, 0, 0  // ��Ҫ��ɫ��
    };

    // ֱ��ͼ������ɫ��
    unsigned char palette[1024];
    for (int i = 0; i < 256; ++i) {
        palette[i * 4] = palette[i * 4 + 1] = palette[i * 4 + 2] = i;  // �Ҷ�ͼ��RGBֵ��ͬ
        palette[i * 4 + 3] = 0;
    }

    //ͼ�����ݴ洢
    unsigned char* image = new unsigned char[info_header.biWidth * info_header.biHeight];

    // ��ʼ��ֱ��ͼ
    int histogram[256] = { 0 };

    //��ʼ��ֱ��ͼ���
    unsigned char imageData[256][256];

    //��������
    input.read((char*)image, info_header.biWidth * info_header.biHeight);

    // ����\����ֱ��ͼ
    calculateHistogram(image, histogram, info_header.biWidth * info_header.biHeight);
    createHistogramImage(histogram, imageData);
    output_histogram.write(reinterpret_cast<char*>(bmpFileHeader), 14);
    output_histogram.write(reinterpret_cast<char*>(bmpInfoHeader), 40);
    output_histogram.write(reinterpret_cast<char*>(palette), 1024);
    for (int y = 255; y >= 0; --y) {
        output_histogram.write(reinterpret_cast<char*>(imageData[y]), 256);
    }

    // ִ��ֱ��ͼ���⻯
    equalizeHistogram(image, histogram, info_header.biWidth * info_header.biHeight);

    // ����\����ֱ��ͼ
    calculateHistogram(image, histogram, info_header.biWidth * info_header.biHeight);
    createHistogramImage(histogram, imageData);
    output_equalized_histogram.write(reinterpret_cast<char*>(bmpFileHeader), 14);
    output_equalized_histogram.write(reinterpret_cast<char*>(bmpInfoHeader), 40);
    output_equalized_histogram.write(reinterpret_cast<char*>(palette), 1024);
    for (int y = 255; y >= 0; --y) {
        output_equalized_histogram.write(reinterpret_cast<char*>(imageData[y]), 256);
    }

    // ���洦����ͼ������
    output_equalized.write((char*)image, info_header.biWidth * info_header.biHeight);

    // �ͷ��ڴ�
    delete[] image;

    // �ر��ļ�
    input.close();
    output_equalized.close();
    output_histogram.close();
    output_equalized_histogram.close();
}



// H3 �ռ��˲�
// ��Ĥ�������ֵ����
int getaverage(int* window, int winsize) {
    int sum = 0;
    int end = winsize * winsize;
    for (int i = 0; i < end; i++)
        sum += window[i];
    return sum / end;
}
// ��ֵ�˲�����
void mean_filter(unsigned char* inbuffer, int width, int height, unsigned char* outbuffer) {
    int mid = 0;
    int winsize = 3;
    int* window = new int[winsize * winsize];
    int pos = (winsize - 1) / 2;
    memcpy(outbuffer, inbuffer, width * height);
    for (int m = pos; m < height - pos; m++) {
        for (int n = pos; n < width - pos; n++) { // (m,n)Ϊÿһ��������λ��
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
// ������
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
// ��ֵ�˲�����
void media_filter(unsigned char* inbuffer, int width, int height, unsigned char* outbuffer) {
    int mid = 0;
    int winsize = 3;//�˲����ڴ�С
    int* windows = new int[winsize * winsize];
    int pos = (winsize - 1) / 2;
    memcpy(outbuffer, inbuffer, width * height);
    for (int m = pos; m < height - pos; m++)
    {
        for (int n = pos; n < width - pos; n++)//��m��n���Ǵ�������λ��
        {
            //��ȡwinsize*winsize�����ݽ����˲���
            int winpos = 0;
            for (int i = -pos; i < (winsize - pos); i++)
                for (int j = -pos; j < (winsize - pos); j++)
                    windows[winpos++] = inbuffer[(m + i) * width + n + j];
            //���˲����е���������ȡ��ֵ
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
    ofstream output("images//H3//median_dest.bmp", ios::binary); // ��ֵ�˲����ͼ��
    // ��дͼ���ļ�ͷ����Ϣͷ
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));

    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    // ��дͼ���ɫ��
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    output.write((char*)palette, 1024);
    //ͼ�����ݴ洢
    unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    unsigned char* outimage = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    //��������
    input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
    mean_filter(image, infoHeader.biWidth, infoHeader.biHeight, outimage);
    output.write((char*)outimage, infoHeader.biWidth * infoHeader.biHeight);

    // �ر��ļ����ͷ�new�ڴ�
    input.close();
    output.close();
    delete[] image;
    delete[] outimage;
}
void media() {
    ifstream input("images//H3//noise.bmp", ios::binary);
    ofstream output("images//H3//media_dest.bmp", ios::binary); // ��ֵ�˲����ͼ��
    // ��дͼ���ļ�ͷ����Ϣͷ
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    input.read((char*)&fileHeader, sizeof(fileHeader));
    input.read((char*)&infoHeader, sizeof(infoHeader));

    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    // ��ɫ��
    unsigned char palette[1024];
    input.read((char*)palette, 1024);
    output.write((char*)palette, 1024);
    //ͼ�����ݴ洢
    unsigned char* image = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    unsigned char* outimage = new unsigned char[infoHeader.biWidth * infoHeader.biHeight];
    //��������
    input.read((char*)image, infoHeader.biWidth * infoHeader.biHeight);
    media_filter(image, infoHeader.biWidth, infoHeader.biHeight, outimage);
    output.write((char*)outimage, infoHeader.biWidth * infoHeader.biHeight); // ���24λ���ɫͼת�Ҷ�ͼ�Ƿ���ȷ

    // �ر��ļ����ͷ�new�ڴ�
    input.close();
    output.close();
    delete[] image;
    delete[] outimage;
}




// H4 ͼ��任
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
            pre_j = width - 1 - j;
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
            pre_i = height - 1 - i;
            pre_j = j;
            if (pre_i >= 0 && pre_i < width && pre_j >= 0 && pre_j < height)
                outimage[i * width + j] = image[pre_i * width + pre_j];
            else
                outimage[i * width + j] = 0;
        }
    }
}
void rotate(unsigned char* image, unsigned char* outimage, int width, int height) { // ��תͼ��
    double degree = 30;
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
void translateImage(translateFunction translate, string fileName) {
    ifstream input("images//H4//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);

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
void bigAndSmall(float val, string fileName) { // �������Ϊ�Ŵ������С�ı���
    ifstream input("images//H4//lena.bmp", ios::binary);
    ofstream output1(fileName, ios::binary); // ͼ�����ŵ�ʵ��
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
    float ExpScalValue = val; // ���ű���
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
    output1.close();
    input.close();
    delete[] image;
    delete[] outImage;
}




// H5 ��ֵ�ָ�
typedef int (*getThreshold)(unsigned char*, int width, int height);
void translate(unsigned char* image, unsigned char* outImage, int width, int height, int thre) {
    for (int i = 0; i < width * height; i++)
        if (image[i] <= thre)
            outImage[i] = 0;
        else
            outImage[i] = 255;
}
void drawHist(unsigned char* image, int width, int height, int thre, string fileName) {
    // ֱ��ͼ����BMP�ļ�ͷ
    unsigned char bmpFileHeader[14] = {
        'B','M',  // �ļ�����
        54 + 1024, 0, 0, 0,  // �ļ���С
        54, 0, 0, 0  // ����ƫ��
    };

    // ֱ��ͼ����BMP��Ϣͷ
    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,  // ��Ϣͷ����
        0, 1, 0, 0,  // ͼ����
        0, 1, 0, 0,  // ͼ��߶�
        1, 0, 8, 0,  // ��ɫƽ������λ��
        0, 0, 0, 0,  // ��ѹ��
        0, 0, 0, 0,  // ͼ�����ݴ�С
        0, 0, 0, 0,  // ˮƽ�ֱ���
        0, 0, 0, 0,  // ��ֱ�ֱ���
        0, 0, 0, 0,  // ��ɫ������
        0, 0, 0, 0  // ��Ҫ��ɫ��
    };

    // ֱ��ͼ������ɫ��
    unsigned char palette[1024];
    for (int i = 0; i < 256; ++i) {
        palette[i * 4] = palette[i * 4 + 1] = palette[i * 4 + 2] = i;  // �Ҷ�ͼ��RGBֵ��ͬ
        palette[i * 4 + 3] = 0;
    }
    // ��ʼ��ֱ��ͼ
    int histogram[256] = { 0 };
    for (int i = 0; i < width * height; ++i) {
        histogram[image[i]]++;
    }
    // �ҵ�ֱ��ͼ�е����ֵ
    int maxFrequency = 0, histogram_[256] = { 0 };
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxFrequency) {
            maxFrequency = histogram[i];
        }
    }
    //��һ��
    for (int i = 0; i < 256; ++i) {
        histogram_[i] = 255 - histogram[i] * 255 / maxFrequency;
    }
    unsigned char imageData[256][256];
    //����ͼ������
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
    int Otsu_Threshold = 0; //��ֵ
    double variance;   //��䷽��
    double maxVariance = 0, w1 = 0, w2 = 0;
    double u1 = 0, u2 = 0;
    int size = width * height;
    //���ɻҶ�ֱ��ͼ
    double histgram[256] = { 0 };
    for (int i = 0; i < width * height; ++i) {
        histgram[image[i]]++;
    }
    for (int i = 0; i < 256; i++)
        histgram[i] /= size;
    //�����ҳ���䷽������Ӧ����ֵ
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
    // ��ʼ��ֱ��ͼ
    int histogram[256] = { 0 };
    for (int i = 0; i < width * height; ++i) {
        histogram[image[i]]++;
    }
    // ��ʼ��ֵ ͼ���лҶȼ����ֵ����Сֵȡƽ��
    int max_hist = 255, min_hist = 0;
    while (!histogram[max_hist])
        max_hist--;
    while (!histogram[min_hist])
        min_hist++;
    double old_thre = (max_hist + min_hist) * 0.5;
    double new_thre = (0 + 255) * 0.5;
    double m1, m2;
    int delta = 1; // �趨��ֹ�����仯ֵ
    while (abs(old_thre - new_thre) > delta) {
        // ������������
        int G1 = 0;
        int count_G1 = 0;
        int G2 = 0;
        int count_G2 = 0;
        int i = 0;
        // ��ֵ֮�µ�ͳ��
        for (i = 0; i <= old_thre; i++) {
            count_G1 += histogram[i];
            G1 += histogram[i] * i;
        }
        // ��ֵ֮�ϵ�ͳ��
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
    input.read((char*)image, width * height);
    unsigned char* outImage = new unsigned char[width * height];
    //�õ���ֵ
    int thre = method(image, width, height);
    // ֱ��ͼ���ƣ�����ֵ
    drawHist(image, width, height, thre, fileName);
    // �����õ���ֵ�ָ���ͼ��
    translate(image, outImage, width, height, thre);
    // ����д��
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    output.write((char*)outImage, width * height);
    output.close();
    input.close();
    delete[] image;
    delete[] outImage;
}


// H6 ����ָ�
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
            if (result.data[neighborPoint.x][neighborPoint.y] == 0 && img.data[neighborPoint.x][neighborPoint.y] > 220) {
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
void regionBaseGrowth() {
    ifstream input("images//H6//before.bmp", ios::binary);
    ofstream output("images//H6//growth.bmp", ios::binary);
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
}
// ��������ṹ��
struct Region {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
};
// ���������ƽ���Ҷ�ֵ
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
// ����ָ������ڲ����ص�һ����
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
// ������Ѻ���
void regionSplit(Image& img, const Region& region, int threshold) {
    const int winsize = 15; // ������С�ָ�ڱ߳�Ϊ8
    int width = region.bottomRightX - region.topLeftX;
    int height = region.bottomRightY - region.topLeftY;
    if (isSimilar(img, region, threshold) || (width < winsize && height < winsize)) {
        return;
    }
    // ���򣬼�������
    int midX = (region.topLeftX + region.bottomRightX) / 2;
    int midY = (region.topLeftY + region.bottomRightY) / 2;
    // ��ʶ�ָ����
    for (int i = region.topLeftX; i <= region.bottomRightX; i++) {
        img.data[midY][i] = 0;
    }
    for (int i = region.topLeftY; i <= region.bottomRightY; i++) {
        img.data[i][midX] = 0;
    }
    // ���ѳ��ĸ�������
    Region subregions[4] = {
        {region.topLeftX, region.topLeftY, midX - 1, midY - 1},
        {midX + 1, region.topLeftY, region.bottomRightX, midY - 1},
        {region.topLeftX, midY + 1, midX - 1, region.bottomRightY},
        {midX + 1, midY + 1, region.bottomRightX, region.bottomRightY}
    };

    // �ݹ��ÿ����������з���
    for (int i = 0; i < 4; ++i) {
        regionSplit(img, subregions[i], threshold);
    }
}
void regionBaseSplit() {
    ifstream input("images//H6//before.bmp", ios::binary);
    ofstream output("images//H6//split.bmp", ios::binary);
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
    // ��������
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    output.write((char*)&fileHeader, sizeof(fileHeader));
    output.write((char*)&infoHeader, sizeof(infoHeader));
    output.write((char*)palette, 1024);
    // �����ʼ����Ϊ����ͼ��
    Region initialRegion = { 0, 0, img.cols - 1, img.rows - 1 };
    // ���÷�����ֵ
    int threshold = 60;
    // ����������Ѻ�������ͼ��ָ�
    regionSplit(img, initialRegion, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(img.data[i]), width);
    }
    // �ͷ�ͼ���ڴ�
    releaseImage(img);
}



// H7 ��Ե���
// ������
int computeDifference(const Image& input, int i, int j, const int mask[3][3]) {
    int result = 0;
    for (int m = -1; m <= 1; ++m) {
        for (int n = -1; n <= 1; ++n) {
            result += input.data[i + m][j + n] * mask[m + 1][n + 1];
        }
    }
    return result;
}
// �����ݶȷ�ֵ
void computeGradient(const Image& input, Image& output, const int maskX[3][3], const int maskY[3][3], int thre) {
    for (int i = 1; i < input.rows - 1; ++i) {
        for (int j = 1; j < input.cols - 1; ++j) {
            int gradientX = abs(computeDifference(input, i, j, maskX));
            int gradientY = abs(computeDifference(input, i, j, maskY));
            // �����ݶȷ�ֵ
            int gradient = gradientX + gradientY;
            if (gradient >= thre)
                output.data[i][j] = 255;
            else
                output.data[i][j] = 0;
        }
    }
}
// ������
int computeDifference(const Image& input, int i, int j, const int mask[5][5]) {
    int result = 0;
    for (int m = -2; m <= 2; ++m) {
        for (int n = -2; n <= 2; ++n) {
            result += input.data[i + m][j + n] * mask[m + 2][n + 2];
        }
    }
    return result;
}
// �����ݶȷ�ֵ�����غ���
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
// ��Ե���������
void edgeDetection1(const int maskX[3][3], const int maskY[3][3], string fileName) {
    ifstream input("images//H7//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);
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
    // ��������
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    // �������ͼ��
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

    // ������ֵ
    int threshold = 230;
    computeGradient(img, result, maskX, maskY, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // �ͷ�ͼ���ڴ�
    releaseImage(img);
    releaseImage(result);
}
void edgeDetection2(const int mask[5][5], string fileName) {
    ifstream input("images//H7//lena.bmp", ios::binary);
    ofstream output(fileName, ios::binary);
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
    // ��������
    for (int i = 0; i < height; ++i) {
        input.read(reinterpret_cast<char*>(img.data[i]), width);
    }
    // �������ͼ��
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

    // ������ֵ
    int threshold = 230;
    computeGradient(img, result, mask, threshold);
    for (int i = 0; i < height; ++i) {
        output.write(reinterpret_cast<char*>(result.data[i]), width);
    }
    // �ͷ�ͼ���ڴ�
    releaseImage(img);
    releaseImage(result);
}


// H8 Hough�任
typedef unsigned char BYTE;
#pragma pack(1)   //ȫ����ģʽ
using namespace std;

typedef struct
{
    unsigned char bfType[2];
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} bitmapFileHeader; //λͼ�ļ�ͷ

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
} bitmapInfoHeader; //λͼ��Ϣͷ

typedef struct
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} rgbQUAD; //��ɫ��

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
    // ��ʼ��
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


    int rho;//��ԭ�����
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
            if (*(pBmpBuf2 + i * lineByte + j) == 255)//��ɫ
            {
                for (int k = 0; k < 180; k++)
                {
                    rho = int(j * cos(theta[k] * 3.14 / 180) + i * sin(theta[k] * 3.14 / 180)) % d;
                    rho = rho + d;//��ֹΪ����
                    A[rho * 180 + k]++;
                }
            }
        }
    }

    for (int i = 0; i < d * 2; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            if (A[i * 180 + j] >= 80) // �趨��ֵ����Щ���Ӧ��ֱ�߱������Ǵ��ڵ�
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
            //�ж�������ص��Ƿ���ĳ��ֱ������
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


// H9 ������
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
    fread(&bfHeader, 14, 1, fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader, 40, 1, fp);//��λͼ��Ϣͷ
    int imSize = biHeader.biSizeImage;//�ļ���С
    int width = biHeader.biWidth;
    int height = biHeader.biHeight;
    int bitCount = biHeader.biBitCount;//��ɫλ��
    int lineBytes = (width * bitCount + 31) / 32 * 4;//���ֽ���

    fseek(fp, bfHeader.bfOffBits, SEEK_SET);
    unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
    int* Flag = (int*)malloc(imSize * sizeof(int));
    memset(Flag, 0, imSize * sizeof(int)); // �������
    fread(imageData, imSize * sizeof(unsigned char), 1, fp);//��λͼ����
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
            if (imageData[lineBytes * i + j * 3] != 0)//���Ǻ�ɫ
            {
                if (i == height-1 && j == 0)//��һ�е�һ����
                {
                    Flag[lineBytes * i + j * 3] = c++;
                }
                int count = 0;
                int a[4] = { 0 };
                for (int n = i; n <= i + 1; n++)
                {
                    if (n < 0 || n >= height)//��ֹԽ��
                        continue;
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m < 0 || m >= width)//��ֹԽ��
                            continue;
                        if (n == i && m > j - 1)//ֻ�ж��ĸ����ص� �����ϡ��ϡ�����
                            continue;

                        if (Flag[lineBytes * n + m * 3] != 0)
                        {
                            Flag[lineBytes * i + j * 3] = Flag[lineBytes * n + m * 3]; // �����Ѿ�������4���������һ����ǵ����

                            a[count] = Flag[lineBytes * n + m * 3];//�����������ǵ����
                            count++;
                        }

                    }
                }
                if (count == 0)//������Ǳ���
                {
                    Flag[lineBytes * i + j * 3] = c++;
                }
                if (count == 2 && a[0] != a[1])
                {
                    equal_value[a[0]][a[1]] = equal_value[a[1]][a[0]] = 1;//���ȼ۱�
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
                    for (int m = n - 1; m >= 0; m--)//�ϲ�����,����С���
                    {
                        if (equal_value[n][m] == 1) // �����ֵ����һ�ξͼ���Ѱ�ң����Ƿ�����С��
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


// H10 ������ȡ
int contourExtraction() {
    FILE* fp;
    if ((fp = fopen("images//H10//before.bmp", "rb")) == NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader, 14, 1, fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader, 40, 1, fp);//��λͼ��Ϣͷ
    int imSize = biHeader.biSizeImage;//�ļ���С
    int width = biHeader.biWidth;
    int height = biHeader.biHeight;
    int bitCount = biHeader.biBitCount;//��ɫλ��
    int lineBytes = (width * bitCount + 31) / 32 * 4;//���ֽ���

    fseek(fp, bfHeader.bfOffBits, SEEK_SET);
    unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));
    int* Flag = (int*)malloc(imSize * sizeof(int));
    memset(Flag, 0, imSize * sizeof(int));
    fread(imageData, imSize * sizeof(unsigned char), 1, fp);//��λͼ����
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

    //�����Χ�˸���Ϊ��/�ڲ�����Ϊ�ڲ���
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (b.imgData[lineBytes * i + j * 3] == 0) //����Ǻ�ɫ
            {
                int count = 0;
                for (int n = i - 1; n <= i + 1; n++)
                {
                    if (n < 0 || n >= height)//��ֹԽ��
                    {
                        count++;
                        continue;
                    }
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m < 0 || m >= width)//��ֹԽ��
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
    // ����BMP�ļ��Ĵ���
    RgbToGray();
    cout << "ִ��BMP�ļ�����Ĵ���..." << endl;
}

void processHistogram() {
    // ֱ��ͼ����Ĵ���
    histogram();
    cout << "ִ��ֱ��ͼ����Ĵ���..." << endl;
}

void processSpatialFiltering() {
    // �ռ��˲��Ĵ���
    median();
    media();
    cout << "ִ�пռ��˲��Ĵ���..." << endl;
}

void processImageTransformation() {
    // ͼ��任�Ĵ���
    bigAndSmall(2, "images//H4//big.bmp"); // �Ŵ�����
    bigAndSmall(0.5, "images//H4//small.bmp");
    translateImage(move, "images//H4//move.bmp");
    translateImage(horizontalMirror, "images//H4//Hmirror.bmp");
    translateImage(verticalMirror, "images//H4//Vmirror.bmp");
    translateImage(rotate, "images//H4//rotate.bmp");
    cout << "ִ��ͼ��任�Ĵ���..." << endl;
}

void processThresholdSegmentation() {
    // ��ֵ�ָ�Ĵ���
    ThresholdSegmentation(givenMethod, "images//H5//threSegment1.bmp");
    ThresholdSegmentation(iterativeMethod, "images//H5//threSegment2.bmp");
    ThresholdSegmentation(otsuMethod, "images//H5//threSegment3.bmp");
    cout << "ִ����ֵ�ָ�Ĵ���..." << endl;
}

void processRegionBasedSegmentation() {
    // ��������ָ�Ĵ���
    regionBaseGrowth();
    regionBaseSplit();
    cout << "ִ�л�������ָ�Ĵ���..." << endl;
}

void processEdgeDetection() {
    // ��Ե���Ĵ���
    // ����Sobel����
    const int sobelMaskX[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    const int sobelMaskY[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    // ����Prewitt����
    const int prewittMaskX[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    const int prewittMaskY[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    // ����log����
    const int log[5][5] = { {0,0,-1,0,0}, {0, -1, -2, -1, 0}, {-1, -2, 16, -2, -1}, {0, -1, -2, -1, 0}, {0,0,-1,0,0} };
    edgeDetection1(sobelMaskX, sobelMaskY, "images//H7//sobel.bmp");
    edgeDetection1(prewittMaskX, prewittMaskY, "images//H7//prewitte.bmp");
    edgeDetection2(log, "images//H7//log.bmp");
    cout << "ִ�б�Ե���Ĵ���..." << endl;
}

void processHoughTransform() {
    // Hough�任�Ĵ���
    houghTransform();
    cout << "ִ��Hough�任�Ĵ���..." << endl;
}

void processRegionLabeling() {
    // �����ǵĴ���
    regionLabeling();
    cout << "ִ�������ǵĴ���..." << endl;
}

void processContourExtraction() {
    // ������ȡ�Ĵ���
    contourExtraction();
    cout << "ִ��������ȡ�Ĵ���..." << endl;
}

void processAllFunctions() {
    // ʵ�����й��ܵ�������
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
    cout << "ִ�����й���������Ĵ���..." << endl;
}

int main() {
    int userInput;

    do {
        // ��ʾ�˵�
        cout << "===== ����ͼ����ϵͳ�˵� =====" << endl;
        cout << "  0. ���й���������" << endl;
        cout << "  1. BMP�ļ�����" << endl;
        cout << "  2. ֱ��ͼ����" << endl;
        cout << "  3. �ռ��˲�" << endl;
        cout << "  4. ͼ��任" << endl;
        cout << "  5. ��ֵ�ָ�" << endl;
        cout << "  6. ��������ָ�" << endl;
        cout << "  7. ��Ե���" << endl;
        cout << "  8. Hough�任" << endl;
        cout << "  9. ������" << endl;
        cout << " 10. ������ȡ" << endl;
        cout << "=============================" << endl;
        cout << "��������Ӧ�����ֽ���ѡ�� (����-1�˳�): ";
        cin >> userInput;

        // �����û�����ִ����Ӧ�Ĺ���
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
            cout << "�˳�����" << endl;
            break;
        default:
            cout << "��Ч��ѡ�����������롣" << endl;
        }
        // ����
        system("pause");
        system("cls");

    } while (userInput != -1);

    return 0;
}
