#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>
#include "types.h"

typedef struct DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char magic_string[30];
    char decoded_magic_string[30];
    int size_magic_string;

    char extn_secret_file[20];
    int size_extn_secret_file;

    long size_secret_file;

    char *output_fname;
    FILE *fptr_output;
} DecodeInfo;

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decinfo);
Status do_decoding(DecodeInfo *decInfo);
Status open_decode_files(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status check_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status open_output_file(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status close_decode_files(DecodeInfo *decInfo);

#endif