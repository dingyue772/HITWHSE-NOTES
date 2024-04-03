#pragma once
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

# pragma pop(2)

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
