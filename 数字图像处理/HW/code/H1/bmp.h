/* C���Զ���ͼ�� λͼ�ļ��ṹ���� */
#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

typedef unsigned short WORD;//2*8=16
typedef unsigned long DWORD;//4*8=32
typedef long LONG;//4*8=32
typedef unsigned char BYTE;//1*8=8

// #pragma pack(1) ָʾ����������1�ֽڵĶ��뷽ʽ���洢�ṹ���Ա
#pragma pack(1) 
// ����λͼ�ļ�ͷ�ṹ�� 14���ֽ� 
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;
// #pragma pop()�������ڻָ�Ĭ�ϵĶ��뷽ʽ 
# pragma pop() 

// λͼ��Ϣͷ�ṹ�� 40���ֽ� 
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

 // ��ɫ�����ݽṹ 4���ֽ� 
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
 }RGBQUAD;

#endif
 


