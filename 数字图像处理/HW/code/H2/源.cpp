#include <iostream>
#include <fstream>

#define WORD short
#define DWORD int
#define LONG long

#pragma pack(2)

// BMP�ļ�ͷ�ṹ��
typedef struct tagBITMAPFILEHEADER {
    WORD   	bfType; //�ļ����ͣ������ǡ�BM��
    DWORD	bfSize; //�ļ���С�������ļ�ͷ�Ĵ�С
    WORD	bfReserved1; //�����֣���������Ϊ0
    WORD	bfReserved2; //�����֣���������Ϊ0
    DWORD	bfOffBits; //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BITMAPFILEHEADER;

// BMPͼ����Ϣͷ�ṹ��
typedef struct tagBITMAPINFOHEADER {
    DWORD   biSize; //�ýṹ�ĳ��ȣ�Ϊ40
    LONG	    biWidth; //ͼ����
    LONG	    biHeight; //ͼ��߶�
    WORD	    biPlanes; //λƽ����������Ϊ1
    WORD	    biBitCount; //��ɫλ��
    DWORD   biCompression; //�Ƿ�ѹ��,��0��ʾ��ѹ��
    DWORD   biSizeImage; //ʵ��λͼ����ռ�õ��ֽ���
    LONG	    biXPelsPerMeter;//Ŀ���豸ˮƽ�ֱ���,����ÿ��
    LONG	    biYPelsPerMeter;//Ŀ���豸��ֱ�ֱ���,����ÿ��
    DWORD   biClrUsed;//ʵ��ʹ�õ���ɫ��
    DWORD   biClrImportant;//ͼ������Ҫ����ɫ��
}BITMAPINFOHEADER;

// ����ͼ���ֱ��ͼ����
void calculateHistogram(unsigned char* image, int* histogram,int size) {
    for (int i = 0; i < size; ++i) {
        histogram[image[i]]++;
    }
}

//ֱ��ͼ���⻯����
void equalizeHistogram(unsigned char* image, int* histogram,int size) {
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

int main() {
    std::ifstream input("lena.bmp", std::ios::binary); 
    std::ofstream output_histogram("src_hist.bmp", std::ios::binary);
    std::ofstream output_equalized_histogram("dest_hist.bmp", std::ios::binary);
    std::ofstream output_equalized("dest.bmp", std::ios::binary);

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
    output_histogram.write(reinterpret_cast<char*>(palette),1024);
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

    return 0;
}
