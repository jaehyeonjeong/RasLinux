#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmpHeader3.h"

#define BYTE	unsigned char
#define BASE    16

#define widthbytes(bits)  (((bits)+31)/32*4)

int main(int argc, char** argv)
{
	FILE *fp;
	RGBQUAD palrgb[256];

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	char input[128], output[128];

	float r, g, b, gray;
	unsigned int imagesize;   

	int i, j, size, index;
	unsigned long histogram[256];
	unsigned char *inimg;
	unsigned char *outimg;
 
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);
 
	if((fp = fopen(input, "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		exit(EXIT_FAILURE);
	}

	fread(&bmpFileHeader, sizeof(bmpFileHeader), 1, fp);

	fread(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp);

	size = widthbytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth);

	imagesize = bmpInfoHeader.SizeImage;
	if(!imagesize) imagesize = bmpInfoHeader.biHeight * size;
	inimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	outimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	fread(inimg, sizeof(BYTE), imagesize, fp);
	fclose(fp);
 
printf("Image width : %d, height : %d(%d)\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth*bmpInfoHeader.biHeight);
	
	for(i = 0; i < 256; i++) histogram[i] = 0;
 
	for(i = 0; i < bmpInfoHeader.biHeight; i++) {
		index = (bmpInfoHeader.biHeight-i-1) * size; 
		for(j = 0 ; j < bmpInfoHeader.biWidth; j++) { 
			r = (float)inimg[index+3*j+2];
			g = (float)inimg[index+3*j+1];
			b = (float)inimg[index+3*j+0];
			//그레이 스케일로 변환하는 공식
			gray = (r*0.3F)+(g*0.59F)+(b*0.11F);
			histogram[(unsigned char)(gray)] += 1;
			outimg[index+3*j] = outimg[index+3*j+1] = outimg[index+3*j+2] = gray;
		};
	};

    for(i = 0; i < 256; i++) {
		printf("%-3d  ", i); //-는 뒤에서 나오는수
		if(!(int)(histogram[i]/bmpInfoHeader.biHeight) && (histogram[i]%bmpInfoHeader.biHeight))
		   printf("*");
		else for(j = 0; j < (int)(histogram[i]/bmpInfoHeader.biHeight); j++)
		   printf("*");
		printf("  %d\n", histogram[i]);
    };
	
	bmpFileHeader.bfOffBits += 256*sizeof(RGBQUAD); 

	if((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		exit(EXIT_FAILURE);
	}

	fwrite(&bmpFileHeader, sizeof(bmpFileHeader), 1, fp);

	fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp);

	fwrite(palrgb, sizeof(unsigned int), 256, fp);
	fwrite(outimg, sizeof(unsigned char), imagesize, fp);

	free(inimg);
	free(outimg);

	fclose(fp);

    return 0;
}
