#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"


/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

//  OperationType check_operation_type(char *argv[]){
//     if(strstr(argv[1],"-e")){
//         return e_encode;
//     }
//     else if(strstr(argv[1],"-d")){
//         return e_decode;
//     }
//     else{
//         return e_unsupported;
//     }
//  }

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp") != NULL){
        printf(".bmp file is present\n");
        encInfo->src_image_fname = argv[2];
    }
    else{
        printf(RED"The source file is not .bmp file.\n"RESET);
        return e_failure;
    }
        encInfo->secret_fname = argv[3];
    
    if((argv[4] == NULL))
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    
    else{
        if(strstr(argv[4],".bmp") != NULL){
            encInfo->stego_image_fname = argv[4];
        }
        
        else{
            printf("The source file is not .bmp file.\n");
            return e_failure;
        }
    }

    return e_success;

}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity

    rewind(fptr_image);
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files_to_encode(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        
    	return e_failure;
    }
    
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        
    	return e_failure;
    }
    
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    
    // No failure return e_success
    return e_success;
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    int size = strlen(encInfo->extn_secret_file);

    char arr[32];
    if(fread(arr,1,sizeof(arr),encInfo->fptr_src_image) != sizeof(arr)){
        return e_failure;
    }
    
    encode_int_image(arr,size);

    if(fwrite(arr,1,sizeof(arr),encInfo->fptr_stego_image) != sizeof(arr)){
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    return encode_string_to_image(encInfo->extn_secret_file,
        strlen(encInfo->extn_secret_file),
        encInfo->fptr_src_image,
        encInfo->fptr_stego_image);
}

Status encode_secret_file_size_to_image(EncodeInfo *encInfo)
{
    int file_size = encInfo->size_secret_file;
    char arr[32];
    
    if(fread(arr,1,sizeof(arr),encInfo->fptr_src_image) != sizeof(arr))
    {
        return e_failure;
    }

    encode_int_image(arr,file_size);

    if(fwrite(arr,1,sizeof(arr),encInfo->fptr_stego_image) != sizeof(arr))
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_data(EncodeInfo * encInfo)
{
    for(long i = 0; i < encInfo->size_secret_file; i++)
    {
        char data;
        char image_buffer[8];

        if(fread(&data,1,1,encInfo->fptr_secret) != 1)
        {
            return e_failure;
        }
        
        if(fread(image_buffer,1,8,encInfo->fptr_src_image) != 8)
        {
            return e_failure;

        }

        encode_char_to_image(image_buffer,data);

        if(fwrite(image_buffer,1,8,encInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status get_secret_file_extn(EncodeInfo *encInfo)
{
    char * dot;
    dot = strrchr(encInfo->secret_fname,'.');

    if(dot == NULL)
    {
        return e_failure;
    }

    strcpy(encInfo->extn_secret_file,dot);

    return e_success;
}

Status encode_secret_file_size(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0,SEEK_END);

    encInfo->size_secret_file = ftell(encInfo->fptr_secret);

    rewind(encInfo->fptr_secret);

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024];
    size_t read_the_bytes;
    
    while((read_the_bytes = fread(buffer,1,1024,fptr_src)) > 0){
        if(fwrite(buffer,1,read_the_bytes,fptr_dest) != read_the_bytes){
            return e_failure;
        }
    }
    return e_success;
}


Status check_capacity(EncodeInfo *encInfo)
{
    // MOVE the secret_fptr to the last

    long size_of_info = sizeof(int) + strlen(encInfo->magic_string) + sizeof(int) + strlen(encInfo->extn_secret_file) + sizeof(int) + encInfo->size_secret_file ;
        
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    
    if((unsigned long)size_of_info * 8UL <= encInfo->image_capacity)
    {
        return e_success;
    }
    
    return e_failure;
}

Status copy_bmp_header(FILE*fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];
    
    if(fread(header,1,54,fptr_src_image)!=54){
        return e_failure;
    }
    
    if(fwrite(header,1,54,fptr_dest_image)!=54){
        return e_failure;
    }
    return e_success;
}

void encode_int_image(char *arr,int data)
{
    for(int i = 0; i < 32; i++){
        arr[i] = arr[i] & (~1);
        int get_bit = (data & (1 << i)) >> i;
        arr[i] = arr[i] | get_bit;
    }
}

Status encode_size_of_MS(EncodeInfo *encInfo)
{
    int size = strlen(encInfo->magic_string);
    
    char arr[32];
    if(fread(arr, 1, sizeof(arr), encInfo->fptr_src_image) != sizeof(arr))
    {
        return e_failure;
    }
    encode_int_image(arr,size);
    if(fwrite(arr, 1, sizeof(arr), encInfo->fptr_stego_image) != sizeof(arr))
    {
        return e_failure;
    }
    
    return e_success;
}

void encode_char_to_image(char *arr,char ch)
{
    for(int i = 0 ; i < 8; i++)
    {
        arr[i] = arr[i] & (~1);
        int get = (ch >> i) & 1;
        arr[i] = arr[i] | get;
    }
}

Status encode_string_to_image(char *str,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    for(int i = 0; i < size; i++)
    {
        char arr[8];
        if(fread(arr,1,8,fptr_src_image) != 8)
        {
            return e_failure;
        }

        encode_char_to_image(arr,str[i]);

        if(fwrite(arr,1,8,fptr_stego_image) != 8)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    return encode_string_to_image((char *)magic_string, strlen(magic_string),
    encInfo->fptr_src_image, encInfo->fptr_stego_image);
}

Status close_encode_files(EncodeInfo *encInfo)
{
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files_to_encode(encInfo) == e_failure){
        printf(RED"Error: unable to open the file.\n"RESET);
        return e_failure;
    }
    
    printf(BLUE "Enter your magic_string:\n"RESET);
    if(scanf(" %99[^\n]", encInfo->magic_string) != 1)
    {
        return e_failure;
    }
    
    if(get_secret_file_extn(encInfo) == e_failure)
    {    
        printf(RED"Error: unable to get secret file extension.\n"RESET);
        return e_failure;
    }

    if(encode_secret_file_size(encInfo) == e_failure)
    {    
        printf(RED"Error: unable to get secret file size.\n"RESET);
        return e_failure;
    }

    if(check_capacity(encInfo) == e_failure){
        printf(YELLOW"Error: Insufficient capacity in the source image.\n"RESET);
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image,
                        encInfo->fptr_stego_image) == e_failure){
        printf(RED"Error: Unable to copy the BMP header.\n"RESET);
        return e_failure;
    }
    
    if(encode_size_of_MS(encInfo) == e_failure)
    {
        printf(RED"Error: Unable to encode magic string size.\n"RESET);
        return e_failure;
    }
    if(encode_magic_string(encInfo->magic_string,encInfo) == e_failure)
    {
        printf(RED"Error: Unable to encode magic string.\n"RESET);
        return e_failure;
    }
    
    if(encode_secret_file_extn_size(encInfo) == e_failure)
    {
        printf(RED"Error: Unable to encode extension size.\n"RESET);
        return e_failure;
        
    }
    
    if(encode_secret_file_extn(encInfo) == e_failure)
    {
        printf(RED"Error: Unable to encode extension.\n"RESET);
        return e_failure;
    }
    
    if(encode_secret_file_size_to_image(encInfo)== e_failure)
    {
        printf(RED"Error: Unable to get secret file size.\n"RESET);
        return e_failure;
        
    }
    
    if(encode_secret_file_data(encInfo) == e_failure)
    {
        printf(RED"Error: Unable to encode secret file data.\n"RESET);
        return e_failure;
    }
    
    if(copy_remaining_img_data(encInfo->fptr_src_image, 
        encInfo->fptr_stego_image) == e_failure)
    {
        printf(RED"Error: Unable to copy remaining image data.\n"RESET);
        return e_failure;
            
    }

    if(close_encode_files(encInfo) == e_failure)
    {
        return e_failure;
    }
    
    return e_success;
}
