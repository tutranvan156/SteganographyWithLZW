#include <stdio.h>

////decode function
int LSB_Decode(FILE *pf1, FILE *pf2);
/**
 * This block of code using to declare interface
 */
void size_decryption(FILE *pf1, int *size_txt);

///* decryption of secret message*/
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2);

/**
 * End of declare interface
 */
void size_decryption(FILE *pf1, int *size_txt) {
    //we are using this function to get the size of the message
    //pf1 is the pointer that go direct to 55'th position
    int file_buff = 0, i;
    int ch, bit_msg;
    //todo: I change this line from 8 to 16
    for (i = 0; i < 8; i++) {
        //get each character of this destination file
        ch = fgetc(pf1);
        //then get the least significant bit of this character
        bit_msg = (ch & 1);
        //let's assume that if bit_msg == 1 then we just need to
        if (bit_msg) {
            //if file_buff = x, then below command make: x * 2 + 1
            file_buff = (file_buff << 1) | 1;
        } else {
            //if file_buff = x, then below command make: x * 2
            file_buff = file_buff << 1;
        }
    }
    //then just give this value for size_txt
    *size_txt = file_buff;
}

/* decryption of secret message*/
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2) {
    printf("size of message is %d", size_txt);
    int file_buff = 0, i, j = 0, k = 0;
    int ch, bit_msg;
    char output[250] = {0};
    for (i = 0; i < (size_txt * 8); i++) {
        j++;
        ch = fgetc(pf1);
        bit_msg = (ch & 1);
        if (bit_msg) {
            file_buff = (file_buff << 1) | 1;
        } else {
            file_buff = file_buff << 1;
        }

        if (j == 8) {
//            printf("File buff %c", file_buff);
//            putc(file_buff, pf2);

            putc(file_buff, pf2);
            printf("File buff %c", file_buff);

            output[k++] = file_buff;
            j = 0;
            file_buff = 0;
        }
    }

    printf("\n*** Secret Text Is ==> %s\n\n", output);
}

int LSB_Decode(FILE *pf1,  FILE *pf2) {
    //pf1 is the destination image
    //pf2 is the destination message

    int size, size1, size_txt;

    //opening Image File

    //just go to 54'th byte because first 54 byte is the same with origin file
    fseek(pf1, 54, SEEK_SET);


    /*Secret Text */
    //we will get the size of this message
    size_decryption(pf1, &size_txt);

    //then base on this size of message we can get whole message
    secret_decryption(size_txt, pf1, pf2);

    /* closing files */
//    fclose(pf1);
//    fclose(pf2);
    //surprise mtfk

}
