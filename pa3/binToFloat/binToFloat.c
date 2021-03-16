#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) {
    int acum=7;
    float m=0;
    int e=0;
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of float point number
    int buffer[EXP_SZ+FRAC_SZ+1];
    
    for (int i=EXP_SZ+FRAC_SZ; 0<=i; i--) { // read MSB first as that is what comes first in the file
       buffer[(31-i)] = (bool) (fgetc(fp) - '0');
    }
    printf("\n");
    // float number = *(float *)&binary; // you are not allowed to do this.

    /* ... */

    // E
    /* ... */
    for (int i=1; i<9; i++) { // read MSB first as that is what comes first in the file
       e+=ldexp ( buffer[i], acum );
       acum--;   
    }
    // M
    /* ... */
    acum=1;
    for (int i=9; i<32; i++) { // read MSB first as that is what comes first in the file
       m+=ldexp ( buffer[i], -acum );
       acum++;
    }

    // https://www.tutorialspoint.com/c_standard_library/c_function_ldexp.htm
    double number = ldexp ( 1+m, e-127 );
    number = buffer[0] ? -number : number;
    printf("%e\n", number);

    return EXIT_SUCCESS;

}
