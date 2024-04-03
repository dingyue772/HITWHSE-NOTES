#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <malloc.h>
#include <math.h>
#include <vector>
#include <stack>
#include<algorithm>

typedef unsigned char BYTE;
#pragma pack(1)   //全紧凑模式
using namespace std;

typedef struct
{
    unsigned char bfType[2];
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} bitmapFileHeader; //位图文件头

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
} bitmapInfoHeader; //位图信息头

typedef struct
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} rgbQUAD; //调色板

typedef struct
{
    bitmapFileHeader bfHeader;
    bitmapInfoHeader biHeader;
    //rgbQUAD palette[256];
    unsigned char *imgData;
} bmp;

int main()
{

    FILE* fp = fopen("8-2.bmp", "rb");
    if (fp == 0)
        return 0;
    bitmapFileHeader fileHead;
    fread(&fileHead,sizeof(bitmapFileHeader), 1, fp);

    bitmapInfoHeader infoHead;
    fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);
    int bmpsize= infoHead.biSizeImage;
    int width = infoHead.biWidth;
    int height = infoHead.biHeight;
    int biCount = infoHead.biBitCount;
    int lineByte = (biCount*width / 8 + 3) / 4 * 4;
    rgbQUAD* pColorTable;
    pColorTable = new rgbQUAD[256];
    fread(pColorTable, sizeof(rgbQUAD), 256, fp);

    unsigned char* pBmpBuf;
    pBmpBuf = new unsigned char[lineByte*height];
    fread(pBmpBuf, lineByte*height, 1, fp);
    fclose(fp);
    char *savePath;


    unsigned char *pBmpBuf2;
    // 初始化
    pBmpBuf2 = new unsigned char[lineByte*height];

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {

            *(pBmpBuf2 + i*lineByte + j) = *(pBmpBuf + i*lineByte + j);
        }
    }

    savePath="after.bmp";
    FILE* fop = fopen(savePath, "wb");
    if (fop == 0)
        return 0;
    fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
    fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);


    int rho;//到原点距离
    int d=sqrt(width*width+height*height);
    int platoon_bit=bmpsize/height-width*(biCount/8);
    int* A=new int[d * 2 *180];//
    int* theta=new int[180];//
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


    for(int i=1; i<height; i++)
    {
        for(int j=1; j<width; j++)
        {
            if(*(pBmpBuf2+i*lineByte+j) == 255)//白色
            {
                for(int k=0; k<180; k++)
                {
                    rho=int(j*cos(theta[k] * 3.14 /180)+i*sin(theta[k] * 3.14/180))%d;
                    rho=rho+d;//防止为负数
                    A[rho *180 +k]++;
                }
            }
        }
    }

    for(int i=0; i<d*2; i++)
    {
        for(int j=0; j<180; j++)
        {
            if (A[i * 180 + j] >= 80)
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
            //判断这个像素点是否在某条直线上面
            for (int i = 0; i < lineAIndexLength; i++)
            {
                int t = lineAIndexArray[i] % 180;//theta
                int r = lineAIndexArray[i] /180 - d;//rho

                //xcos(a)+ysin(a)=r
                if ((abs(column * cos(t * 3.14/180) + row * sin(t * 3.14/180) - r) <= 0.0001) )
                    gbquadIndex = 255;

                if(t==90&&row==r)
                    gbquadIndex = 255;

            }
            fwrite(&gbquadIndex, sizeof(BYTE), 1, fop);
        }
        BYTE zero = 0;
        fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, fop);
    }

    fclose(fop);

    return 0;
}
