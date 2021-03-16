#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP
    
    // first, read the minuend (number to be subtracted from)
    bool minuend[8]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=7; 0<=i; i--) { // read MSB first as that is what comes first in the file
        //Transforming the character read from stream to int and then to bool 
        minuend[i] = (bool) (fgetc(fp) - '0');
    }
    
    fgetc(fp);

    // second, read the subtrahend (number to subtract)
    bool subtrahend[8]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
    for (int i=7; 0<=i; i--) { // read MSB first as that is what comes first in the file
        subtrahend[i] = (bool) (fgetc(fp) - '0');
    }

    // WE WILL DO SUBTRACTION BY NEGATING THE SUBTRAHEND AND ADD THAT TO THE MINUEND

    // Negate the subtrahend
    // flip all bits
    for(int i=7;0<=i;i--) {
        subtrahend[i] = !subtrahend[i];
    }
    // add one
    bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
    bool difference[8];
    int result = 0;
    for (int i=0; i<8; i++) { // iterate from LSB to MSB
        result = minuend[i] + subtrahend[i] + carry;
        printf("%d %d %d %d\n",minuend[i],subtrahend[i],carry,result);
        if(result == 0){
            difference[i] = false;
            carry = false;
        }
        else if (result == 1) {
            difference[i] = true;
            carry = false;
        } else if (result == 2) {
            difference[i] = false;
            carry = true;
        } else{
            difference[i] = true;
            carry = true;
        }
    }
    // print the difference bit string
    for (int i=7; 0<=i; i--)
        printf("%d",minuend[i]);
    printf("\n");
    for (int i=7; 0<=i; i--)
        printf("%d",subtrahend[i]);
    printf("\n");
    for (int i=7; 0<=i; i--)
        printf("%d",difference[i]);
    

    return EXIT_SUCCESS;

}
