#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

typedef int int_b;

bool equalsZero(int_b op[]) {
    bool is_zero = true;
    for(int i = 0; i < 32 && is_zero; i++) {
        if(op[i] != 0)
          is_zero=false;
    }
    return is_zero;
}

void binAdd(int_b op1[], int_b op2[], int_b res[], int size, int_b is_sub) { 
    //If we're substracting we implement add one logic
    int_b carry = is_sub;
    int result = 0;
    // iterate from LSB to MSB
    for (int i=0; i<size; i++) { 
        result = op1[i] + op2[i] + carry;
        if(result == 0){
            res[i] = false;
            carry = false;
        }
        else if (result == 1) {
            res[i] = true;
            carry = false;
        } else if (result == 2) {
            res[i] = false;
            carry = true;
        } else {
            res[i] = true;
            carry = true;
        }
    }
}

void binAddMul(int_b prod[], int_b op[], int start, int end) {
     int_b carry = false;
    int result = 0;
    // iterate from LSB to MSB
    for (int i=start,j=0; i<end; i++,j++) { 
        result = prod[i] + op[j] + carry;
        if(result == 0){
            prod[i] = false;
            carry = false;
        }
        else if (result == 1) {
            prod[i] = true;
            carry = false;
        } else if (result == 2) {
            prod[i] = false;
            carry = true;
        } else {
            prod[i] = true;
            carry = true;
        }
    }
}
int main(int argc, char *argv[]) {

    // float value = *(float *) &binary; // you are not allowed to do this.
    // unsigned int binary = *(unsigned int*) &value; // you are not allowed to do this.
    
    int_b multiplier[EXP_SZ+FRAC_SZ+1];
    int_b multiplicand[EXP_SZ+FRAC_SZ+1];

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of multiplier
    for(int i = 31; i>=0; i--) {
        multiplier[i] = (int_b) (fgetc(fp) - '0');
    }

    //Reading the newline character
    fgetc(fp);

    for(int i = 31; i>=0; i--) {
        multiplicand[i] = (int_b) (fgetc(fp) - '0');
    }

   
    //float product = *(float *) &multiplier * *(float *) &multiplicand; // you are not allowed to print from this.
    //unsigned int ref_bits = *(unsigned int *) &product; // you are not allowed to print from this. But you can use it to validate your solution.

    // SIGN
    int_b sign = multiplier[31]^multiplicand[31];

    //printf("%d_",sign);
    //assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ)));

    // EXP
    // get the exp field of the multiplier and multiplicand
    int_b exp_multiplier[EXP_SZ];
    int_b exp_multiplicand[EXP_SZ];
    int_b exp_res[EXP_SZ];

    for(int i = 30, j=7;i>=23;i--,j--) {
        exp_multiplicand[j] = multiplicand[i];
        exp_multiplier[j] = multiplier[i];
    }
    // add the two exp together
    binAdd(exp_multiplicand, exp_multiplier, exp_res, 8, false);
    // subtract bias
    //We substract the bias i.e 127. We store it in its one-complement form
    int_b bias[8] = {0,0,0,0,0,0,0,1};
    
    binAdd(exp_res, bias, exp_res, 8, true);
 
    // get the frac field of the multiplier and multiplicand
    int_b frac_multiplier[FRAC_SZ+1];
    int_b frac_multiplicand[FRAC_SZ+1];
    int_b frac_res[FRAC_SZ];
    for(int i=22, j=22; i>=0; i--,j--) {
        frac_multiplicand[j] = multiplicand[i];
        frac_multiplier[j] = multiplier[i];
    }
    
    // assuming that the input numbers are normalized floating point numbers, 
    //add back the implied leading 1 in the mantissa
    frac_multiplicand[FRAC_SZ] = 1;
    frac_multiplier[FRAC_SZ] = 1;
    

    //When we multiply we need double space to store the result
    int_b mul_res[(FRAC_SZ+1)*2];
    int ans_ind=0;
    int_b is_carry = true;
    int incr=1;
    for(int i = 0; i < (FRAC_SZ+1)*2; i++) {
        mul_res[i] = 0;
    }
    
    // multiply the mantissas
    for(int i = 0; i < FRAC_SZ+1; i++) {
        //Using Hennessy's algorithm. Multiply the current bit in multiplier with multiplicand
        ans_ind = i;
        for(int j = 0; j < FRAC_SZ+1; j++) {
            mul_res[i+j] = mul_res[ans_ind+j]+(frac_multiplicand[i]*frac_multiplier[j]);
            if(mul_res[ans_ind+j]>1) {
                is_carry = true;
                incr=j;
                while(is_carry) {
                    mul_res[ans_ind+incr+1]+=(mul_res[ans_ind+incr]/2);
                    mul_res[ans_ind+incr]%=2;
                    if(mul_res[ans_ind+incr] > 1) {
                        is_carry=true;
                        incr++;
                    } else {
                        is_carry=false;
                    }
                }
            }
        }
    } 
    // overflow and normalize
    int_b found_one = 0;
    int one_pos=-1;
    
    for(int i = FRAC_SZ-1; i>=0; i--) {
        frac_res[i] = 0;
    }
    
    //If one of the operands is zero we don't need to look for a onw
    if(equalsZero(multiplicand) || equalsZero(multiplier)) {
        printf("0_00000000_00000000000000000000000");
    } else { //We find the first one
        for(int i = 47; i>=0 && !found_one; i--)  {
            if(mul_res[i] == 1) {
                found_one = 1;
                one_pos = i;
            }        
        }
        //We copy from that one onwards
        if(found_one) {
            for(int i = one_pos-1,j=FRAC_SZ-1; j >= 0; i--,j--) {
                frac_res[j] = mul_res[i];
            }
        }

        //Normalizing
        int zeros_mcand = 0, zeros_mer = 0, zeros_res=0;
        for(int i = 0; i < FRAC_SZ+1; i++) {
            if(frac_multiplicand[i] == 0){
                zeros_mcand++;
            }
            else {
                break;
            }
        }
        
        for(int i = 0; i < FRAC_SZ+1; i++) {
            if(frac_multiplicand[i] == 0){
                zeros_mer++;
            }
            else {
                break;
            }
        }
        for(int i = 0; i < (FRAC_SZ+1)*2;i++) {
            if(frac_multiplicand[i] == 0){
                zeros_mer++;
            }
            else {
                break;
            }
        }
        printf("\nMul res:\n");
        for(int i = (FRAC_SZ+1)*2-1; i>=0; i--) {
            printf("%d",mul_res[i]);
            }
            printf("\n");
        // PRINTING
        // print exp
        printf("%d_",sign);
        for(int i = EXP_SZ-1; i>=0; i--) {
            printf("%d",exp_res[i]);
        }
        printf("_");
        for(int i = FRAC_SZ-1; i>=0; i--) {
            printf("%d",frac_res[i]);
        }
    }
    
   /*

    for ( int bit_index=EXP_SZ-1; 0<=bit_index; bit_index-- ) {
        int_b trial_bit = 1&exp>>bit_index;
        printf("%d",trial_bit);
        assert (trial_bit == (1&ref_bits>>(bit_index+FRAC_SZ)));
    }
    printf("_");

    // print frac
    for ( int bit_index=FRAC_SZ-1; 0<=bit_index; bit_index-- ) {
        int_b trial_bit = 1&frac>>bit_index;
        printf("%d",trial_bit);
        assert (trial_bit == (1&ref_bits>>(bit_index)));
    }
    */
    return(EXIT_SUCCESS);

}


/*
 for(int i = 31; i>=0; i--) {
        printf("%d",multiplier[i]);
    }
    printf("\n");


    for(int i = 31; i>=0; i--) {
        printf("%d",multiplicand[i]);
*/

 /*
    printf("\n");
    for(int i = 7; i >= 0; i--) {
        printf("%d",exp_res[i]);
    }
    printf("\n\n");
    
    printf("\n\n");
    for(int i = 7; i >= 0; i--) {
        printf("%d",exp_multiplier[i]);
    }
    printf("\n\n");
    for(int i = 31; i>=0; i--) {
        printf("%d",multiplier[i]);
    }
    printf("\n\n");
    for(int i = 31; i>=0; i--) {
        printf("%d",multiplicand[i]);
    }
    printf("\n\n");



       for(int i = FRAC_SZ; i>=0; i--) {
        printf("%d",frac_multiplier[i]);
    }
    printf("\n\n");
    for(int i = FRAC_SZ; i>=0; i--) {
        printf("%d",frac_multiplicand[i]);
    }
    printf("\n\n");

    printf("\nMul res:\n");
    for(int i = (FRAC_SZ+1)*2-1; i>=0; i--) {
        printf("%d",mul_res[i]);
    }

     printf("\nfrac res:\n");
    for(int i = FRAC_SZ-1; i>=0; i--) {
        printf("%d",frac_res[i]);
    }
    printf("\n\n");

    printf("\nMul res:\n");
    for(int i = (FRAC_SZ+1)*2-1; i>=0; i--) {
        printf("%d",mul_res[i]);
    }
    printf("\n\n");
    
*/