#include <stdio.h>
#include "algorithms.c"
#include "LSB_Encode.c"
#include "LSB_Decode.c"

int main(int argc, char *argv[]) {
    //Function
    printf("LZW Encoding\n");
    printf("1. Encode\n");
    printf("2. Decode\n");
    int choice;
    scanf("%d", &choice);
    char input_image[10000], output_image[10000], secret_text[10000];
    fflush(stdin);

    //switch choice Encode or Decode
    switch (choice) {
        case 1:
            printf("Input your target file link: ");
            scanf("%s", input_image);
            fflush(stdin);
            printf("Input your secret text file link: ");
            scanf("%s", secret_text);
            fflush(stdin);
            printf("Input name of output file: ");
            scanf("%s", output_image);
            fflush(stdin);

            FILE *inputImage = fopen(input_image, "rb+");
            FILE *secretMessage = fopen(secret_text, "rb+");
            FILE *outputImage = fopen(output_image, "wb+");
            FILE *compressMessage = fopen("D://compressMessage.txt", "rwb+"); // binary write to output file
            /**
             * After compress, then we have compressMessage file have compressed message
             * Then we will embed this message into image
             */
            compress(secretMessage, compressMessage);
            LSB_Encode(inputImage, compressMessage, outputImage);
            break;
        case 2:
            printf("Input your input file: ");
            scanf("%s", output_image);
            fflush(stdin);
            FILE *decodeInputImage = fopen(output_image, "r");
            FILE *decodeSecretText = fopen("D://beforeDecompress.txt", "rwb");
            FILE *decodeAfterDecompress = fopen("D://result.txt", "w");
            LSB_Decode(decodeInputImage, decodeSecretText);
            decompress(decodeSecretText, decodeAfterDecompress);
            fclose(decodeInputImage);
            fclose(decodeSecretText);
            fclose(decodeAfterDecompress);
            break;
        default :
            printf("\t*** !! Error !! ERROR !! Error !!***\n\n");
            printf("*** EXECUTION ==> ");
            printf("\n\t\t*** ENCODING *** ==> \t./a.out -E -i <input.bmp> -s <secret.txt> -o <output.bmp>\n\n");
            printf("\t\t*** DECODING *** ==> \t./a.out -D -i <output.bmp> -o <output_text.txt>\n\n");
            printf("\t\tFOR MORE DETAILS ===> **README**\n");
    }
    return 0;
}
