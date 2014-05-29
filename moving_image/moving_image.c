#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SUCCESS 0
#define FAIL    -1

#define LOGD //printf
#define LOGE printf
#define FRAME_COUNT 1000
typedef struct
{
	short signature;
	int filesize;
	short reserved1;
	short reserved2;
	int offset;
	int size_bmp_info_header;
	int width;
	int height;
	short no_planes;
	short bpp;
	int cmp_type;
	int raw_size;
	int hor_res;
	int ver_res;
	int num_col;
	int num_imp_col;
	char *raw_image;
}bmp_header;

void usage(char *progname)
{
	LOGE("Usage is %s <in_file_name1> <in_file_name2> <out_file_name>",progname);
}
int convert_bmp_image_to_rgb(char *img,bmp_header *hdr)
{
	if(img == NULL)
	{
		LOGE("NULL pointer sent for conversion\n");
		return FAIL;
	}
	if(hdr == NULL)
	{
		LOGE("NULL pointer sent for header\n");
		return FAIL;
	}
	/* Read the header and raw_image as specified in the specification */
	char *input = img;
	char *raw_image = NULL;
	int *tp1;
	short *tp2;
	int t1;
	short t2;
	int i,j;

	int row_size;
	int padding;
	t2 = *input++;
	t1 = *input++;
	t1 <<= 8;
	t2 |= t1;
	hdr->signature = t2;
	LOGD("temp %x\n",t2);
	if(t2 != 0x4d42)
		return FAIL;
	tp1 = (int *)input;
	hdr->filesize = *tp1;
	input += 4;
	LOGD("filesize %d\n",hdr->filesize);
	tp2 = (short *)input;
	hdr->reserved1 = *tp2;
	input += 2;
	LOGD("reserved %d\n",hdr->reserved1);
	tp2 = (short *)input;
	hdr->reserved2 = *tp2;
	input += 2;
	LOGD("reserved %d\n",hdr->reserved2);
	tp1 = (int *)input;
	hdr->offset = *tp1;
	input += 4;
	LOGD("offset %d\n",hdr->offset);
	tp1 = (int *)input;
	hdr->size_bmp_info_header = *tp1;
	input += 4;
	LOGD("size of bitmapinfoheader %d\n",hdr->size_bmp_info_header);
	tp1 = (int *)input;
	hdr->width = *tp1;
	input += 4;
	LOGD("width %d\n",hdr->width);
	tp1 = (int *)input;
	hdr->height = *tp1;
	input += 4;
	LOGD("height %d\n",hdr->height);
	tp2 = (short *)input;
	hdr->no_planes = *tp2;
	input += 2;
	LOGD("number of planes in the image %d\n",hdr->no_planes);
	tp2 = (short *)input;
	hdr->bpp = *tp2;
	input += 2;
	LOGD("number of bits per pixel %d\n",hdr->bpp);
	tp1 = (int *)input;
	hdr->cmp_type = *tp1;
	input += 4;
	LOGD("compression type %d\n",hdr->cmp_type);
	tp1 = (int *)input;
	hdr->raw_size = *tp1;
	input += 4;
	LOGD("size of image data in bytes %d\n",hdr->raw_size);
	tp1 = (int *)input;
	hdr->hor_res = *tp1;
	input += 4;
	LOGD("horizontal resolution %d\n",hdr->hor_res);
	tp1 = (int *)input;
	hdr->ver_res = *tp1;
	input += 4;
	LOGD("vertical resolution %d\n",hdr->ver_res);
	tp1 = (int *)input;
	hdr->num_col = *tp1;
	input += 4;
	LOGD("number of colors in image %d\n",hdr->num_col);
	tp1 = (int *)input;
	hdr->num_imp_col = *tp1;
	input += 4;
	LOGD("number of important colors %d\n",hdr->num_imp_col);

	row_size = hdr->width*hdr->bpp/8;
	hdr->raw_image = calloc(hdr->width*hdr->height*hdr->bpp/8,1);
	if(hdr->raw_image == NULL)
	{
		LOGE("cannot allocate memory\n");
	}
	LOGD("rowsize %d calculated %d\n",row_size,(((hdr->bpp*hdr->width + 31)/32)*4));
	padding =  (((hdr->bpp*hdr->width + 31)/32)*4) - row_size;
	LOGD("padding %d\n",padding);
	input = img + hdr->offset;
	raw_image = hdr->raw_image+row_size*(hdr->height-1);
	for(i=0;i<hdr->height;i++)
	{
		for(j=0;j<hdr->width;j++)
		{
			*raw_image++ = *(input+2);
			*raw_image++ = *(input+1);
			*raw_image++ = *(input+0);
			input += 3;

		}
		raw_image -= (2*hdr->width*3);
		input += padding;
	}
	LOGD("Returning from bmp to rgb conversion\n");
	return SUCCESS;
}


int convert_rgb_image_to_yuv(char* rgb,char* yuv,int width,int height)
{
	if(rgb == NULL)
	{
		LOGE("NULL pointer sent for conversion\n");
		return FAIL;
	}
	if(yuv == NULL)
	{
		LOGE("NULL pointer sent for conversion\n");
		return FAIL;
	}
	int ret_val = convert_rgb_image_to_yuv444(rgb,yuv,width,height);
	return ret_val;
}

int convert_rgb_image_to_yuv444(char* rgb,char* yuv,int width,int height)
{
	unsigned char *in_img = (unsigned char *)rgb;
	char *out_img = yuv;
	unsigned char Y,U,V;
	unsigned char R,G,B;
	int i,j;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			B = *in_img++;
			G = *in_img++;
			R = *in_img++;

			Y = (char)(0.299*R + 0.587*G + 0.114*B);
			U = (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
			V = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
			
			*out_img++ = Y;
			*out_img++ = U;
			*out_img++ = V;
		}
	}
	LOGD("Returning from rgb to yuv conversion\n");
	return SUCCESS;
}

int overlay(char* yuv1,char* yuv2,char* out,int width1,int height1,int width2,int height2,int x,int y)//x is col no y is row no
{
	if(yuv1 == NULL)
	{
		LOGE("NULL pointer sent for overlay\n");
		return FAIL;
	}
	if(yuv2 == NULL)
	{
		LOGE("NULL pointer sent for overlay\n");
		return FAIL;
	}
	if(out == NULL)
	{
		LOGE("NULL pointer sent for overlay\n");
		return FAIL;
	}
	int i,j;
	unsigned char Y,U,V;
	/* Keep the smaller image as the image to be overlayed */
	if(width2>width1)
	{
		j = width1;
		width1 = width2;
		width2 = j;
	}
	if(height2 > height1)
	{
		j = height1;
		height1 = height2;
		height2 = j;
	}

	/* Copy the first y lines from larger image */
	memcpy(out,yuv1,y*width1*3);
	out+=y*width1*3;
	yuv1+=y*width1*3;

	/* Copy the smaller image */
	for(i=0;i<height2;i++)
	{
#ifdef REMOVE_SURROUNDING_BLACK
		for(j=0;j<x;j++)
		{
			*out++ = *yuv1++;
			*out++ = *yuv1++;
			*out++ = *yuv1++;
		}
		for(j=0;j<width2;j++)
		{
			Y = *yuv2++;
			U = *yuv2++;
			V = *yuv2++;
			//printf("Y %d U %d V %d\n",Y,U,V);
			if(Y==0 && U==128 && V==128)
			{
				*out++ = *yuv1++;
				*out++ = *yuv1++;
				*out++ = *yuv1++;
			}
			else
			{
				*out++ = Y;
				*out++ = U;
				*out++ = V;
				yuv1++;
				yuv1++;
				yuv1++;
			}
		}
		for(j=width2+x;j<width1;j++)
		{
			*out++ = *yuv1++;
			*out++ = *yuv1++;
			*out++ = *yuv1++;
		}
#else
		memcpy(out,yuv1,(x)*3);
		memcpy(out+x*3,yuv2,width2*3);
		memcpy(out+(x+width2)*3,yuv1+(x+width2)*3,(width1-width2-x)*3);
		out+=width1*3;
		yuv1+=width1*3;
		yuv2+=width2*3;
#endif
	}
	/* Copy the remaining from the larger image */
	memcpy(out,yuv1,(height1-height2-y)*width1*3);

	return SUCCESS;
}

int main(int argc, char *argv[])
{
	FILE *fp1 , *fp2, *fp3;
	int filelen1,filelen2;
	char *img1,*img2;
	char *yuv1,*yuv2;
	char *out;
	bmp_header *hdr1;
	bmp_header *hdr2;
	if(argc < 4)
	{
		usage(argv[0]);
		return FAIL;
	}
	LOGD("in_image1 %s in_image2 %s out_file %s\n",argv[1],argv[2],argv[3]);

	fp1 = fopen(argv[1],"rb");
	if(fp1 == NULL)
	{
		LOGE("Cannot open %s\n",argv[1]);
		return FAIL;
	}
	fp2 = fopen(argv[2],"rb");
	if(fp2 == NULL)
	{
		LOGE("Cannot open %s\n",argv[2]);
		return FAIL;
	}
	fp3 = fopen(argv[3],"wb");
	if(fp3 == NULL)
	{
		LOGE("Cannot open %s\n",argv[3]);
		return FAIL;
	}
	fseek(fp1,0,SEEK_END);
	fseek(fp2,0,SEEK_END);
	filelen1 = ftell(fp1);
	filelen2 = ftell(fp2);
	LOGD("filelen1 %d\n",filelen1);
	LOGD("filelen2 %d\n",filelen2);
	fclose(fp1);
	fclose(fp2);
	
	img1 = (char *)calloc(filelen1,1);
	if(img1==NULL)
	{
		LOGE("Cannot allocate memory for in_image1\n");
		return FAIL;
	}
	img2 = (char *)calloc(filelen2,1);
	if(img2==NULL)
	{
		LOGE("Cannot allocate memory for in_image2\n");
		return FAIL;
	}
	
	fp1 = fopen(argv[1],"rb");
	if(fp1 == NULL)
	{
		LOGE("Cannot open %s\n",argv[1]);
		return FAIL;
	}
	fp2 = fopen(argv[2],"rb");
	if(fp2 == NULL)
	{
		LOGE("Cannot open %s\n",argv[2]);
		return FAIL;
	}

	int bytes = fread(img1,1,filelen1,fp1);
	LOGD("bytes %d\n",bytes);
	if(filelen1 != bytes)
	{
		LOGE("Cannot read %s\n",argv[1]);
		return FAIL;
	}
	bytes = fread(img2,1,filelen2,fp2);
	LOGD("bytes %d\n",bytes);
	if(filelen2 != bytes)
	{
		LOGE("Cannot read %s\n",argv[2]);
		return FAIL;
	}

	hdr1 = (bmp_header *)calloc(sizeof(bmp_header),1);
	hdr2 = (bmp_header *)calloc(sizeof(bmp_header),1);
	if(convert_bmp_image_to_rgb(img1,hdr1))
	{
		LOGE("conversion failed %s\n",argv[1]);
		return FAIL;
	}
	if(hdr1->bpp != 24)
	{
		LOGE("Valid only for uncompressed bmp\n");
		return FAIL;
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug1.rgb","wb");
		fwrite(hdr1->raw_image,hdr1->width*hdr1->height*3,1,fp);
		fclose(fp);
	}
#endif
	
	if(convert_bmp_image_to_rgb(img2,hdr2))
	{
		LOGE("conversion failed %s\n",argv[2]);
		return FAIL;
	}
	if(hdr2->bpp != 24)
	{
		LOGE("Valid only for uncompressed bmp\n");
		return FAIL;
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug2.rgb","wb");
		fwrite(hdr2->raw_image,hdr2->width*hdr2->height*3,1,fp);
		fclose(fp);
	}
#endif
	
	yuv1 = (char *)calloc(hdr1->width * hdr1->height * 3,1);
	if(yuv1 == NULL)
	{
		LOGE("cannot allocate memory for yuv image\n");
	}
	yuv2 = (char *)calloc(hdr2->width * hdr2->height * 3,1);
	if(yuv2 == NULL)
	{
		LOGE("cannot allocate memory for yuv image\n");
	}
	/* Convert rgb input1 to yuv */
	if(convert_rgb_image_to_yuv(hdr1->raw_image,yuv1,hdr1->width,hdr1->height))
	{
		LOGE("conversion failed rgb to yuv %s\n",argv[1]);
		return FAIL;
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug1.yuv","wb");
		fwrite(yuv1,hdr1->width*hdr1->height*3,1,fp);
		fclose(fp);
	}
#endif
	/* Convert rgb input2 to yuv */
	if(convert_rgb_image_to_yuv(hdr2->raw_image,yuv2,hdr2->width,hdr2->height))
	{
		LOGE("conversion failed rgb to yuv %s\n",argv[2]);
		return FAIL;
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug2.yuv","wb");
		fwrite(yuv2,hdr2->width*hdr2->height*3,1,fp);
		fclose(fp);
	}
#endif
	out = (char *)calloc(hdr1->width * hdr1->height * 3,1);
	if(out == NULL)
	{
		LOGE("cannot allocate memory for out\n");
		return FAIL;
	}
	/* Overlaying of smaller image over larger image and writing to file */
	{
		int x = 0;
		int inc_x = 1;
		int y = 0;
		int inc_y = 1;
		int ii;
		int width1 = hdr1->width;
		int width2 = hdr2->width;
		int height1 = hdr1->height;
		int height2 = hdr2->height;
		for(ii=0;ii<FRAME_COUNT;ii++)
		{
			overlay(yuv1,yuv2,out,hdr1->width,hdr1->height,hdr2->width,hdr2->height,x,y);
			x+= inc_x;
			y+= inc_y;
			if(x == 0 || (x+width2 == width1))
			{
				inc_x *= -1;
			}
			if(y == 0 || (y+height2 == height1))
			{
				inc_y *= -1;
			}
			fwrite(out,hdr1->width*hdr1->height*3,1,fp3);
		}
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug3.yuv","wb");
		fwrite(out,hdr1->width*hdr1->height*3,1,fp);
		fclose(fp);
	}
#endif
	
	/* Cleaning up */
	free(img1);
	free(img2);
	free(yuv1);
	free(yuv2);
	free(out);
	free(hdr1->raw_image);
	free(hdr2->raw_image);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	LOGD("End of main \n");
	return SUCCESS;
}
