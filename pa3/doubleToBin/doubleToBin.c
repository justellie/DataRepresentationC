#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) {

    // SETUP
    int sign;
    int intpart = 0;
    int decimal[18];
    int exponent[11];
    int frac[52];
    int countE=17,countF=0,countZeros=0;
    int e=0;
    int pres=52;
    int i=0;
    double decpart = 0;
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);
    sign = value<0.0;
    
    // the reference solution ('the easy way')
    // you are not allowed to print from this casted 'ref_bits' variable below
    // but, it is helpful for validating your solution
    unsigned long int ref_bits = *(unsigned long int*) &value;

     
    // THE SIGN BIT
    printf("%d_", sign);
    assert ( sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ))); // validate your result against the reference

    // THE EXP FIELD
    // find the exponent E such that the fraction will be a canonical fraction:
    // 1.0 <= fraction < 2.0
    //111010

    //init
    value=sign ? -value : value;
    intpart = (int)value;
    decpart = value- intpart;
    for( i=0;i<18;i++)    
    {    
        decimal[i]=-1;
        frac[i]=-1;   
         
    }
    i=0;
    for(int i=0;i<52;i++)    
    {    
        frac[i]=-1;   
    }
    i=0;
    for (int i = 0; i < 11; i++)
    {
        exponent[i] = 0;
    }

    //treatment of the number
    int n=intpart;
    i=0;
    for( i=0;n>0;i++)    
    {    
        decimal[i]=n%2;    
        n=n/2;
    }

    double nf=decpart;
    while (pres--) 
    { 
        // Find next bit in fraction 
        nf *= 2; 
        long fract_bit = nf; 
  
        if (fract_bit == 1) 
        { 
            nf -= fract_bit; 
            frac[countF]=1;
            
        } 
        else
        {
            frac[countF]=0;
        }
        countF++; 
    } 

    //in case =0
    if (intpart != 0)
    {
        while (decimal[countE] != 1)
        {
            countE--;
        }
        e = 1023 + countE;
    }
    else
    {
        countF = 0;
        if (decpart != 0)
        {
            while (frac[countF] != 1)
            {
                countF++;
            }
            e = 1023 -(countF+1);
        }
    }
    i=0;
    for (i = 0; e > 0; i++)
    {
        exponent[i] = e % 2;
        e = e / 2;
    }
    i=10;
    for(i=10;i>-1;i--)    
    {   
        printf("%d",exponent[i]);
    }
    printf("_");

    //in number >.0
    if (intpart > 0)
    {   
        i=countE-1;
        for (i = countE-1; i > -1; i--)
        {
            printf("%d",decimal[i]);
            countZeros++;
        }
        if(frac[0]!=-1)
        {
            i=0;
            for (i = 0; i < (52-countZeros); i++)
            {
                printf("%d", frac[i]);
                
            }
            countZeros+=i;
        }
    }
    else
    {
        if (decpart != 0)
        {
            i=countF+1;
            for (i = countF+1; i < (52-countZeros); i++)
            {
                printf("%d", frac[i]);
                
            }
            countZeros+=i-(countF+1);
        }
    }
    i=countZeros;
    for (i = countZeros; i < 52; i++)
    {
         printf("%d", 0);
    }
    //printf("\n");
    
}
