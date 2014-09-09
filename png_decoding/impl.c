#include "interface.h"
#include <stdlib.h>


#if 0
/* Sample CRC program */
   /* Table of CRCs of all 8-bit messages. */
   unsigned long crc_table[256];
   
   /* Flag: has the table been computed? Initially false. */
   int crc_table_computed = 0;
   
   /* Make the table for a fast CRC. */
   void make_crc_table(void)
   {
     unsigned long c;
     int n, k;
   
     for (n = 0; n < 256; n++) {
       c = (unsigned long) n;
       for (k = 0; k < 8; k++) {
         if (c & 1)
           c = 0xedb88320L ^ (c >> 1);
         else
           c = c >> 1;
       }
       crc_table[n] = c;
     }
     crc_table_computed = 1;
   }
   
   /* Update a running CRC with the bytes buf[0..len-1]--the CRC
      should be initialized to all 1's, and the transmitted value
      is the 1's complement of the final running CRC (see the
      crc() routine below)). */
   
   unsigned long update_crc(unsigned long crc, unsigned char *buf,
                            int len)
   {
     unsigned long c = crc;
     int n;
   
     if (!crc_table_computed)
       make_crc_table();
     for (n = 0; n < len; n++) {
       c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
     }
     return c;
   }
   
   /* Return the CRC of the bytes buf[0..len-1]. */
   unsigned long crc(unsigned char *buf, int len)
   {
     return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
   }
#endif

void *my_malloc(int size,void *malloc_buffer)
{
	char *ptr = (char *)malloc(size);
	return (void *)ptr;
}
int check_png_file_for_signature(char *in)
{
	unsigned char temp;
	char *temp_ptr = in;
	int i;
	{
	temp = *temp_ptr++;
	if(temp != 0x89)
		return FAIL;
	temp = *temp_ptr++;
	i = temp;
	temp = *temp_ptr++;
	i <<= 8;
	i|= temp;
	temp = *temp_ptr++;
	i <<= 8;
	i|= temp;
	if(i != 0x504E47) // PNG in ASCII
		return FAIL;
	temp = *temp_ptr++;
	i = temp;
	temp = *temp_ptr++;
	i <<= 8;
	i|= temp;
	if(i != 0x0D0A) // DOS-style line ending (CRLF)
		return FAIL;
	temp = *temp_ptr++;
	if(temp != 0x1A) //EOF in DOS
		return FAIL;
	temp = *temp_ptr++;
	if(temp != 0x0A) // UNIX style line ending
		return FAIL;
	}
#if 0
	int *ptr = (int *)temp_ptr;
	for(i=0;i<8;i++)
	printf("%x\n",*temp_ptr++);
	printf("%x\n",*temp_ptr++);
#endif
	return SUCCESS;
}




int decode_ihdr_png(char *in_img, int *bytes_consumed,void *malloc_buf)
{
	LOGD("In function decode_ihdr_png\n");
/*	IHDR Decoding */
	char *hdr_data;
	int err = 0;
	int length;
	int chunk_type;
	int crc;
   int Width=0;             //4 bytes
   int Height;            //4 bytes
   char Bit_depth;         //1 byte
   char Color_type;        //1 byte
   char Compression_method;//1 byte
   char Filter_method;     //1 byte
   char Interlace_method;  //1 byte

   READ_WORD(in_img,length)
	   in_img += 4;
	   LOGD("Length %d\n",length);

   READ_WORD(in_img,chunk_type)
	   in_img += 4;
   if(chunk_type == 0x49484452) // IHDR in ASCII
   {
	   LOGD("IHDR chunk found\n");
   }
	   LOGD("chunk_type %x\n",chunk_type);
	   hdr_data = my_malloc(length,malloc_buf);
	   if(hdr_data == NULL)
	   {
		   LOGE("Memory allocation failed\n");
		   return FAIL;
	   }

   READ_WORD(in_img,Width)
	   in_img += 4;
	   LOGD("Width %d\n",Width);

   READ_WORD(in_img,Height)
	   in_img += 4;
	   LOGD("Height %d\n",Height);

	   Bit_depth = *in_img++;
	   LOGD("Bit_depth %d\n",Bit_depth);

	   Color_type = *in_img++;
	   LOGD("Color_type %d\n",Color_type);
	   switch(Color_type)
	   {
		case 0:
			if(Bit_depth == 1 || Bit_depth == 2 || Bit_depth == 4 || Bit_depth == 8 || Bit_depth == 16)
			LOGD("Correct color_type\n");			
			else
				err = -1;
			break;
		case 2:
			if(Bit_depth == 8 || Bit_depth == 16)
			LOGD("Correct color_type\n");			
			else
				err = -1;
			break;
		case 3:
			if(Bit_depth == 1 || Bit_depth == 2 || Bit_depth == 4 || Bit_depth == 8)
			LOGD("Correct color_type\n");			
			else
				err = -1;
			break;
		case 4:
			if(Bit_depth == 8 || Bit_depth == 16)
			LOGD("Correct color_type\n");			
			else
				err = -1;
			break;
		case 6:
			if(Bit_depth == 8 || Bit_depth == 16)
			LOGD("Correct color_type\n");			
			else
				err = -1;
			break;
		default:
				err = -1;
			break;
	   }
	   if(err == -1)
	   {
			LOGE("Incorect color_type %d and bit depth %d\n",Color_type,Bit_depth);
			return FAIL;
	   }

	   Compression_method = *in_img++;
	   LOGD("Compression_method %d\n",Compression_method);
	   if(Compression_method != 0 )
	   {
		   LOGE("Compression_method is wrong\n");
		   return FAIL;
	   }

	   Filter_method = *in_img++;
	   LOGD("Filter_method %d\n",Filter_method);
	   if(Filter_method != 0 )
	   {
		   LOGE("Filter_method is wrong\n");
		   return FAIL;
	   }

	   Interlace_method = *in_img++;
	   LOGD("Interlace_method %d\n",Interlace_method);
	   if(Interlace_method != 0 && Interlace_method != 1)
	   {
		   LOGE("Interlace method is wrong\n");
		   return FAIL;
	   }
		READ_WORD(in_img,crc);
		LOGD("crc %x\n",crc);
		in_img+=4;
		*bytes_consumed = 4+4+length+4;
		return SUCCESS;
	}

int decode_iend_png(char *in_img, int *bytes_consumed,void *malloc_buf)
{
	LOGD("In function decode_iend_png\n");
	int length = 0;
	int chunk_type = 0;
	int crc = 0;
   READ_WORD(in_img,length)
	   in_img += 4;
	   LOGD("Length %d\n",length);

   READ_WORD(in_img,chunk_type)
	   in_img += 4;
   if(chunk_type == 0x49454e44) // IEND in ASCII
   {
	   LOGD("IEND chunk found\n");
   }
	   LOGD("chunk_type %x\n",chunk_type);
		READ_WORD(in_img,crc);
		LOGD("crc %x\n",crc);
		in_img+=4;
	*bytes_consumed = 4+4+length+4;
	return SUCCESS;
}

int decode_idat_png(char *in_img, int *bytes_consumed,void *malloc_buf)
{
	LOGD("In function decode_idat_png\n");
	int length = 0;
	int chunk_type = 0;
	int crc = 0;
	int temp = 0;
   READ_WORD(in_img,length)
	   in_img += 4;
	   LOGD("Length %d\n",length);

   READ_WORD(in_img,chunk_type)
	   in_img += 4;
   if(chunk_type == 0x49444154) // IDAT in ASCII
   {
	   LOGD("IDAT chunk found\n");
   }
	   LOGD("chunk_type %x\n",chunk_type);
#if 0
   READ_WORD(in_img,temp)
	   in_img += 4;
	   LOGD("next %x\n",temp);
   READ_WORD(in_img,temp)
	   in_img += 4;
	   LOGD("next %x\n",temp);
   READ_WORD(in_img,temp)
	   in_img += 4;
	   LOGD("next %x\n",temp);
#endif
	   temp = *in_img;
	   LOGD("next %x\n",temp);
	   temp = *(in_img+1);
	   LOGD("next %x\n",temp);
	
	*bytes_consumed = 4+4+length+4;
	return SUCCESS;
}

int decode_png_file(char *in_img)
{
	int bytes_consumed;
	if(check_png_file_for_signature(in_img))
	{
		LOGE("Not a png file \n");
		return FAIL;
	}
	/* 8 bytes are consumed for signature */
	in_img += 8;
	int i;
	void *malloc_buf;
	int length,chunk_type;


	
	while(1)
	{
		READ_WORD(in_img,length);
		//LOGD("length of next chunk %d\n",length);
		in_img+= 4;
		READ_WORD(in_img,chunk_type);
		//LOGD("chunk_type of next chunk %x\n",chunk_type);
		in_img-= 4;
		switch(chunk_type)
		{
			case 0x49484452:
	   LOGD("IHDR chunk found\n");
	decode_ihdr_png(in_img,&bytes_consumed,malloc_buf);
	in_img+=bytes_consumed;
				break;
			case 0x49444154:
	   LOGD("IDAT chunk found\n");
	decode_idat_png(in_img,&bytes_consumed,malloc_buf);
	in_img+=bytes_consumed;
	   //return SUCCESS;
				break;
			case 0x49454e44:
	   LOGD("IEND chunk found\n");
	decode_iend_png(in_img,&bytes_consumed,malloc_buf);
	return SUCCESS;
	break;
			default:
				LOGE("Unsupported chunk_type found\n");
				//return FAIL;
				in_img++;
		}
	}
	

	return SUCCESS;
}
