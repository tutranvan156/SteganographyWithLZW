
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * This block of code using to declare interface
 */
//Encryption function
void LSB_Encode(FILE *fp1, FILE *fp2, FILE *fp3);
//
int size_of_image(FILE *fp1);
//
////Size Of The Secret Message
int secret_text_size(FILE *fp2);
//
////bits of text file
int get_bit(char byte, int bit);
//
////encryption of message
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3);
//
////Encryption For Numbers
void size_encrypt(int num,FILE *fp1, FILE *fp3);
/**
 * End of declare interface
 */

int size_of_image(FILE *fp1) {
    int width, height;

    fseek(fp1, 0x12, SEEK_SET);    //Seek to the Width position
    fread(&width, sizeof(int), 1, fp1);    //Reading Width
    fread(&height, sizeof(int), 1, fp1);    //Read height
    printf("Dimensions of the Image is %d x %d \n", width, height);
    fseek(fp1, 0L, SEEK_SET);    //Seek to the start position

    return ((width * height * 3) / 8);
}

//Size Of The Secret Message
/**
 * This function using to get size of secret message
 * @param fp2 is compressMessage
 * @return
 */
int secret_text_size(FILE *fp2) {
//    int counter = 0;
//    char tempChar;
//    while ((tempChar = fgetc(fp2)) != EOF) {
//        counter++;
//    }
//    printf("number of word in counter is %d", counter);
//    return counter;


    int size_txt;
    fseek(fp2, 0L, SEEK_END);
    size_txt = ftell(fp2);
    fseek(fp2, 0L, SEEK_SET);
    return size_txt;
}

//bits of text file
int get_bit(char byte, int bit) {
    return ((byte >> 8 - bit) & 1);
}

//encryption of message
/**
 *
 * @param fp1 fp1 is inputImage
 * @param fp2 fp2 is inputSecret
 * @param fp3 fp3 is outputImage
 */
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3) {
    //msg_buff is for secret message
    //file_buff is for target file
    char file_buff = 0, msg_buff = 0, ch;    //temp var for one byte from file and msg
    int i;
    int bit_msg;
    int counter = 0;
    while ((msg_buff = fgetc(fp2)) != EOF) {
        printf("%c", msg_buff);
        counter++;
        for (i = 1; i <= 8; i++) {
            file_buff = fgetc(fp1);

            int file_byte_lsb = (file_buff & 1);

            bit_msg = get_bit(msg_buff, i);

            if (file_byte_lsb == bit_msg) {
                fputc(file_buff, fp3);
            } else {
                if (file_byte_lsb == 0)
                    file_buff = (file_buff | 1);
                else
                    file_buff = (file_buff ^ 1);

                fputc(file_buff, fp3);
            }
        }
    }
    printf("Number of word %d", counter);
    /*copying rest of data */
    while (!feof(fp1)) {
        char tmp_cpy = fgetc(fp1);
        fputc(tmp_cpy, fp3);

    }

    if (msg_buff == EOF)
        printf("\n*** Secret Message Encrypted Successfully ***\n");
    else
        printf("\n*** Failed Encrypting ***\n");
}

//Encryption For Numbers
void size_encrypt(int num, FILE *fp1, FILE *fp3) {

    char file_buff;
    int i, j = 0;
    int bit_msg;

    //todo: I think this line must be 16
    for (i = 1; i <= 16; i++) {
        //file_buff store
        file_buff = fgetc(fp1);

        //get the least significant bit in file_buff
        int file_byte_lsb = (file_buff & 1);

        bit_msg = get_bit(num, i);

        if (file_byte_lsb == bit_msg) {
            //if equal then just put this file_buff value in to fp3
            fputc(file_buff, fp3);
        } else {
            //similar with stega_encryption above
            if (file_byte_lsb == 0)
                file_buff = (file_buff | 1);
            else
                file_buff = (file_buff ^ 1);

            fputc(file_buff, fp3);
        }
    }

}

/**
 * This function using to embed
 * @param fp1 is target file Image
 * @param fp2 is secret text that want to embed
 * @param fp3 is destination file Image
 */
void LSB_Encode(FILE *fp1, FILE *fp2, FILE *fp3) {

    /**
     * This function using to calculate how many character can be stored
     */
    int size_image = size_of_image(fp1);
    printf("Total %d Characters can be stored.\n", size_image);

    /**
     * This function using to write secret message into secret file text
     * But I think it is not necessary
     * So I will comment it out
	    //Entering secret text to file
	    printf("Enter your secret text and Press CTRL + D To Stop : \t");
	    secret_text(fp2);
     */


    /**
     * This function using to calculate size of secret text
     */
    int size_txt = secret_text_size(fp2);
    printf("%d", size_txt);
    printf("\nSize of The Secret Message is ==> %d\n", size_txt);

    //Comparing Image Size With Text
    if (size_image < size_txt) {
        printf("\n*** The Size Of Message Exceeds the Size of image ***\n");
        return;
    }

    if (fp3 == NULL) {
        fprintf(stderr, "Cannot create output file %s\n");
        exit(1);
    }

    //copying 54 header file
    int i, c = 0;
    char tmp_cpy;
    rewind(fp1);    //Goes back to the beginning of the file
    //first 54 byte there are
    //14 first byte using to identify that this file is BMP
    //40 byte is using to store some information help show image
    for (i = 0; i < 54; i++) {
        tmp_cpy = fgetc(fp1);
        fputc(tmp_cpy, fp3);
        c++;
    }

    //checking 54 bytes copied or not
    if (i == c)
        printf("\n*** Successfully copied 54 header bytes ***\n");
    else {
        printf("\n*** Failed to copy 54 header bytes ***\n");
        return;
    }

    /**
     * After copy 54 bit from fp1 to fp3 then we will embed size of secret text into fp3
     */
    size_encrypt(size_txt, fp1, fp3);

    //final we will encrypt the secret message
    stega_encrypt(fp1, fp2, fp3);

    /*Clean up before exit */
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return;

}
