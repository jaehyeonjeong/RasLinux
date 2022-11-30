#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define BYTE unsigned char

#define widthbytes(bits) (((bits)+31)/32*4)

typedef struct tagRGBQUAD{
	BYTE rgbBlue; 
	BYTE rgbGreen; 
	BYTE rgbRed; 
	BYTE rgbReserved;
} RGBQUAD;

typedef struct {

	/* BMP FILE INFO */
	unsigned short int type; 
	unsigned int file_size; 
	unsigned short int reserved1; 
	unsigned short int reserved2; 
	unsigned int offset;
}	BMPFILEHEADER;


typedef struct{
	/* BMP IMAGE INFO */
	unsigned int header_size; 
	int width, height; 
	unsigned short int planes; 
	unsigned short int bits; 
	unsigned int compression; 
	int hresolution, vresolution; 
	unsigned int ncolors, importantcolors; 
}BMPINFOHEADER;


inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
    return(value > max? max : value < min? min : value);
}

int main(int argc, char** argv) {
	FILE* fp; 
	RGBQUAD palrgb[256];
	BMPFILEHEADER bmpFileheader;
	BMPINFOHEADER bmpInfoheader;
	char input[128], output[128];
	unsigned int imagesize; 

	int i, j, size; 
	float srcX, srcY;
	int index; 
	float r,g,b,gray;
	int graysize; 
	int index2;
	
	unsigned char *grayimg, *inimg, *outimg;
	
	/* usage a.out in.bmp out.bmp */
	strcpy(input, argv[1]); 
	strcpy(output, argv[2]);
	
	
	if((fp=fopen(input, "rb")) == NULL) { 
		fprintf(stderr, "Error : Failed to open file...\n"); 
		return -1;
	}
	fread(&bmpFileheader, sizeof(BMPFILEHEADER), 1, fp);

	fread(&bmpInfoheader, sizeof(BMPINFOHEADER), 1, fp);

	size=widthbytes(bmpInfoheader.bits * bmpInfoheader.width); 
	graysize = widthbytes(8 * bmpInfoheader.width);
    	
	if(!imagesize) 
		imagesize=bmpInfoheader.height*size;
	
	inimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoheader.imagesize); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoheader.imagesize); 
	fread(inimg, sizeof(BYTE), bmpInfoheader.imagesize, fp); 
	
	fclose(fp);
	
	for(i=0; i<bmpInfoheader.height*3; i+=3) { 
		for(j=0; j<bmpInfoheader.width*3; j+=3) {
			int b = inimg[j+(i*bmpInfoheader.width+0)]; 
			int g = inimg[j+(i*bmpInfoheader.width+1)]; 
			int r = inimg[j+(i*bmpInfoheader.width+2)]; 
			
			b += 50;
			g += 50;
			r += 50; 
			
			outimg[j+bmpInfoheader.width*i+0]= clip(b, 0, 255);
			outimg[j+bmpInfoheader.width*i+1]= clip(g, 0, 255);
			outimg[j+bmpInfoheader.width*i+2]= clip(r, 0, 255);
		};
	 };	   
	 
	size=widthbytes(bmpInfoheader.bits*bmpInfoheader.width); 
	bmpInfoheader.imagesize=bmpInfoheader.height*size; 
	bmpFileheader.offset+=256*sizeof(RGBQUAD);
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...\n"); 
		return -1;
	}
	fwrite(&bmpFileheader, sizeof(BMPFILEHEADER), 1, fp);
	fwrite(&bmpInfoheader, sizeof(BMPINFOHEADER), 1, fp);

	fwrite(palrgb, sizeof(unsigned int), 256, fp); 

	fwrite(outimg, sizeof(unsigned char), bmpInfoheader.imagesize, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 
	
	return 0;
}
