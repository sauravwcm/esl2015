#include <stdio.h>
#include <stdlib.h>
#include "arm_neon.h"

#define ROW1 32
#define COL1 32
#define ROW2 32
#define COL2 32

/*void add3 (uint8x16x2_t *data1, uint8x16x2_t *data2)
{
	int i;
	static uint8_t a[COL2], b[COL2];

	vst2q_u8 (a, *data1);
	vst2q_u8 (b, *data2);
    	data2 = vaddq_u8 (*data1, *data2);
}
*/
void print_uint8 (uint8x16x2_t data, char* name)
{
    int i;
    static uint8_t p[COL2];

    vst2q_u8 (p, data);

    printf ("%s = ", name);
    for (i = 0; i < COL2; i++)
    {
        printf ("%02d ", p[i]);
    }
    printf ("\n");
}

int main ()
{	int r=0, c=0, i=0;
	int k=0, l=0, m=0;

    /* Create custom arbitrary data. */
   	uint8_t row1[COL2];
	uint8_t row2[COL2];
    	for(i=0; i<COL2; i++)
	{	
		row1[i] = 0;
		row2[i] = 0;
	}
	uint8_t mat1[ROW1][COL1];
	uint8_t mat2[ROW2][COL2];
	
	for(r=0; r<ROW1; r++)
		for(c=0; c<COL1; c++)
		{
			mat1[r][c] = 1;
		}
	for(r=0; r<ROW2; r++)
		for(c=0; c<COL2; c++)
		{
			mat2[r][c] = 1;
		}

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
	
/*	for(r=0; r<ROW1*ROW2;r++)
	{
		for(c=0;c<COL2;c++)
		{
			printf("%d ", res[r][c]);
    		}
		 printf("\n");
	}
*/	
        printf("\n\n\n\n\n");
 	uint8x16x2_t data1, data2;
	
	for(i=0; i<ROW1; i++)
	{	data2 = vld2q_u8 (row2);
		//print_uint8 (data2, "data2 \n");	
		for(r=0; r<ROW2; r++)
		{	
			
			for(l=0; l<COL2; l++)
			{
				row1[l] = res[r+(ROW2*i)][l];
			}
			data1 = vld2q_u8 (row1);
			//print_uint8 (data1, "data1 \n");
			//add3(&data1, &data2);
			data2.val[0] = vaddq_u8 (data1.val[0], data2.val[0]);
			data2.val[1] = vaddq_u8 (data1.val[1], data2.val[1]);
		}
		vst2q_u8 (row1, data2);
		for(c=0; c<COL2; c++)
		{
			Mat_Res[i][c] = row1[c];
		}
	}
	printf("Resultant Matrix \n \n");
	for(r=0; r<ROW1; r++)
	{
		for(c=0; c<COL2; c++)
		{
			printf("%d ", Mat_Res[r][c]);
		}
		printf("\n");
	}
    return 0;
}
