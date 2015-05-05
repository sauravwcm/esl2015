#include <stdio.h>
#include <stdlib.h>
#include "arm_neon.h"
#define ROW 3
#define COL 3


void add3 (uint8x16_t *data1, uint8x16_t *data2)
{
    /* Set each sixteen values of the vector to 3.
    *
    * Remark: a 'q' suffix to intrinsics indicates
    * the instruction run for 128 bits registers.
    */
   // uint8x16_t three = vmovq_n_u8 (3);
   
    /* Add 3 to the value given in argument. */
    *data1 = vaddq_u8 (*data1, *data2);
}

void print_uint8 (uint8x16_t data, char* name)
{
    int i;
    static uint8_t p[COL];

    vst1q_u8 (p, data);

    printf ("%s = ", name);
    for (i = 0; i < COL; i++)
    {
        printf ("%02d ", p[i]);
    }
    printf ("\n");
}

int main ()
{
    /* Create custom arbitrary data. */
   	uint8_t row1[COL];
	uint8_t row2[COL];
    
	const uint8_t mat1[ROW][COL] = { {1, 2, 3}, {2, 4, 6}, {1, 2, 3}};
	const uint8_t mat2[ROW][COL] = { {1, 1, 2}, {1, 2, 1}, {2, 1, 1}};
	int res[ROW*COL][COL];
	
	int r=0, c=0, i=0;
	int k=0, l=0;

	for(r=0; r<ROW; r++)
		{
			for(c=0; c<COL; c++)
		     	{
				for(l=0; l<COL; l++)
			 	{
					res[k][l] = mat1[r][c]	* mat2[c][l];
				}
				k++;
			}
		}
	
	for(r=0; r<ROW*COL;r++)
	{
		for(c=0;c<COL;c++)
		{
			printf("%d ", res[r][c]);
    		}
		 printf("\n");
	}
	
        printf("\n\n\n\n\n");
 	uint8x16_t data1, data2;

	for(r=3;r<=k;r= r+ROW)
	{	for(i=0; i<COL; i++)
		{
			row1[i] = res[r-3][i];
			row2[i] = res[r-2][i];
			
		}
		//printf("%d \n", row1[0]);
		//printf("%d \n", row2[0]);
		data1 = vld1q_u8 (row1);
    		data2 = vld1q_u8 (row2);
    		add3(&data1, &data2);
    		for(i=0; i<COL; i++)
		{
			row2[i] = res[r-1][i];
			
		}
		data2 = vld1q_u8 (row2);
		add3(&data1, &data2);
    		print_uint8 (data1, "data \n");
    	}
    return 0;
}
