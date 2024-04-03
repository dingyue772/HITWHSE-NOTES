#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<fstream>
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
    unsigned char *imgData;
} bmp;

//д���ļ�
int write(char savePath[],bmp b)
{
    FILE *f=fopen(savePath,"wb");
    if(f==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f);
    fclose(f);

}

int main()
{
    FILE *fp;
    if((fp=fopen("before.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);//��λͼ��Ϣͷ
    int imSize=biHeader.biSizeImage;//�ļ���С
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;//��ɫλ��
    int lineBytes=(width*bitCount+31)/32*4;//���ֽ���

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    int *Flag=(int*)malloc(imSize*sizeof(int));
    memset(Flag,0,imSize*sizeof(int));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);//��λͼ����
    fclose(fp);

    bmp b;

    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*b.biHeader.biSizeImage);
    memset(b.imgData,0,sizeof(unsigned char)*b.biHeader.biSizeImage);
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes*i+j*3]=imageData[lineBytes*i+j*3];
            b.imgData[lineBytes*i+j*3+1]=imageData[lineBytes*i+j*3+1];
            b.imgData[lineBytes*i+j*3+2]=imageData[lineBytes*i+j*3+2];
        }
    }

    char*savePath="after.bmp";

//�����Χ�˸���Ϊ��/�ڲ�����Ϊ�ڲ���
    for(int i=0; i < height; i++)
    {
        for(int j=0; j < width; j++)
        {
            if(b.imgData[lineBytes*i+j*3]==0) //����Ǻ�ɫ
            {
                int count=0;
                for(int n=i-1; n<=i+1; n++)
                {
                    if(n<0||n>=height)//��ֹԽ��
                    {
                        count++;
                        continue;
                    }
                    for(int m=j-1; m<=j+1; m++)
                    {
                        if(m<0||m>=width)//��ֹԽ��
                        {
                            count++;
                            continue;
                        }
                        if(b.imgData[lineBytes*n+m*3]==0||Flag[lineBytes*n+m*3]==1)
                        {
                            count++;
                        }
                    }
                }
                if(count==9)
                {
                    Flag[lineBytes*i+j*3]=1;
                    b.imgData[lineBytes*i+j*3]=255;
                    b.imgData[lineBytes*i+j*3+1]=255;
                    b.imgData[lineBytes*i+j*3+2]=255;
                }
            }
        }
    }

    write(savePath,b);

    free(imageData);
    free(b.imgData);
    return 0;
}

