#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main(int argc, char *argv[]) {

    int binaryS[17];
    char response[5];
    char decToHex[16]={'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '\0'};
    int elevate=0,j=0;
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    
    // first, read the number
    signed short int input;
    fscanf(fp, "%hd", &input); // note how the 'h' modifier shortens the %d to half size

    // print bits; you will see this kind of for loop often in this assignment
    for ( int digit=16; 0<=digit; digit-- ) {
        bool char_val = 0b1 & input>>digit; // shift and mask
        binaryS[(16-digit)]=char_val;
    }

    for ( int i = 1; i < 17; i+=4)
    {
        //use this formula http://www.technosoft.ro/KB/index.php?/article/AA-15440/0/The-negative-numbers-representation-in-hex.html#:~:text=The%20hexadecimal%20value%20of%20a,the%20respective%20negative%20decimal%20number.
        elevate=binaryS[i]*pow(2,3)+binaryS[i+1]*pow(2,2)+binaryS[i+2]*pow(2,1)+binaryS[i+3]*pow(2,0);
        if(elevate!=0){
            response[j]=decToHex[elevate-1];
        }
        else{
            response[j]='0';
        }
        j++;
    }
    printf("%s\n",response);
    return EXIT_SUCCESS;

}
