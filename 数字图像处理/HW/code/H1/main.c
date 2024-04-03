#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#include "bmp.h"

//ȫ�ֱ�������

FILE * fpSrcBmpfile;

FILE * fpDestBmpfile; // ת���õ���8λ�Ҷ�ͼ���

FILE * fpDestBmpfile1; // �Ҷ�ͼ��ɫ���

FILE * fpDestBmpfile2[3]; //  ͨ������õ���3���Ҷ�ͼ 

void GetBmpHeader(PBITMAPFILEHEADER, PBITMAPINFOHEADER); // ���λͼ���ļ�ͷ����Ϣͷ 

void ChangeBmpHeader(PBITMAPFILEHEADER, PBITMAPINFOHEADER, WORD); // ת��ͼ��ʱ�ĸı��ļ�ͷ����Ϣͷ����

void SetBmpHeader(const PBITMAPFILEHEADER, const PBITMAPINFOHEADER); 

void SetRGBQUAD(); // �Ҷ�ͼ��ɫ��Ľ���

int RgbToGray(); // ͼ��ת������ 

int main(int argc, char *argv[]) {
	RgbToGray();
	return 0;
}
// ʹ��C ��׼���е��ļ���ȡ���� fread()���Ӵ򿪵�λͼ�ļ��ж�ȡλͼ�ļ�ͷ��λͼ��Ϣͷ
// ����fread()������ 
// param1����һ��ָ�����ȡ���ݵĴ洢�����ָ�룬param2����ÿ��������Ĵ�С���ֽ�������param3���� Ҫ��ȡ���������������param4���� �ļ�ָ�룬ָ��Ҫ��ȡ���ļ�
void GetBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader){
	fread(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpSrcBmpfile);
	fread(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpSrcBmpfile);
}

// ��һ��24λ���ɫλͼ�ļ�תΪһ��8Ϊ�Ҷ�λͼ����Ҫ���ļ�ͷ����Ϣͷ����Щ�ı�
// 1. ��Ϣͷ�е�biBitCount��λͼ��λ��� 
// 2. ��Ϣͷ�е�biClrUsed�� 
// 3. ��Ϣͷ�е�biSizeImage
// 4. �ļ�ͷ�� bfOffBits��54+��ɫ��
// 5. �ļ�ͷ�� bfSize��λͼ��С 
void ChangeBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader, WORD wType){
	pbiheader->biBitCount = wType; // 8λ�Ҷ�ͼ�轫��ֵ��Ϊ8 
	pbiheader->biClrUsed = (wType == 24)? 0: 256; // �Ҷ�ͼ��Ҫʹ��256ɫ��ɫ�壬������Ϊ256 
	pbfheader->bfOffBits = 54+ pbiheader->biClrUsed* sizeof(RGBQUAD); // bfOffBitsָ���Ǵ�λͼͷ���������ֽڿ���ֱ�ӵ���λͼ���ݲ��� 
	// ��һ���й���biSizeImage�ļ��㣬ͨ��biWidth*biBitCount�õ�������λ������LineBytes��&~31�Ĳ���ʹ��LineBytes�����5λ����Ϊ0��ȷ��ÿ�е�λ����Ϊ32�ı���
	// /8��Ϊ�˵õ��ֽ�����biSizeImage=LineBytes*biHeight 
	pbiheader->biSizeImage = ((((pbiheader->biWidth * pbiheader->biBitCount) + 31) & ~31) /8) * pbiheader->biHeight;
	// һ��λͼ�Ĵ�С���㣬bfOffBitsָ����λͼͷ����λͼ�ļ�ͷ��λͼ��Ϣͷ�Ϳ�ѡ�ĵ�ɫ���С֮�ͣ���Ϣͷ�е�biSizeImageָ������λͼ���ݴ�С������֮�ͼ�Ϊλͼ�ܴ�С�����ļ�ͷ�е�bfSize 
	pbfheader->bfSize = pbfheader->bfOffBits + pbiheader->biSizeImage; 
}

void SetBmpHeader(const PBITMAPFILEHEADER pbfheader, const PBITMAPINFOHEADER pbiheader){
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

void SetRGBQUAD(){
	int i;
	RGBQUAD rgbquad[256];
	// Ϊ��ɫ����256����ɫ��������Ӧ��RGBֵ��ͨ������ÿ����ɫ��RGB��������ͬ�õ�����һ���Ҷ�ͼ���ɫ�� 
	for (i=0; i<256; i++){
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	// ����ɫ��д��λͼ�ļ��� 
	fwrite(rgbquad, 256*sizeof(RGBQUAD), 1, fpDestBmpfile);
	
	fwrite(rgbquad, 256*sizeof(RGBQUAD), 1, fpDestBmpfile1);
	
	fwrite(rgbquad, 256*sizeof(RGBQUAD), 1, fpDestBmpfile2[0]);
	fwrite(rgbquad, 256*sizeof(RGBQUAD), 1, fpDestBmpfile2[1]);
	fwrite(rgbquad, 256*sizeof(RGBQUAD), 1, fpDestBmpfile2[2]);
}

int RgbToGray()
{
    LONG w,h;
    BYTE r,g,b;
    BYTE gray, xgray;
    //  ȷ��ͼ����������ݴ洢���ļ��е�ÿһ�ж�����4�ı�������
    BYTE count24,count8; // count24Ϊ24λ���ɫͼ��ÿ�е�����ֽ����� count8Ϊ8λ�Ҷ�ͼ��ÿ�е�����ֽ�����Ϊ�˱�֤ͼ���ÿ���ֽ���Ϊ4�ı��� 
    BYTE Bmpnul=0; // һ���ֽڣ��������8λ�Ҷ�ͼ��ÿ�е�����ֽ� 

    char SrcBmpfile[256];
    char DestBmpfile[256];
    char DestBmpfile1[256];
	char DestBmpfile2[3][256];
    
    BITMAPFILEHEADER bmfh; // bmp�ļ�ͷ

    BITMAPINFOHEADER bmih; // λͼ��Ϣͷ

    BYTE *data;

    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));//�ڴ��ʼ��

    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
    data=(BYTE *)malloc(3*sizeof(BYTE));
    if(!data)
    {
       printf("Error:Can not allocate memory .\n");
       free(data);
       return -1;
    }

    printf("Input the path of SrcBmpfile:\n");
    gets(SrcBmpfile);
//	printf("----------------------------------");
    if((fpSrcBmpfile=fopen(SrcBmpfile,"rb"))==NULL) 
    {
        printf("Error:Open the file of SrcBmpfile failed!\n");//����Դλͼ�ļ�

        free(data);
        return -1;
    }
    
    rewind(fpSrcBmpfile); // ʹ��rewind()�������ļ�ָ�����õ��ļ���ͷ 
    GetBmpHeader(&bmfh,&bmih);
//ceshie_start

//    printf("The contents in the file header of the BMP file:\n");
//    printf("bfType:%ld\n",bmfh.bfType); // 19778
//    printf("bfSize:%ld\n",bmfh.bfSize); // 687054
//    printf("bfReserved1:%ld\n",bmfh.bfReserved1); // 0
//    printf("bfReserved2:%ld\n",bmfh.bfReserved2); // 0
//    printf("bfOffBits:%ld\n",bmfh.bfOffBits); // 54
//    printf("The contents in the info header:\n"); 
//    printf("biSize:%ld\n",bmih.biSizeImage); // 687000
//    printf("bmih.biBitCount:%ld\n", bmih.biBitCount);
//ceshi_end
	// �ж�Դλͼ�Ƿ�Ϊbmp��ʽ 
    if(bmfh.bfType!=0x4D42)
    {
    	printf("bmfh.bfType: ");
    	printf(bmfh.bfType);
        printf("Error:This file is not bitmap file!\n");
        free(data);
        return -1;
    }
    // �ж�Դλͼ�Ƿ�Ϊ24λ���ɫͼ 
    if(bmih.biBitCount!=24)
    {
    	printf("bmih.biBitCount: " );
    	printf(bmih.biBitCount);
        printf("Error:This bmpfile is not 24bit bitmap!\n");
        free(data);
        return -1;
    }
    // �ж�Դλͼ�Ƿ���ѹ�� 
    if(bmih.biCompression!=0)
    {
    	printf("bmih.biCompression: ");
    	printf(bmih.biCompression);
        printf("Error:This 8bit bitmap file is not BI_RGB type!\n");
        free(data);
        return -1;
    }

    printf("Input the path of the DestBmpfile:\n");//����Ŀ��λͼ�ļ�

    gets(DestBmpfile);

    if((fpDestBmpfile=fopen(DestBmpfile,"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }
	
	
	printf("Input the path of the DestBmpfile1:\n");//����Ŀ��λͼ�ļ�

    gets(DestBmpfile1);

    if((fpDestBmpfile1=fopen(DestBmpfile1,"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile1 failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile2:\n");//����Ŀ��λͼ�ļ�

    gets(DestBmpfile2[0]);

    if((fpDestBmpfile2[0]=fopen(DestBmpfile2[0],"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile2 failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile3:\n");//����Ŀ��λͼ�ļ�

    gets(DestBmpfile2[1]);

    if((fpDestBmpfile2[1]=fopen(DestBmpfile2[1],"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile4:\n");//����Ŀ��λͼ�ļ�

    gets(DestBmpfile2[2]);

    if((fpDestBmpfile2[2]=fopen(DestBmpfile2[2],"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }
    
    
    ChangeBmpHeader(&bmfh,&bmih,8);
    SetBmpHeader(&bmfh,&bmih);
    SetRGBQUAD();
//	printf("----------------------�ı�λͼͷ����Ϣ��--------------------------");
//	printf("The contents in the file header of the BMP file:\n");
//    printf("bfType:%ld\n",bmfh.bfType); // 19778
//    printf("bfSize:%ld\n",bmfh.bfSize); // 230078
//    printf("bfReserved1:%ld\n",bmfh.bfReserved1); // 0
//    printf("bfReserved2:%ld\n",bmfh.bfReserved2); // 0
//    printf("bfOffBits:%ld\n",bmfh.bfOffBits); // 1078=54+1024=54+256*4 
//    printf("The contents in the info header:\n"); 
//    printf("biSize:%ld\n",bmih.biSizeImage); // 229000
//	printf("-------------------------------------------------------------------");
    // ÿһ��Ϊ�����ֽڶ��룬����Ҫ������ֽ��� 
	count24=(4-(bmih.biWidth*3)%4)%4;
    count8=(4-(bmih.biWidth)%4)%4;

// ��ͼ��ײ��򶥲�����ÿһ�����ص� 
    for(h=bmih.biHeight-1;h>=0;h--)
    {
        for(w=0;w<bmih.biWidth;w++)
        {
            fread(data,3,1,fpSrcBmpfile);
            if(feof(fpSrcBmpfile))
            {
                printf("Error:Read Pixel data failed!\n");
                free(data);
                return -1;
            }
            b=*data;
            g=*(data+1);
            r=*(data+2);
            gray=(299*r+587*g+114*b)/1000;
            //if(gray>120)gray=250;
            // ��ɫ�Ҷ�ͼ�����ص�ֵ
			xgray = 255 - gray; 

            fwrite(&gray,sizeof(gray),1,fpDestBmpfile);
            fwrite(&xgray,sizeof(xgray),1,fpDestBmpfile1);
            
            fwrite(&b,sizeof(b),1,fpDestBmpfile2[0]);
            fwrite(&g,sizeof(g),1,fpDestBmpfile2[1]);
            fwrite(&r,sizeof(r),1,fpDestBmpfile2[2]);
        }
        fseek(fpSrcBmpfile,count24,SEEK_CUR); // ��Դλͼ�ļ�ָ������ƶ�count24���ֽ� 
        fwrite(&Bmpnul,1,count8,fpDestBmpfile);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile1);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[0]);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[1]);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[2]);
    }
    printf("Hint:Convert RGB To GRAY Successfully!\n");
    free(data);//�ͷ��ڴ�ռ�

    
    fclose(fpDestBmpfile);//�ر��ļ�ָ��

    fclose(fpSrcBmpfile);
    return 0;
}
