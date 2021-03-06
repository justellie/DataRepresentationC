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

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);

    // the reference solution ('the easy way')
    // you are not allowed to print from this casted 'ref_bits' variable below
    // but, it is helpful for validating your solution
    //unsigned long int ref_bits = *(unsigned long int*) &value;

    // THE SIGN BIT
    bool sign = value<0;
    
    printf("%d_",sign);
   // assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ))); // validate your result against the reference
    
    // THE EXP FIELD
    // find the exponent E such that the fraction will be a canonical fraction:
    // 1.0 <= fraction < 2.0
    double fraction = sign ? -value : value;
    double nf=fraction;
    int trial_exp=(1<<(EXP_SZ-1)); // start by assuming largest possible exp (before infinity)
    // do trial division until the proper exponent E is found
    /* ... */
    unsigned short bias = (1<<(EXP_SZ-1))-1;
    signed short exp=0;
    if(fraction>1e-308)
    {
        //https://www.tutorialspoint.com/c_standard_library/c_function_frexp.htm
        frexp(fraction,&trial_exp);
            
        exp = (trial_exp-1) + bias;
    }
    
    for ( int exp_index=EXP_SZ-1; 0<=exp_index; exp_index-- ) {
        bool exp_bit = 1&exp>>exp_index;
        printf("%d",exp_bit);
        //assert (exp_bit == (1&ref_bits>>(exp_index+FRAC_SZ))); // validate your result against the reference
    }
    printf("_");
    // you get partial credit by getting the exp field correct
    // even if you print zeros for the frac field next
    // you should be able to pass test cases 0, 1, 2, 3

    // THE FRAC FIELD
    // prepare the canonical fraction such that:
    // 1.0 <= fraction < 2.0
    /* ... */
    double intpart ;
    int fract_bit;
    nf = fraction;
    if ((fabs(nf-2) < 1e-308) ||(fabs(nf-1) < 1e-308) ||( nf > 1 && nf > 2))
    {
       nf= ldexp ( nf, -(trial_exp-1) );
       
    }
    else
    {
        if(trial_exp!=0)
        {
            
            if (trial_exp==1024)
            {
                nf= ldexp ( nf, -(-(trial_exp-2)) );
            }
            else
            {
                nf= ldexp ( nf, -(trial_exp-1) );
            }
            
        }
        else
        {
            nf=0;
        }
    }
    nf=modf(nf, &intpart);


    bool frac_array[FRAC_SZ+1]; // one extra LSB bit for rounding
    for ( int frac_index=FRAC_SZ; 0<=frac_index; frac_index-- ) {
        
        nf *= 2;
        fract_bit = nf; 
        frac_array[frac_index] = fract_bit; // frac set to zero to enable partial credit
        if (fract_bit)
        {
            nf -= fract_bit;
        }   
    }

    // rounding
    /* ... */

    for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {
        bool frac_bit = frac_array[frac_index+1]; // skipping the extra LSB bit for rounding
        printf("%d", frac_bit);
        // assert (frac_bit == (1&ref_bits>>frac_index)); // validate your result against the reference
    }

}