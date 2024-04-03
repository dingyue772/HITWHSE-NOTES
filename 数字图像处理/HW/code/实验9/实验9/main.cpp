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
    unsigned char *imgData;
} bmp;

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
    int c=0;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*b.biHeader.biSizeImage);
    memset(b.imgData,0,sizeof(unsigned char)*b.biHeader.biSizeImage);
    int equal_value[240][240]= {0};
    char*savePath="after.bmp";

    for(int i=height-1; i>=0; i--)
    {
        for(int j=0; j<width; j++)
        {
            if(imageData[lineBytes*i+j*3]!=0)//���Ǻ�ɫ
            {
                if(i==height&&j==0)//��һ�е�һ����
                {
                    Flag[lineBytes*i+j*3]=c++;
                }
                int count=0;
                int a[2]= {0};
                for(int n=i; n<=i+1; n++)
                {
                    if(n<0||n>=height)//��ֹԽ��
                        continue;
                    for(int m=j-1; m<=j+1; m++)
                    {
                        if(m<0||m>=width)//��ֹԽ��
                            continue;
                        if(n==i&&m>j-1)//ֻ�ж��ĸ����ص�
                            continue;

                        if(Flag[lineBytes*n+m*3]!=0)
                        {
                            Flag[lineBytes*i+j*3]=Flag[lineBytes*n+m*3];

                            a[count]=Flag[lineBytes*n+m*3];//�����������ǵ����
                            count++;
                        }

                    }
                }
                if(count==0)//������Ǳ���
                {
                    Flag[lineBytes*i+j*3]=c++;
                }
                if(count==2 && a[0]!=a[1] )
                {
                    equal_value[a[0]][a[1]]=equal_value[a[1]][a[0]]=1;//���ȼ۱�
                }
            }
        }
    }
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(Flag[lineBytes*i+j*3]!=0)
            {
                bool flag=true;
                while(flag)
                {
                    flag=false;
                    int n=Flag[lineBytes*i+j*3];
                    for(int m=n-1; m >= 0; m--)//�ϲ�����,����С���
                    {
                        if(equal_value[n][m]==1)
                        {
                            Flag[lineBytes*i+j*3]=m;
                            flag=true;
                        }
                    }
                }
                b.imgData[lineBytes*i+j*3]=Flag[lineBytes*i+j*3];
                b.imgData[lineBytes*i+j*3+1]=Flag[lineBytes*i+j*3];
                b.imgData[lineBytes*i+j*3+2]=Flag[lineBytes*i+j*3];

                int k=Flag[lineBytes*i+j*3]%3;//0-2
                b.imgData[lineBytes*i+j*3+k]=255;
            }
        }
    }

    write(savePath,b);
    free(imageData);
    free(b.imgData);
    return 0;
}

