#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAIL    -1

#define LOGD printf
#define LOGE printf

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
	LOGE("Usage is %s <in_file_name1> <out_file_name> <resized_width> <resized_height>",progname);
}

int convert_bmp_image_to_rgb(char *img,bmp_header *hdr)
{
	if(img == NULL)
	{
		LOGE("NULL pointer sent for conversion\n");
		return FAIL;
	}
	if(!hdr)
	{
		LOGE("NULL pointer sent hdr\n");
		return FAIL;
	}
	/* Read header and rgb values as given in the specification */
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

int resize(char *in_img,char *out_img,int in_width, int in_height,int out_width,int out_height,int num_components)
{
	int i,j;
	if(out_width>in_width)
	{
		LOGE("out_width %d in_width %d\n",out_width , in_width);
		LOGE("Resize to larger width not supported\n");
		return FAIL;
	}
	if(out_height > in_height)
	{
		LOGE("out_height %d in_height %d\n",out_height , in_height);
		LOGE("Resize to larger height not supported\n");
		return FAIL;
	}
	
	int hsr=0,vsr=0;
	float hsr_f,vsr_f;
	hsr = in_width/out_width;
	vsr = in_height/out_height;
	hsr_f = (float)in_width/out_width;
	vsr_f = (float)in_height/out_height;
	LOGD("in_height %d in_width %d\n",in_height,in_width);
	LOGD("hsr %d vsr %d\n",hsr,vsr);
	if(hsr == (int)hsr_f && vsr ==(float)vsr_f)
	{
		LOGD("Integer downsampling\n");
	for(i=0;i<in_height;i+=vsr)
	{
		for(j=0;j<in_width;j+=hsr)
		{
			*out_img++ = *in_img++;
			*out_img++ = *in_img++;
			*out_img++ = *in_img++;
			in_img += (hsr-1)*num_components;
		}
		in_img += (vsr-1)*in_width*num_components;
	}
	}
	else
	{
		LOGE("Non-integer downsampling not supported\n");
		return FAIL;
	}
	//memcpy(out_img,in_img,out_width*out_height*num_components);
	LOGD("Returning from resize function\n");
	return SUCCESS;
}

int resize_bmp_image(bmp_header *hdr1,bmp_header *hdr2,char *out)
{
	if(!hdr1)
	{
		LOGE("NULL pointer sent hdr1\n");
		return FAIL;
	}
	if(!hdr2)
	{
		LOGE("NULL pointer sent hdr2\n");
		return FAIL;
	}
	if(hdr2->width == 0)
	{
		LOGE("width to be resized is 0\n");
		return FAIL;
	}
	if(hdr2->height == 0)
	{
		LOGE("height to be resized is 0\n");
		return FAIL;
	}
	if(out == NULL)
	{
		LOGE("NULL pointer sent for out_ptr\n");
		return FAIL;
	}
	if(hdr1->bpp != 24)
	{
		LOGE("Valid only for uncompressed bmp\n");
		return FAIL;
	}

	int *tp1;
	short *tp2;
	int t1;
	short t2;
	int i,j;

	int row_size,calc_row_size;
	int padding;
	int raw_size;
	char *outptr, *raw_image;
	float hsr,vsr;

	/* Write header and rgb values as given in the specification */
	hdr2->bpp = hdr1->bpp;
	row_size = hdr2->width*hdr2->bpp/8;
	calc_row_size = (((hdr2->bpp*hdr2->width + 31)/32)*4);
	padding =  calc_row_size - row_size;

	t2 = hdr1->signature ;
	LOGD("temp %x\n",t2);
	if(t2 != 0x4d42)
		return FAIL;
	*out++ = (char)t2;
	t2>>=8;
	*out++ = (char)t2;
	hdr2->signature = hdr1->signature;

	hdr2->filesize = hdr2->height*calc_row_size + hdr1->offset;
	tp1 = (int *)out;
	*tp1 = hdr2->filesize;
	out += 4;
	LOGD("filesize %d\n",hdr2->filesize);

	tp2 = (short *)out;
	*tp2 = hdr1->reserved1;
	hdr2->reserved1 = hdr1->reserved1;
	out += 2;
	LOGD("reserved %d\n",hdr2->reserved1);
	tp2 = (short *)out;
	*tp2 = hdr1->reserved2;
	hdr2->reserved2 = hdr1->reserved2;
	out += 2;
	LOGD("reserved %d\n",hdr2->reserved2);
	tp1 = (int *)out;
	*tp1 = hdr1->offset;
	hdr2->offset = hdr1->offset;
	out += 4;
	LOGD("offset %d\n",hdr2->offset);
	tp1 = (int *)out;
	*tp1 = hdr1->size_bmp_info_header;
	hdr2->size_bmp_info_header = hdr1->size_bmp_info_header;
	out += 4;
	LOGD("size of bitmapinfoheader %d\n",hdr2->size_bmp_info_header);
	tp1 = (int *)out;
	*tp1 = hdr2->width;
	out += 4;
	LOGD("width %d\n",hdr2->width);
	tp1 = (int *)out;
	*tp1 = hdr2->height;
	out += 4;
	LOGD("height %d\n",hdr2->height);
	tp2 = (short *)out;
	*tp2 = hdr1->no_planes;
	hdr2->no_planes = hdr1->no_planes;
	out += 2;
	LOGD("number of planes in the image %d\n",hdr2->no_planes);
	tp2 = (short *)out;
	*tp2 = hdr1->bpp;
	hdr2->bpp = hdr1->bpp;
	out += 2;
	LOGD("number of bits per pixel %d\n",hdr2->bpp);
	tp1 = (int *)out;
	*tp1 = hdr1->cmp_type;
	hdr2->cmp_type = hdr1->cmp_type;
	out += 4;
	LOGD("compression type %d\n",hdr2->cmp_type);
	//raw_size = hdr2->width * hdr2->height * hdr2->bpp/8;
	raw_size = hdr1->raw_size;
	tp1 = (int *)out;
	*tp1 = raw_size;
	hdr2->raw_size = raw_size;
	out += 4;
	LOGD("size of image data in bytes %d\n",hdr2->raw_size);
	tp1 = (int *)out;
	*tp1 = hdr1->hor_res;
	hdr2->hor_res = hdr1->hor_res;
	out += 4;
	LOGD("horizontal resolution %d\n",hdr2->hor_res);
	tp1 = (int *)out;
	*tp1 = hdr1->ver_res;
	hdr2->ver_res = hdr1->ver_res;
	out += 4;
	LOGD("vertical resolution %d\n",hdr2->ver_res);
	tp1 = (int *)out;
	*tp1 = hdr1->num_col;
	hdr2->num_col = hdr1->num_col;
	out += 4;
	LOGD("number of colors in image %d\n",hdr2->num_col);
	tp1 = (int *)out;
	*tp1 = hdr1->num_imp_col;
	hdr2->num_imp_col = hdr1->num_imp_col;
	out += 4;
	LOGD("number of important colors %d\n",hdr2->num_imp_col);

	hdr2->raw_image = calloc(hdr2->width*hdr2->height*hdr2->bpp/8,1);

	if(hdr2->raw_image == NULL)
	{
		LOGE("cannot allocate memory\n");
	}
	LOGD("rowsize %d calculated %d\n",row_size,(((hdr2->bpp*hdr2->width + 31)/32)*4));
	LOGD("padding %d\n",padding);

	if(hdr1->raw_image == NULL)
	{
		LOGE("NULL pointer sent for raw_image input\n");
	}
	if(resize(hdr1->raw_image,hdr2->raw_image,hdr1->width,hdr1->height,hdr2->width,hdr2->height,(hdr2->bpp/8)))
	{
		LOGE("resizing failed\n");
		return FAIL;
	}

	outptr = out;
	raw_image = hdr2->raw_image+row_size*(hdr2->height-1);
	for(i=0;i<hdr2->height;i++)
	{
		for(j=0;j<hdr2->width;j++)
		{
			*(outptr+2) = *raw_image++;
			*(outptr+1) = *raw_image++;
			*(outptr+0) = *raw_image++;
			outptr += 3;

		}
		raw_image -= (2*hdr2->width*3);
		outptr += padding;
	}
	
	LOGD("Returning from resize bmp image\n");
	return SUCCESS;
}

int main(int argc,char* argv[])
{
	FILE *fp1 , *fp2;
	int filelen1,filelen2;
	char *img1;
	char *out;
	bmp_header *hdr1;
	bmp_header *hdr2;
	int bytes;
	int width2,height2;

	if(argc < 5)
	{
		usage(argv[0]);
		return FAIL;
	}
	LOGD("in_image1 %s in_image2 %s \n",argv[1],argv[2]);

	fp1 = fopen(argv[1],"rb");
	if(fp1 == NULL)
	{
		LOGE("Cannot open %s\n",argv[1]);
		return FAIL;
	}
	fp2 = fopen(argv[2],"wb");
	if(fp2 == NULL)
	{
		LOGE("Cannot open %s\n",argv[2]);
		return FAIL;
	}

	/* Read the width and height from the input arguments */
	width2 = atoi(argv[3]);
	height2 = atoi(argv[4]);

	fseek(fp1,0,SEEK_END);
	filelen1 = ftell(fp1);
	LOGD("filelen1 %d\n",filelen1);
	fclose(fp1);
	
	img1 = (char *)calloc(filelen1,1);
	
	fp1 = fopen(argv[1],"rb");
	if(fp1 == NULL)
	{
		LOGE("Cannot open %s\n",argv[1]);
		return FAIL;
	}
	bytes = fread(img1,1,filelen1,fp1);
	LOGD("bytes %d\n",bytes);
	if(filelen1 != bytes)
	{
		LOGE("Cannot read %s\n",argv[1]);
		return FAIL;
	}

	hdr1 = (bmp_header *)calloc(sizeof(bmp_header),1);
	hdr2 = (bmp_header *)calloc(sizeof(bmp_header),1);
	if(convert_bmp_image_to_rgb(img1,hdr1))
	{
		LOGE("conversion failed %s\n",argv[1]);
		return FAIL;
	}
#ifdef DEBUG
	{
		FILE* fp = fopen("debug1.rgb","wb");
		fwrite(hdr1->raw_image,hdr1->width*hdr1->height*3,1,fp);
		fclose(fp);
	}
#endif
	filelen2 = hdr1->offset + (height2*(((hdr1->bpp*width2 + 31)/32)*4));
	out = (char *)calloc(filelen2,1);

	hdr2->width = width2;
	hdr2->height = height2;
	if(resize_bmp_image(hdr1,hdr2,out))
	{
		LOGE("Cannot resize \n");
		return FAIL;
	}
	fwrite(out,filelen2,1,fp2);
	LOGD("End of main \n");

	/* Cleaning up */ 
	free(img1);
	free(out);
	free(hdr1->raw_image);
	free(hdr2->raw_image);
	fclose(fp1);
	fclose(fp2);
	return SUCCESS;
}
