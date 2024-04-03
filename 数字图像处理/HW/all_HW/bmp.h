#pragma once
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
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;

# pragma pop(2)

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
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
}RGBQUAD, * PRGBQUAD;