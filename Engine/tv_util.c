#include "tv_util.h"

int UtilReadFile(const char* filename, char** buffer)
{
    FILE* fp = NULL;
    char* data = NULL;
    long lSize;

    fp = fopen(filename, "rb");
    if(fp == NULL) {
        fprintf(stderr, "Error: could not open file %s for reading\n", filename);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    
    rewind(fp);
    data = (char*)malloc(sizeof(char) * (lSize+1));
    if(data == NULL) {
        fclose(fp);
        fprintf(stderr, "Failed to allocate buffer for load\n");
        return -2;
    }
    if(lSize != fread(data, sizeof(char), lSize, fp)) {
        fclose(fp);
        free(data);
        fprintf(stderr, "Failed to file %s\n", filename);
        return -3;
    }
    data[lSize] = '\0';
    *buffer = data;
    fclose(fp);
    return 0;
}

