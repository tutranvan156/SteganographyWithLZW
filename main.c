#include <stdio.h>
#include "algorithms.c"
#include "LSB_Encode.c"
#include "LSB_Decode.c"
#include <stdbool.h>

int main(int argc, char *argv[]) {
    //Function

    bool isExit = false;

    //switch choice Encode or Decode
    while (!isExit) {
        printf("LZW Encoding\n");
        printf("1. Encode\n");
        printf("2. Decode\n");
        int choice;
        char input_image[10000], output_image[10000], secret_text[10000], output_image2[10000];
        fflush(stdin);
        scanf("%d", &choice);
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
                FILE *secretMessage = fopen(secret_text, "r");
                FILE *outputImage = fopen(output_image, "wb+");
                FILE *compressMessage = fopen("D://compressMessage.txt", "wb+"); // binary write to output file
                /**
                 * After compress, then we have compressMessage file have compressed message
                 * Then we will embed this message into image
                 */
                compress(secretMessage, compressMessage);
                LSB_Encode(inputImage, compressMessage, outputImage);
                fclose(inputImage);
                fclose(secretMessage);
                fclose(outputImage);
                fclose(compressMessage);
                break;
            case 2:
                printf("Input your input file: ");
                scanf("%s", output_image2);
                FILE *decodeInputImage = fopen(output_image2, "rb+");
                /**
                 * We meet problem at beforeDecompress, this file does not change
                 */
                FILE *decodeSecretText = fopen("D://beforeDecompress.txt", "w");
                LSB_Decode(decodeInputImage, decodeSecretText);
                fclose(decodeInputImage);
                fclose(decodeSecretText);
                /**
                 * Secret text after LSB_Decode is correct
                 */
                FILE *file1 = fopen("D://beforeDecompress.txt", "r");
                FILE *result = fopen("D://result.txt", "w");
                decompress(file1, result);
                fflush(stdin);
                fclose(file1);
                fclose(result);
                break;
            default :
                printf("\t*** !! Error !! ERROR !! Error !!***\n\n");
                printf("*** EXECUTION ==> ");
        }
    }
    return 0;
}
