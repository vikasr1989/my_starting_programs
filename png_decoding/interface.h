#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define SUCCESS 0
#define FAIL -1

#define LOGD printf
#define LOGE printf


#define READ_WORD(ptr,t) 		\
{					\
	char *temp_ptr = (in_img);	\
	int i;				\
	unsigned char temp;		\
	temp = *temp_ptr++;		\
	i = temp;			\
	temp = *temp_ptr++;		\
	i <<= 8;			\
	i|= temp;			\
	temp = *temp_ptr++;		\
	i <<= 8;			\
	i|= temp;			\
	temp = *temp_ptr++;		\
	i <<= 8;			\
	i|= temp;			\
	t = i;				\
}					\


int check_png_file_for_signature(char *in);
int decode_png_file(char *in_img);
#endif
