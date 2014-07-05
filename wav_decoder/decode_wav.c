#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGD //printf
#define LOGC
#define LOGE printf
#define LOGI printf

#define FOURCC(a,b,c,d) a | (b<<8) | (c<<16) | (d<<24)

int my_get_bytes(char *cInputBuffer,int offset,int num_bytes,void *ptr);

#define GET_BYTES(size,ptr)   iNumBytes = my_get_bytes(cInputBuffer,iOffset,(size),(ptr)); \
                              iOffset += iNumBytes;                                        \
                              LOGD("value = %d\n",*(ptr));                                 \
                              LOGC("value = %c \n",*(ptr));                                 \

enum
{
    FOURCC_RIFF = FOURCC('R','I','F','F'),
    FOURCC_WAVE = FOURCC('W','A','V','E'),
    FOURCC_FMT  = FOURCC('f','m','t',' '),
    FOURCC_DATA = FOURCC('d','a','t','a'),
    FOURCC_FACT = FOURCC('f','a','c','t'),
    FOURCC_LIST = FOURCC('L','I','S','T'),
}FourccCodes;

int main(int argc, char* argv[])
{
    char *cInFileName;
    char *cOutFileName;
    char *cInputBuffer;
    int iOffset = 0;
    int iNumBytes = 0;
    int fourcc = 0;
    if(argc != 3)
    {
        LOGE("Filename not specified\n");
        printf("Usage %s <in_file_name> <out_file_name> \n",argv[0]);
        exit(1);
    }
    else
    {
        cInFileName = malloc(strlen(argv[1])+5);
        strcpy(cInFileName,argv[1]);
        cOutFileName = malloc(strlen(argv[2])+5);
        strcpy(cOutFileName,argv[2]);
    }
    FILE *InFile;
    InFile = fopen(cInFileName,"rb");
    if(!InFile)
    {
        LOGE("Cannot open input file %s\n",cInFileName);
        exit(1);
    }
    FILE *OutFile;
    OutFile = fopen(cOutFileName,"wb");
    if(!OutFile)
    {
        LOGE("Cannot open output file %s\n",cOutFileName);
        exit(1);
    }
    {
        int iChunkId;
        fread(&iChunkId,4,1,InFile);
        LOGD("iChunkId %x\n",iChunkId);
        if(iChunkId != FOURCC_RIFF)
        {
            LOGE("Not a valid riff format\n");
            goto error;
        }
        int iFileSize;
        fread(&iFileSize,4,1,InFile);
        LOGD("FileSize %d\n",iFileSize);

        cInputBuffer = malloc(iFileSize);
        fread(cInputBuffer,iFileSize,1,InFile);
        /* iRiffType */
        LOGD("iRiffType %x\n",fourcc);
        //while(iOffset == iFileSize)
        int count =0;
        while(iOffset < iFileSize)
        {
            int iDataSize = 0;
            char tmp;
            GET_BYTES(4,&fourcc);
            switch(fourcc)
            {
                case FOURCC_WAVE:
                    LOGD("Wave header found\n");
                    LOGD("count %d\n",count);
                    break;
                case FOURCC_FMT:
                    LOGD("FMT chunk found\n");
                    LOGD("count %d\n",count);
                    GET_BYTES(4,&iDataSize);
                    short sCompressionCode;
                    short sChannels;
                    short sBlockAlign;
                    short sBitPerSample;
                    int iSampRate;
                    int iAvgBytesSec;
                    int iExtraBytes;
                    GET_BYTES(2,&sCompressionCode);
                    GET_BYTES(2,&sChannels);
                    GET_BYTES(4,&iSampRate);
                    GET_BYTES(4,&iAvgBytesSec);
                    GET_BYTES(2,&sBlockAlign);
                    GET_BYTES(2,&sBitPerSample);
                    iExtraBytes = iDataSize - 16;
                    if(iExtraBytes % 2 != 0)
                    {
                        iExtraBytes++;
                    }
                    while(iExtraBytes--)
                    {
                        GET_BYTES(1,&tmp);
                    }
                    break;
                case FOURCC_DATA:
                    LOGD("DATA chunk found\n");
                    LOGD("count %d\n",count);
                    GET_BYTES(4,&iDataSize);
                    if(iDataSize % 2 != 0)
                    {
                        iDataSize++;
                    }
                    
                    /* Have to process data here */
                    fwrite(cInputBuffer+iOffset,iDataSize,1,OutFile);
                    iOffset += iDataSize;
                    LOGD("iDataSize %d\n",iDataSize);
                    LOGD("iFileSize %d\n",iFileSize);
                    LOGD("iOffset %d\n",iOffset);
                    break;
                case FOURCC_FACT:
                    LOGD("FACT chunk found\n");
                    LOGD("count %d\n",count);
                    GET_BYTES(4,&iDataSize);
                    if(iDataSize % 2 != 0)
                    {
                        iDataSize++;
                    }
                    while(iDataSize--)
                    {
                        GET_BYTES(1,&tmp);
                    }
                    break;
                case FOURCC_LIST:
                    LOGD("LIST chunk found\n");
                    LOGD("count %d\n",count);
                    GET_BYTES(4,&iDataSize);
                    if(iDataSize % 2 != 0)
                    {
                        iDataSize++;
                    }
                    while(iDataSize--)
                    {
                        GET_BYTES(1,&tmp);
                    }
                    break;                    
                default:
                    LOGI("chunk found with fourcc %x\n",fourcc);
                    LOGD("count %d\n",count);
                    GET_BYTES(4,&iDataSize);
                    if(iDataSize % 2 != 0)
                    {
                        iDataSize++;
                    }
                    while(iDataSize--)
                    {
                        GET_BYTES(1,&tmp);
                    }                     
                    break;

            }
            count++;
        }
    }

error:
    fclose(InFile);
    fclose(OutFile);
    return 0;
}

int my_get_bytes(char *cInputBuffer,int offset,int num_bytes,void *ptr)
{
    char *pData = cInputBuffer+offset;
    memcpy(ptr,pData,num_bytes);
    return num_bytes;
}
