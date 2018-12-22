#include <stdio.h>
#include <stdlib.h>

int crc_for_5g(int * crc, int * input_bits, int input_length, int crc_type)
{
    int crc_polynomial_24a[25] = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1};
    int crc_polynomial_24b[25] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1};
    int crc_polynomial_24c[25] = {1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1};
    int crc_polynomial_16[17]  = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    int crc_polynomial_11[12]  = {1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    int crc_polynomial_6[7]    = {1, 1, 0, 0, 0, 0, 1};
    int * crc_polynomial;
    
    int crc_length;

    int * temp_list;
    
    int ii;

    int step = 0;
    int position = 0;
    int first_one_meet_flag = 0;
    int accumulated_step = 0;
    
    switch(crc_type)
    {
        case 0:       /* 24A */
            crc_polynomial = crc_polynomial_24a;
            crc_length = 24;
            break;
        case 1:       /* 24B */
            crc_polynomial = crc_polynomial_24b;
            crc_length = 24;
            break;
        case 2:       /* 24C */
            crc_polynomial = crc_polynomial_24c;
            crc_length = 24;
            break;
        case 3:       /* 16 */
            crc_polynomial = crc_polynomial_16;
            crc_length = 16;
            break;
        case 4:       /* 11 */
            crc_polynomial = crc_polynomial_11;
            crc_length = 11;
            break;
        case 5:       /* 6 */
            crc_polynomial = crc_polynomial_6;
            crc_length = 6;
            break;
    }
    
    temp_list = (int *) malloc(sizeof(int) * (input_length + crc_length));
    
    for(ii=0; ii<input_length; ii++)
    {
        temp_list[ii] = input_bits[ii];
    }
    
    for(ii=input_length; ii<input_length+crc_length; ii++)
    {
        temp_list[ii] = 0;
    }

    while(1)
    {    
        for (ii = 0; ii < crc_length+1; ii++)
        {
            temp_list[ii + position] = temp_list[ii + position] ^ crc_polynomial[ii];
            if (temp_list[ii + position] == 0 && !first_one_meet_flag)
            {
                step++;
            }
            else
            {
                if (temp_list[ii+position] == 1 && !first_one_meet_flag)
                    first_one_meet_flag = 1;
            }
        }
 
        accumulated_step += step;

        /* printf("accumulated step is %d\n", accumulated_step); */

        if (accumulated_step >= input_length)
            break;

        position = position + step;
        step = 0;
        first_one_meet_flag = 0;
    }

    for (ii=0; ii<crc_length; ii++)
        crc[ii] = temp_list[ii+input_length];

    free(temp_list);

    return crc_length;
}

int main(int argc, char ** argv)
{
    int input_bits[1000];
    int crc[24];
    int index;
    int crc_type = 5;
    int crc_length;

    for(index=0; index < 1000; index++)
        input_bits[index] = 1;
    
    if (argc > 1)
        crc_type = atoi(argv[1]);

    if ( !((crc_type == 0) || (crc_type == 1) || (crc_type == 2) || (crc_type == 3) || (crc_type == 4) || (crc_type == 5)) )
    {
        fprintf(stderr, "invalid crc type. Please input a crc type between 0 and 5.\n");
        return 1;
    }
    
    crc_length = crc_for_5g(crc, input_bits, 1000, crc_type);

    for(index=0; index < crc_length; index++)
        printf("%d ", crc[index]);

    printf("\n");

    return 0;
}
