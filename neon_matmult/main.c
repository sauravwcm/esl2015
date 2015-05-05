#include <stdio.h>
#include <stdlib.h>
#include "arm_neon.h"

#define ROW1 2
#define COL1 4
#define ROW2 4
#define COL2 3

void add3 (uint8x16_t *data1, uint8x16_t *data2)
{
    /* Set each sixteen values of the vector to 3.
    *
    * Remark: a 'q' suffix to intrinsics indicates
    * the instruction run for 128 bits registers.
    */
    //uint8xCOL2_t zero = vmovq_n_u8 (3);
   
    /* Add 3 to the value given in argument. */
    *data2 = vaddq_u8 (*data1, *data2);

}

void print_uint8 (uint8x16_t data, char* name)
{
    int i;
    static uint8_t p[COL2];

    vst1q_u8 (p, data);

    printf ("%s = ", name);
    for (i = 0; i < COL2; i++)
    {
        printf ("%02d ", p[i]);
    }
    printf ("\n");
}

int main ()
{	int r=0, c=0, i=0;
	int k=0, l=0;

    /* Create custom arbitrary data. */
   	uint8_t row1[COL2];
	uint8_t row2[COL2];
    	for(i=0; i<COL2; i++)
	{	
		row1[i] = 0;
		row2[i] = 0;
	}
	const uint8_t mat1[ROW1][COL1] = { {1, 1, 1, 1}, {2, 2, 2, 2}};
	const uint8_t mat2[ROW2][COL2] = { {1, 1, 2}, {2, 1, 2}, {3, 1, 2}, {4, 1, 2}};
	int res[ROW1*ROW2][COL2];
	int Mat_Res[ROW1][COL2];

	for(r=0; r<ROW1; r++)
		{
			for(c=0; c<COL1; c++)
		     	{
				for(l=0; l<COL2; l++)
			 	{
					res[k][l] = mat1[r][c]	* mat2[c][l];
				}
				k++;
			}
		}
	
	for(r=0; r<ROW1*ROW2;r++)
	{
		for(c=0;c<COL2;c++)
		{
			printf("%d ", res[r][c]);
    		}
		 printf("\n");
	}
	
        printf("\n\n\n\n\n");
 	uint8x16_t data1, data2;
	
	for(i=0; i<ROW1; i++)
	{	data2 = vld1q_u8 (row2);	
		for(r=0; r<ROW2; r++)
		{	
			
			for(l=0; l<COL2; l++)
			{
				row1[l] = res[r+(ROW2*i)][l];
			}
			data1 = vld1q_u8 (row1);
			//print_uint8 (data1, "data1 \n");
			add3(&data1, &data2);
			//printf("\n \n \n \n");
			//print_uint8 (data2, "data2 \n");
		}
		vst1q_u8 (row1, data2);
		for(c=0; c<COL2; c++)
		{
			Mat_Res[i][c] = row1[c];
		}

		//print_uint8 (data2, "data \n");
	}
	printf("Result Matrix \n \n");
	for(r=0; r<ROW1; r++)
	{
		for(c=0; c<COL2; c++)
		{
			printf("%d ", Mat_Res[r][c]);
		}
		printf("\n");
	}
	
/*	for(r=ROW2;r<=k;r= r+ROW2)
	{	for(i=0; i<COL2; i++)
		{
			row1[i] = res[r-3][i];
			row2[i] = res[r-2][i];
			
		}
		//printf("%d \n", row1[0]);
		//printf("%d \n", row2[0]);
		data1 = vld1q_u8 (row1);
    		data2 = vld1q_u8 (row2);
    		add3(&data1, &data2);
    		for(i=0; i<COL2; i++)
		{
			row2[i] = res[r-1][i];
			
		}
		data2 = vld1q_u8 (row2);
		add3(&data1, &data2);
    		print_uint8 (data1, "data \n");
    	}
*/
    return 0;
}
