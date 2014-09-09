
#include "interface.h"

void usage(char *progname)
{
	LOGE("Usage %s <input_png_file> <output_rgb_file>\n");
}

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		usage(argv[1]);
		return FAIL;
	}
	FILE *fp,*fp_out;
	char *in_img;
	int bytes;
	int filelen;

	fp = fopen(argv[1],"rb");
	if(fp == NULL)
	{
		LOGE("Cannot open input file %s\n",argv[1]);
		return FAIL;
	}
	fp_out = fopen(argv[2],"wb");
	if(fp_out == NULL)
	{
		LOGE("Cannot open output file %s\n",argv[2]);
		return FAIL;
	}
	fseek(fp,0,SEEK_END);
	filelen = ftell(fp);
	fclose(fp);
	fp = fopen(argv[1],"rb");
	in_img = calloc(filelen,1);
	bytes = fread(in_img,filelen,1,fp);
	LOGD("bytes %d\n",bytes);
#if 0
	int i;
	char *ptr = in_img;
	for(i=0;i<8;i++)
	printf("in_img %x\n",*ptr++);
	if(bytes != 8)
	{
		LOGE("Cannot read from input file %s",argv[1]);
		return FAIL;
	}
#endif

	if(check_png_file_for_signature(in_img))
	{
		LOGE("Not a png file \n");
		return FAIL;
	}
	LOGD("The input file is a png file \n");

	if(decode_png_file(in_img))
	{
		LOGE("Decoding failed \n");
		return FAIL;
	}
	LOGD("Decoding successful\n");
	LOGD("End of main function \n");
	free(in_img);
	fclose(fp);
	fclose(fp_out);
	return SUCCESS;
}

