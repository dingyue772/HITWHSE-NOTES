#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#include "bmp.h"

//全局变量声明

FILE * fpSrcBmpfile;

FILE * fpDestBmpfile; // 转换得到的8位灰度图结果

FILE * fpDestBmpfile1; // 灰度图反色结果

FILE * fpDestBmpfile2[3]; //  通道分离得到的3个灰度图 

void GetBmpHeader(PBITMAPFILEHEADER, PBITMAPINFOHEADER); // 获得位图的文件头和信息头 

void ChangeBmpHeader(PBITMAPFILEHEADER, PBITMAPINFOHEADER, WORD); // 转换图像时的改变文件头和信息头函数

void SetBmpHeader(const PBITMAPFILEHEADER, const PBITMAPINFOHEADER); 

void SetRGBQUAD(); // 灰度图调色板的建立

int RgbToGray(); // 图像转换函数 

int main(int argc, char *argv[]) {
	RgbToGray();
	return 0;
}
// 使用C 标准库中的文件读取函数 fread()，从打开的位图文件中读取位图文件头和位图信息头
// 关于fread()函数： 
// param1——一个指向待读取内容的存储区域的指针，param2——每个数据项的大小（字节数），param3—— 要读取的数据项的数量，param4—— 文件指针，指向要读取的文件
void GetBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader){
	fread(pbfheader, sizeof(BITMAPFILEHEADER), 1, fpSrcBmpfile);
	fread(pbiheader, sizeof(BITMAPINFOHEADER), 1, fpSrcBmpfile);
}

// 将一个24位真彩色位图文件转为一个8为灰度位图，需要在文件头和信息头做这些改变
// 1. 信息头中的biBitCount，位图的位深度 
// 2. 信息头中的biClrUsed， 
// 3. 信息头中的biSizeImage
// 4. 文件头中 bfOffBits，54+调色板
// 5. 文件头中 bfSize，位图大小 
void ChangeBmpHeader(PBITMAPFILEHEADER pbfheader, PBITMAPINFOHEADER pbiheader, WORD wType){
	pbiheader->biBitCount = wType; // 8位灰度图需将其值设为8 
	pbiheader->biClrUsed = (wType == 24)? 0: 256; // 灰度图需要使用256色调色板，需设置为256 
	pbfheader->bfOffBits = 54+ pbiheader->biClrUsed* sizeof(RGBQUAD); // bfOffBits指的是从位图头跳过多少字节可以直接到达位图数据部分 
	// 这一步中关于biSizeImage的计算，通过biWidth*biBitCount得到的是行位数，即LineBytes，&~31的操作使得LineBytes的最低5位均设为0，确保每行的位数都为32的倍数
	// /8是为了得到字节数，biSizeImage=LineBytes*biHeight 
	pbiheader->biSizeImage = ((((pbiheader->biWidth * pbiheader->biBitCount) + 31) & ~31) /8) * pbiheader->biHeight;
	// 一个位图的大小计算，bfOffBits指的是位图头包括位图文件头、位图信息头和可选的调色板大小之和，信息头中的biSizeImage指的则是位图数据大小，二者之和即为位图总大小，即文件头中的bfSize 
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
	// 为调色板中256个颜色都设置相应的RGB值，通过设置每个颜色的RGB三分量相同得到的是一个灰度图像调色板 
	for (i=0; i<256; i++){
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	// 将调色板写入位图文件中 
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
    //  确保图像的像素数据存储在文件中的每一行都具有4的倍数长度
    BYTE count24,count8; // count24为24位真彩色图像每行的填充字节数， count8为8位灰度图像每行的填充字节数；为了保证图像的每行字节数为4的倍数 
    BYTE Bmpnul=0; // 一个字节，用于填充8位灰度图像每行的填充字节 

    char SrcBmpfile[256];
    char DestBmpfile[256];
    char DestBmpfile1[256];
	char DestBmpfile2[3][256];
    
    BITMAPFILEHEADER bmfh; // bmp文件头

    BITMAPINFOHEADER bmih; // 位图信息头

    BYTE *data;

    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));//内存初始化

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
        printf("Error:Open the file of SrcBmpfile failed!\n");//输入源位图文件

        free(data);
        return -1;
    }
    
    rewind(fpSrcBmpfile); // 使用rewind()函数将文件指针设置到文件开头 
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
	// 判断源位图是否为bmp格式 
    if(bmfh.bfType!=0x4D42)
    {
    	printf("bmfh.bfType: ");
    	printf(bmfh.bfType);
        printf("Error:This file is not bitmap file!\n");
        free(data);
        return -1;
    }
    // 判断源位图是否为24位真彩色图 
    if(bmih.biBitCount!=24)
    {
    	printf("bmih.biBitCount: " );
    	printf(bmih.biBitCount);
        printf("Error:This bmpfile is not 24bit bitmap!\n");
        free(data);
        return -1;
    }
    // 判断源位图是否无压缩 
    if(bmih.biCompression!=0)
    {
    	printf("bmih.biCompression: ");
    	printf(bmih.biCompression);
        printf("Error:This 8bit bitmap file is not BI_RGB type!\n");
        free(data);
        return -1;
    }

    printf("Input the path of the DestBmpfile:\n");//输入目标位图文件

    gets(DestBmpfile);

    if((fpDestBmpfile=fopen(DestBmpfile,"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }
	
	
	printf("Input the path of the DestBmpfile1:\n");//输入目标位图文件

    gets(DestBmpfile1);

    if((fpDestBmpfile1=fopen(DestBmpfile1,"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile1 failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile2:\n");//输入目标位图文件

    gets(DestBmpfile2[0]);

    if((fpDestBmpfile2[0]=fopen(DestBmpfile2[0],"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile2 failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile3:\n");//输入目标位图文件

    gets(DestBmpfile2[1]);

    if((fpDestBmpfile2[1]=fopen(DestBmpfile2[1],"wb"))==NULL)
    {
        printf("Error:Open the file of DestBmpfile failed!\n");
        free(data);
        return -1;
    }
    
    
    printf("Input the path of the DestBmpfile4:\n");//输入目标位图文件

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
//	printf("----------------------改变位图头部信息后--------------------------");
//	printf("The contents in the file header of the BMP file:\n");
//    printf("bfType:%ld\n",bmfh.bfType); // 19778
//    printf("bfSize:%ld\n",bmfh.bfSize); // 230078
//    printf("bfReserved1:%ld\n",bmfh.bfReserved1); // 0
//    printf("bfReserved2:%ld\n",bmfh.bfReserved2); // 0
//    printf("bfOffBits:%ld\n",bmfh.bfOffBits); // 1078=54+1024=54+256*4 
//    printf("The contents in the info header:\n"); 
//    printf("biSize:%ld\n",bmih.biSizeImage); // 229000
//	printf("-------------------------------------------------------------------");
    // 每一行为了四字节对齐，而需要补齐的字节数 
	count24=(4-(bmih.biWidth*3)%4)%4;
    count8=(4-(bmih.biWidth)%4)%4;

// 从图像底部向顶部遍历每一行像素点 
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
            // 反色灰度图的像素点值
			xgray = 255 - gray; 

            fwrite(&gray,sizeof(gray),1,fpDestBmpfile);
            fwrite(&xgray,sizeof(xgray),1,fpDestBmpfile1);
            
            fwrite(&b,sizeof(b),1,fpDestBmpfile2[0]);
            fwrite(&g,sizeof(g),1,fpDestBmpfile2[1]);
            fwrite(&r,sizeof(r),1,fpDestBmpfile2[2]);
        }
        fseek(fpSrcBmpfile,count24,SEEK_CUR); // 将源位图文件指针向后移动count24个字节 
        fwrite(&Bmpnul,1,count8,fpDestBmpfile);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile1);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[0]);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[1]);
        fwrite(&Bmpnul,1,count8,fpDestBmpfile2[2]);
    }
    printf("Hint:Convert RGB To GRAY Successfully!\n");
    free(data);//释放内存空间

    
    fclose(fpDestBmpfile);//关闭文件指针

    fclose(fpSrcBmpfile);
    return 0;
}
