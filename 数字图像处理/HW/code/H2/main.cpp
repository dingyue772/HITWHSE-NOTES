#include <stdio.h>
#include <stdlib.h>
#include "bmp.h" 


FILE* readBmp(FILE* fpSrcBmpFile);
void getBmpHeader(FILE*, PBITMAPFILEHEADER, PBITMAPINFOHEADER, PRGBQUAD);
void ChangeHistHeader(PBITMAPFILEHEADER, PBITMAPINFOHEADER);
void setHistHeader(FILE*, PBITMAPFILEHEADER, PBITMAPINFOHEADER, PRGBQUAD);
void calculateImage(FILE*, PBITMAPFILEHEADER, PBITMAPINFOHEADER, FILE*, PBITMAPFILEHEADER, PBITMAPINFOHEADER, FILE*, FILE*);
int main(int argc, char *argv[]) {
	FILE* fpSrcBmpFile = readBmp(fpSrcBmpFile);
	//  = (FILE*)malloc(sizeof(FILE));
	PBITMAPFILEHEADER fileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	PBITMAPINFOHEADER infoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	PRGBQUAD quadHeader = (PRGBQUAD)malloc(sizeof(RGBQUAD)*256);
	if (!fileHeader || !infoHeader || !quadHeader){
		printf("allocate memory error!");
		exit(0);
	}
	getBmpHeader(fpSrcBmpFile, fileHeader, infoHeader, quadHeader);
	ChangeHistHeader(fileHeader, infoHeader);
	FILE* fpHistBmpFile = fopen("hist.bmp", "wb");
	FILE* fpHistChangedBmpFile = fopen("histchanged.bmp", "wb");
	if (!fpHistBmpFile || !fpHistChangedBmpFile){
		printf("Error: Open the hist file!\n");
		exit(-1);
	}
	setHistHeader(fpHistBmpFile, fileHeader, infoHeader, quadHeader);
	setHistHeader(fpHistChangedBmpFile, fileHeader, infoHeader, quadHeader);
	PBITMAPFILEHEADER fileHeader_s = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	PBITMAPINFOHEADER infoHeader_s = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	PRGBQUAD quadHeader_s = (PRGBQUAD)malloc(sizeof(RGBQUAD)*256);
	if (!fileHeader_s || !infoHeader_s || !quadHeader_s){
		printf("allocate memory error!");
		exit(0);
	}
	getBmpHeader(fpSrcBmpFile, fileHeader_s, infoHeader_s, quadHeader_s);
	// change the image
	FILE* fpDestBmpFile = fopen("dest.bmp", "wb");
	if (fpDestBmpFile == NULL){
		printf("can't open the file!");
		exit(-1);
	}
	setHistHeader(fpDestBmpFile, fileHeader_s, infoHeader_s, quadHeader_s);
	calculateImage(fpSrcBmpFile, fileHeader_s, infoHeader_s, fpHistBmpFile, fileHeader, infoHeader, fpHistChangedBmpFile, fpDestBmpFile);
	fclose(fpSrcBmpFile);
	fclose(fpHistBmpFile);
	fclose(fpDestBmpFile);
	fclose(fpHistChangedBmpFile);
	free(fileHeader);
	free(infoHeader);
	free(quadHeader);
	free(fileHeader_s);
	free(infoHeader_s);
	free(quadHeader_s);
	return 0;
}

FILE* readBmp(FILE* fpSrcBmpFile) {
	char SrcBmpfile[256];
	printf("Input the path of SrcBmpfile:\n");
    scanf("%s", SrcBmpfile);
    if((fpSrcBmpFile=fopen(SrcBmpfile,"rb"))==NULL) 
    {
        printf("Error:Open the file of SrcBmpfile failed!\n");
        exit(-1);
    }
    rewind(fpSrcBmpFile);
	return fpSrcBmpFile;
//	 printf("read dim.bmp successfully!"); 
}

void getBmpHeader(FILE* fpSrcBmpFile, PBITMAPFILEHEADER fileHeader, PBITMAPINFOHEADER infoHeader, PRGBQUAD quadHeader){
	rewind(fpSrcBmpFile);
	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpSrcBmpFile);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpSrcBmpFile);
	fread(quadHeader, sizeof(RGBQUAD), 256, fpSrcBmpFile);
	rewind(fpSrcBmpFile);
//	printf("read headers successfully!"); 
}

void ChangeHistHeader(PBITMAPFILEHEADER fileHeader, PBITMAPINFOHEADER infoHeader){
	infoHeader->biWidth = 256;
	infoHeader->biHeight = 1024;
	infoHeader->biSizeImage = 256*1024;
	fileHeader->bfSize = fileHeader->bfOffBits+infoHeader->biSizeImage;
//	printf("change hist headers successfully!\n"); 
}
void setHistHeader(FILE* fpHistBmpFile, PBITMAPFILEHEADER fileHeader, PBITMAPINFOHEADER infoHeader, PRGBQUAD quadHeader){
	rewind(fpHistBmpFile);
	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpHistBmpFile);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpHistBmpFile);
	fwrite(quadHeader, sizeof(RGBQUAD)*256, 1, fpHistBmpFile);
	rewind(fpHistBmpFile);
//	printf("write hist headers successfully!\n"); 
}
void calculateImage(FILE* fpSrcBmpFile, PBITMAPFILEHEADER srcFileHeader, PBITMAPINFOHEADER srcInfoHeader, FILE* fpHistBmpFile, PBITMAPFILEHEADER histFileHeader, PBITMAPINFOHEADER histInfoHeader, FILE* fpHistChangedBmpFile, FILE* fpDestBmpFile){
	double countPixels[256] = {0};
	fseek(fpSrcBmpFile, srcFileHeader->bfOffBits, SEEK_SET);
	BYTE* data = (BYTE*)malloc(sizeof(BYTE));
	int width1 = srcInfoHeader->biWidth;
	int height1 = srcInfoHeader->biHeight;
	int rest = (4-(width1%4))%4;
	int i, j; 
	for (i=0; i<height1; i++) {
		for (j=0; j<width1; j++) {
			fread(data, sizeof(BYTE), 1, fpSrcBmpFile);
			countPixels[*data]++;
		}
		fseek(fpSrcBmpFile, rest, SEEK_CUR);
	}
	BYTE x;
	fseek(fpHistBmpFile, histFileHeader->bfOffBits, SEEK_SET);
	int width2 = histInfoHeader->biWidth;
	int height2 = histInfoHeader->biHeight;
	for(i=0; i<height2; i++){
		for (j=0; j<width2; j++){
			x = 255;
			if(countPixels[j]!=0 && countPixels[j]>=i)
				x = 0;
			fwrite(&x, sizeof(BYTE), 1, fpHistBmpFile);
		}
	}
		
	int COUNT = width1*height1; // n值
	// cdf计算,灰度级映射计算
	for (i=0; i<256; i++) {
		countPixels[i] = 1.0*countPixels[i]/COUNT;
		if (i>=1)
			countPixels[i] += countPixels[i-1];
	}
	int new_color[256] = {0};
	for (i=0; i<256; i++) {
//		countPixels[i] = countPixels[i]*255+0.5;
		new_color[i] = countPixels[i]*255+0.5;
//		printf("%d\n", new_color[i]); 
	}
	// 图像均衡化结果
	fseek(fpSrcBmpFile, srcFileHeader->bfOffBits, SEEK_SET);
	fseek(fpDestBmpFile, srcFileHeader->bfOffBits, SEEK_SET);

	for (i=0; i<height1; i++) {
		for (j=0; j<width1; j++) {
			fread(data, sizeof(BYTE), 1, fpSrcBmpFile);
			x = new_color[*data];
			fwrite(&x, sizeof(BYTE), 1, fpDestBmpFile);
		}
		fseek(fpSrcBmpFile, rest, SEEK_CUR);
		fseek(fpDestBmpFile, rest, SEEK_CUR);
	}
	fseek(fpDestBmpFile, srcFileHeader->bfOffBits, SEEK_SET);
	int countPixels2[256] = {0};
	// 再次统计
	for (i=0; i<height1; i++) {
		for (j=0; j<width1; j++) {
			fread(data, sizeof(BYTE), 1, fpDestBmpFile);
			countPixels2[*data]++;
		}
		fseek(fpDestBmpFile, rest, SEEK_CUR);
	}
	
	// 再次直方图
	for(i=0; i<height2; i++){
		for (j=0; j<width2; j++){
			x = 255;
			if(countPixels2[j]!=0 && countPixels2[j]>=i)
				x = 0;
			fwrite(&x, sizeof(BYTE), 1, fpHistChangedBmpFile);
		}
	}
	printf("1111111111111111\n");
	free(data);
}
