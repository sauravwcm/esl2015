#include <stdio.h>
#include <stdlib.h>
#include "arm_neon.h"

#define ROW1 64
#define COL1 128
#define ROW2 128
#define COL2 64

void Timer_Init(void);
void Timer_DeInit(void);
long HiResTime(void);


int main ()
{	int r, c, i, j;
	uint16_t mat1[ROW1][COL1], mat2[ROW2][COL2];
	uint16_t res[ROW1][COL2];
	uint16x8_t vec1, vec2;
	uint16_t row[8], col[8];
	uint8_t count=0, step=0, rem=0, que=0;
	
	long tStart, tEnd;
	Timer_Init();

	for(r=0; r<ROW1; r++)
		for(c=0; c<COL1; c++)
			mat1[r][c] = 2;

	for(r=0; r<ROW2; r++)
		for(c=0; c<COL2; c++)
			mat2[r][c] = 1;
	if(ROW2%8==0)
		step = ROW2/8;
	else
		step = (ROW2/8)+1;
	
	que = ROW2/8;
	rem = ROW2%8;

	tStart = HiResTime();
	for(i=0; i<ROW1;i++)
	{	for(j=0; j<COL2; j++)
		{	res[i][j] = 0;
			while(count<step)
			{
				//for(r=0; r<8; r++)
				//{
				//	row[r] = 0;
				//	col[r] = 0;
				//}
				vec1 = vmovq_n_u16 (0);
				//vec1.val[0] = vmovq_n_u8 (0);
				//vec1.val[1] = vmovq_n_u8 (0);
				
				vec2 = vmovq_n_u16 (0);
				//vec2.val[0] = vmovq_n_u8 (0);
				//vec2.val[1] = vmovq_n_u8 (0);
				if(step-count == 1)
				{	for(r=0; r<rem; r++)
					{	
						row[r]=mat1[i][r+count*8];
						col[r]=mat2[r+count*8][j];
						row[8-rem] = 0;
						col[8-rem] = 0;
					}
				}
				else
				{
					for(r=0; r<8; r++)
					{	
						row[r]=mat1[i][r+count*8];
						col[r]=mat2[r+count*8][j];
					}
				}
				vec1 = vld1q_u16 (row);
				vec2 = vld1q_u16 (col);
				vec1 = vmulq_u16(vec1, vec2);
				//vec1.val[0] = vmulq_u8(vec1.val[0], vec2.val[0]);
				//vec1.val[1] = vmulq_u8(vec1.val[1], vec2.val[1]);
			
				vst1q_u16 (row, vec1);

				for(r=0; r<8; r++)
				{
					res[i][j]+= row[r];
				}
				count++;
			}
			count=0;
		}
	}
	tEnd = HiResTime();
	printf("result matrix \n");

	for(i=0;i<ROW1;i++)
	{
		for(j=0;j<COL2;j++)
		printf("%d ", res[i][j]);

		printf("\n");
        }
	printf("\n Execution Time = %d \n", (tEnd - tStart) );
    	return 0;
}


//timer functions

void Timer_Init(void)
{
#if defined(__arm__)
    int user_enable;
#if defined(__GNUC__)
#define COMPILER_ID "GCC"
asm ("mrc p15, 0, %0, c9, c14, 0" : "=r"(user_enable));
#elif defined(__CC_ARM)
#define COMPILER_ID "ARMCC"
#error "ARMCC not implemented"
#else
#error  "Unknown ARM compiler"
#endif
    if (!user_enable)
        printf("User mode enable is not set, no access to cycle counter\n");

#if defined(__GNUC__)
    /* Reset and enable */
    asm volatile ("mcr p15, 0, %0, c9, c12, 0" :: "r"(5));
    /* Enable cycle counter */
    asm volatile ("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x80000000));
    /* Reset overflow flags */
    asm volatile ("mcr p15, 0, %0, c9, c12, 3" :: "r"(0x80000000));
#endif

#else
    printf("No support for RDTSC on this CPU platform\n");
#endif /* defined(__arm__) */
}

void Timer_DeInit(void)
{
#if defined(__GNUC__)
    /* Disable */
    asm volatile ("mcr p15, 0, %0, c9, c12, 0" :: "r"(0));
#endif
}

long HiResTime(void)
{
    long cycles;
#if defined(__arm__)
#if defined(__GNUC__)
asm volatile ("mrc p15, 0, %0, c9, c13, 0": "=r"(cycles));
#endif
    return cycles;
#else
    return 0;
#endif
}
