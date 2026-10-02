#include<stdio.h>
#include<string.h>

#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decinfo)
{
    if(strstr(argv[2],".bmp")!= NULL)
    {
        printf(".bmp file is present.\n");
        decinfo->stego_image_fname = argv[2];
    }
    else{
        printf("Stego image is not .bmp file.\n");
        return e_failure;
    }
    // if(strstr(argv[3],".txt")!= NULL)
    // {
    //     printf(".txt file is present.\n");
    // }
    // else{
    //     printf("output file is not .txt file.\n");
    //     return e_failure;
    // }
    decinfo->output_fname = argv[3];
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo){
    printf("Enter your magic string to decode:\n");
    if(scanf(" %29[^\n]",decInfo->magic_string) != 1)
    {
        return e_failure;
    }

    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"rb");
    if(decInfo->fptr_stego_image == NULL){
        perror("fopen");
        fprintf(stderr,"Error: Unable to open the %s file",decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

Status decode_magic_string_size(DecodeInfo *decInfo){
    char image_buffer[32];
    int data = 0;
    
    if(fread(image_buffer,1,32,decInfo->fptr_stego_image) != 32){
        return e_failure;
    }
    
    for(int i = 0; i < 32; i++){
        int get_bit = image_buffer[i] & 1;
        data = data | (get_bit << i);
    }
    decInfo->size_magic_string = data;

     if(decInfo->size_magic_string < 0 ||
         decInfo->size_magic_string >= (int)sizeof(decInfo->decoded_magic_string))
    {
        return e_failure;
    }
    
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo){
    char image_buffer[8];
    for(int i = 0; i < decInfo->size_magic_string; i++){
        char data = 0;
        if(fread(image_buffer,1,8,decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
        for(int j = 0; j < 8; j++){
            int get_bit = image_buffer[j] & 1;
            data = data | (get_bit << j);
        }
        decInfo->decoded_magic_string[i] = data;
    }
    decInfo->decoded_magic_string[decInfo->size_magic_string] = '\0';
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    char image_buffer[32];
    int data = 0;
    
    if(fread(image_buffer,1,32,decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }
    
    for(int i = 0; i < 32; i++)
    {
        int get_bit = image_buffer[i] & 1;
        data = data | (get_bit << i);
    }
    decInfo->size_extn_secret_file = data;

     if(decInfo->size_extn_secret_file < 0 ||
         decInfo->size_extn_secret_file >= (int)sizeof(decInfo->extn_secret_file))
    {
        return e_failure;
    }
    
    return e_success;

}

Status decode_secret_file_extn(DecodeInfo *decInfo){
    char image_buffer[8];
    for(int i = 0; i < decInfo->size_extn_secret_file; i++){
        char data = 0;
        if(fread(image_buffer,1,8,decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
        for(int j = 0; j < 8; j++){
            int get_bit = image_buffer[j] & 1;
            data = data | (get_bit << j);
        }
        decInfo->extn_secret_file[i] = data;
    }
    decInfo->extn_secret_file[decInfo->size_extn_secret_file] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    long data = 0;

    if(fread(image_buffer,1,32,decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    for(int i = 0; i < 32;i++){
        int get_bit = image_buffer[i] & 1;
        data = data | ((long)get_bit << i);
    }
    decInfo->size_secret_file = data;
    return e_success;
}

Status open_output_file(DecodeInfo *decInfo){
    char output_filename[50];

    if(strstr(decInfo->output_fname, decInfo->extn_secret_file) != NULL)
    {
        snprintf(output_filename, sizeof(output_filename), "%s", decInfo->output_fname);
    }
    else
    {
        snprintf(output_filename, sizeof(output_filename), "%s%s",
                 decInfo->output_fname, decInfo->extn_secret_file);
    }

    decInfo->fptr_output = fopen(output_filename, "wb");

    if(decInfo->fptr_output == NULL){
        perror("fopen");
        return e_failure;
    }

    printf("Output file created: %s\n", output_filename);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];

    for(long i = 0; i < decInfo->size_secret_file; i++)
    {
        char data = 0;

        if(fread(image_buffer, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }

        for(int j = 0; j < 8; j++)
        {
            int get_bit = image_buffer[j] & 1;
            data = data | (get_bit << j);
        }

        if(fwrite(&data, 1 , 1, decInfo->fptr_output) != 1)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status close_decode_files(DecodeInfo *decInfo)
{
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    return e_success;
}

Status check_magic_string(DecodeInfo *decInfo){
    if(strcmp(decInfo->magic_string,decInfo->decoded_magic_string) == 0){
        return e_success;
    }

    return e_failure;
}
Status do_decoding(DecodeInfo *decInfo){
    if(open_decode_files(decInfo) == e_failure){
        printf("Error: we can't open the file.\n");
        return e_failure;
    }

    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    if(decode_magic_string_size(decInfo) == e_failure){
        printf("Error : unable to decode the magic string size.\n");
        return e_failure;
    }

    printf("Magic string size = %d\n",decInfo->size_magic_string);

    if(decode_magic_string(decInfo) == e_failure){
        printf("Error: unable to decode magic string.\n");
        return e_failure;
    }

    printf("Decoded magic string is = %s\n",decInfo->decoded_magic_string);

    if(check_magic_string(decInfo) == e_failure){
        printf("Magic string doesn't match.\n");
        return e_failure;
    }

    printf("Magic string is matched.\n");

    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Error: unable to decode extension size.\n");
        return e_failure;
    }

    printf("Extension size = %d\n",decInfo->size_extn_secret_file);

    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Error: unable to decode extension.\n");
        return e_failure;
    }

    printf("Secret file extension = %s\n",decInfo->extn_secret_file);

    if(decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error: unable to decode secret file size.\n");
        return e_failure;
    }

    printf("Secret file size = %ld bytes.\n",decInfo->size_secret_file);

    if(open_output_file(decInfo) == e_failure)
    {
        printf("Error: unable to create output file.\n");
        return e_failure;
    }

    if(decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error: unable to decode secret file data.\n");
        return e_failure;
    }

    close_decode_files(decInfo);

    // printf("Decoding completed successfully.\n");

    return e_success;
}