#include <stdio.h>
#include <assert.h>

#include "../include/file.h"


int get_file_capacity(FILE *fs)
{
    assert(fs != NULL && "[!] You have passed a null pointer as a file_stream!");

    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}