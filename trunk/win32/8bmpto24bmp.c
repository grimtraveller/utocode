/**
 * @file	8bmpto24bmp.c 
 * @brief	convert 8-bit bitmap file to 24-bit bitmap
 * @details	
 * @author	zuohaitao
 * @date	2011-04-14 
 * @warning	
 *			[todo]
 *			1. support 16-bit bitmap and 32-bit bitmap
 *			2. test run in linux OS
 * @bug	
 */
#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>

typedef unsigned char zjbyte;

unsigned int fsize(FILE* fp)
{
	int fd = _fileno(fp);
	struct _stat s = {0};
	_fstat(fd, &s);
	return s.st_size;
}



zjbyte* loadbmp(const char* filename)
{
	zjbyte* data = NULL;
	int ret = -1;
	unsigned int size = 0;
	BITMAPFILEHEADER fileHdr = {0};
	PBITMAPINFOHEADER  pImageHdr = NULL;
	LPRGBQUAD pColorTbl = NULL;
	FILE* fp = NULL;
	fp = fopen(filename, "rb");
	if (NULL == fp)
	{
		return NULL;
	}
	size = fsize(fp);
	data = (zjbyte*)malloc(size*sizeof(zjbyte));
	if (1 != fread(data, size, 1, fp))
	{
		fclose(fp);
		free(data);
		data = NULL;
		return data;
	}
	fclose(fp);
	return data;
}
 
int getBmpType(const zjbyte* data)
{
	const unsigned short int type = 'B'|'M'<<8;
	BITMAPFILEHEADER* pfileHdr = NULL;
	PBITMAPINFOHEADER  pImageHdr = NULL;
	pfileHdr = (BITMAPFILEHEADER*)data;
	pImageHdr = (PBITMAPINFOHEADER)(data + sizeof(BITMAPFILEHEADER));
	if (type != pfileHdr->bfType)
	{
		return -1;
	}
	return pImageHdr->biBitCount;
}

zjbyte* to24bmp(const zjbyte* src)
{
	int type = -1;
	int quad_count = 0;
	PBITMAPFILEHEADER src_fh = NULL;
	PBITMAPINFOHEADER src_bi = NULL;
	LPRGBQUAD src_rgb = NULL;
	zjbyte* src_bits = NULL;
	unsigned int src_bits_size = 0;


	unsigned int des_size = 0;
	zjbyte* des = NULL;
	PBITMAPINFOHEADER des_bi = NULL;
	zjbyte* des_bits = NULL;

	unsigned int i = 0;
	unsigned int j = 0;
	
	
	type = getBmpType(src);
	if (8 != type)
	{
		return NULL;
	}
	quad_count = 1<<type;
	src_fh = (PBITMAPFILEHEADER)src;
	src_bi = (PBITMAPINFOHEADER)(src + sizeof(BITMAPFILEHEADER));
	src_rgb =  (LPRGBQUAD)(src + sizeof(BITMAPFILEHEADER) + src_bi->biSize);
	src_bits = (zjbyte*)(src + src_fh->bfOffBits);
	src_bits_size = src_bi->biWidth * src_bi->biHeight; 
	des_size = (src_bi->biWidth * src_bi->biHeight)*3;
	des = (zjbyte*)malloc(des_size);
	if (NULL == des)
	{
		return NULL;
	}
	des_bi = (PBITMAPINFOHEADER)des;
	des_bi->biSize = sizeof(BITMAPINFOHEADER);
	des_bi->biWidth = src_bi->biWidth;
	des_bi->biHeight = src_bi->biHeight;
	des_bi->biPlanes = 1;
	des_bi->biBitCount = 24;
	des_bi->biCompression = src_bi->biCompression;
	des_bi->biSizeImage = des_bi->biWidth*des_bi->biHeight*3; 
	des_bi->biXPelsPerMeter = src_bi->biXPelsPerMeter;
	des_bi->biYPelsPerMeter = src_bi->biYPelsPerMeter;
	des_bi->biClrUsed = 0;
	des_bi->biClrImportant = 0;
	des_bits = des + des_bi->biSize;
	for (i = 0,j = 0; i < src_bits_size; i++,j+=3)
	{
		*(des_bits+j+0) = src_rgb[*(src_bits+i)].rgbBlue;
		*(des_bits+j+1) = src_rgb[*(src_bits+i)].rgbGreen;
		*(des_bits+j+2) = src_rgb[*(src_bits+i)].rgbRed;
	}
	return des;
}


int savebmp(const zjbyte* data, const char* filename)
{
	BITMAPFILEHEADER fh = {0};
	PBITMAPINFOHEADER  pbi = NULL;
	LPRGBQUAD prgb = NULL;
	zjbyte* pbits = NULL;
	FILE * fp = NULL;
	int quad_count = 0;

	pbi = (PBITMAPINFOHEADER)data;
	prgb = (LPRGBQUAD)(data + sizeof(BITMAPINFOHEADER));
	if (24 != pbi->biBitCount)
	{
		quad_count = 1<<pbi->biBitCount;
	}

	pbits = (zjbyte*)(data + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*quad_count);
	fh.bfSize = pbi->biSizeImage
						+ sizeof(BITMAPFILEHEADER)
						+ sizeof(BITMAPINFOHEADER)
						+ sizeof(RGBQUAD)*quad_count;
	fh.bfType = 'B'|'M'<<8;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER)
						+ sizeof(BITMAPINFOHEADER)
						+ sizeof(RGBQUAD)*quad_count;
	fp = fopen(filename, "wb");
	if(fp)
	{
		if (1 != fwrite(&fh, sizeof(BITMAPFILEHEADER), 1, fp))
		{
			goto ERROUT;
		}
		if (1 != fwrite(pbi, sizeof(BITMAPINFOHEADER), 1, fp))
		{
			goto ERROUT;
		}
		if (24 != pbi->biBitCount)
		{
			if (1 != fwrite(prgb, sizeof(RGBQUAD)*256, 1, fp))
			{
				goto ERROUT;
			}
		}
		if (1 != fwrite(pbits,pbi->biSizeImage, 1, fp))
		{
			goto ERROUT;
		}
	}
	else
	{
		goto ERROUT;
	}
	fclose(fp);
	return TRUE;
ERROUT:
	if (NULL != fp)
	{
		fclose(fp);
	}
	return FALSE;
}

int bmp8to24(const char* bmp8file, const char* bmp24file)
{
	zjbyte* source = NULL;
	zjbyte* dest = NULL;
	source = loadbmp(bmp8file);
	if (NULL == source)
	{
		printf("loadbmp is fail\n");
		return -1;
	}
	dest = to24bmp(source);
	if (NULL == dest)
	{
		printf("to24bmp is fail\n");
		free(source);
		return -1;
	}
	free(source);
	savebmp(dest, bmp24file);
	free(dest);
	return 0;

}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("arg error\n");
		return -1;
	}
	return bmp8to24(argv[1], argv[2]);
}
