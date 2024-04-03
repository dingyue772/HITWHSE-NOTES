/* C语言读入图像 位图文件结构声明 */
#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

typedef unsigned short WORD;//2*8=16
typedef unsigned long DWORD;//4*8=32
typedef long LONG;//4*8=32
typedef unsigned char BYTE;//1*8=8

// #pragma pack(1) 指示编译器按照1字节的对齐方式来存储结构体成员
#pragma pack(1) 
// 定义位图文件头结构体 14个字节 
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;
// #pragma pop()函数用于恢复默认的对齐方式 
# pragma pop() 

// 位图信息头结构体 40个字节 
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSzie;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
 }BITMAPINFOHEADER, *PBITMAPINFOHEADER;

 // 调色板数据结构 4个字节 
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
 }RGBQUAD;

#endif
 


