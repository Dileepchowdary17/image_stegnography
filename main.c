#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if(strstr(argv[1],"-e")){
        return e_encode;
    }
    else if(strstr(argv[1],"-d")){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("For encoding Use: %s -e source.bmp secret_file [stego.bmp]\n", argv[0]);
        return e_failure;
    }

    OperationType res =  check_operation_type(argv);

    if(res == e_encode)
    {
        printf("You choosed encoding.\n");

        if(argc < 4)
        {
            printf("For encoding Use: %s -e source.bmp secret_file [stego.bmp]\n", argv[0]);
            return e_failure;
        }
        
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv,&encInfo) == e_success)
        {
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding completed successfull.\n");
            }

            else
            {
                printf("Encoding failed.\n");
                return e_failure;
            }
        }
        else
        {
            printf("Read and validation is unsuccessfull.\n");
            return e_failure;
        }
    }
    else if(res == e_decode)
    {
        printf("You choosed decoding.\n");
        if(argc < 4){
            printf("For decoding Use: %s -d stego.bmp output\n", argv[0]);
            return e_failure;
        }
        
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            if(do_decoding(&decInfo) == e_success)
            {
                printf("Decoding completed successfully.\n");
            }

            else{
                printf("Decoding failed.\n");
                return e_failure;
            }
        }
        
        else
        {
            printf("Read and validation is unsuccessful.\n");
            return e_failure;
        }

    }
    else
    {
        printf("Invalid input\n");
        printf("./a.out -e source.bmp secret_file stego.bmp\n");
        printf("./a.out -d filename1.bmp output");

        return e_failure;
    }
}
