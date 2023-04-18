/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define REPEAT 1
#define LENA 6
#define NROLL 2


void IUJK(int a[LENA][LENA],int b[LENA][LENA],int c[LENA][LENA])
{
  int i,j,k,jhi;

  jhi=(LENA/NROLL)*NROLL;
  jhi--;

  for(i=0; i< LENA;i++)
  {
	  for(j=0;j<LENA;j++)
	  {
		a[i][j]=0;
		b[i][j]=1;
		c[i][j]=1;
	  }
	  }

  for (i=0;i <LENA; i++)
    for (j=0; j < jhi;j+=NROLL)
       for (k=0; k < LENA; k++)
       {
         a[i][k]=a[i][k]+b[i][j]*c[j][k];
         a[i][k]=a[i][k]+b[i][j+1]*c[j+1][k];
         //a[i][k]=a[i][k]+b[i][j+2]*c[j+2][k];
         //a[i][k]=a[i][k]+b[i][j+3]*c[j+3][k];
       }
  for (i=0; i < LENA; i++)
    for (j=jhi+1;j < LENA;j++)
       for (k=0; k < LENA; k++)
         a[i][k]=a[i][k]+b[i][j]*c[j][k];

  return;
}

int main() {


 int cnt1=0;
 int cnt2=0;
 int sum=0;
 int result[5]={0,};
 int base[5]={0,};
 int ia[LENA][LENA];
 int ib[LENA][LENA],ic[LENA][LENA];

 struct metal_cpu *cpu;
 /* Get CPU device hsrale. */
 cpu = metal_cpu_get(metal_cpu_get_current_hartid());

 /* Enable module */
 if (metal_hpm_init(cpu) != 0) {
 return 1;
 }
 //
 //printf("hi_1343 \r\n");
 for(int m=0;m<5;m++)
 {

 for(int i=0; i<REPEAT; i++)
 {


 cnt1=metal_hpm_read_counter(cpu, 0);
 cnt1=metal_hpm_read_counter(cpu, 0);
 cnt1=metal_hpm_read_counter(cpu, 0);

 for(int mmm=0;mmm<27000;mmm++){
 IUJK(ia,ib,ic);
 }

 cnt2=metal_hpm_read_counter(cpu, 0);
sum+=(cnt2-cnt1);

}

 //printf("hi_1757 \r\n");
 //printf("mean= %d\n",sum);
 result[m]=sum;
 sum=0;
 }


 for(int m=0;m<5;m++)
 {
 for(int i=0; i<REPEAT; i++)
 {


 cnt1=metal_hpm_read_counter(cpu, 0);
 cnt1=metal_hpm_read_counter(cpu, 0);
 cnt1=metal_hpm_read_counter(cpu, 0);
/*
 __asm__ __volatile__(
		 "jalr x1, 0;"
		 "jalr x1, 0;"
		 "jalr x1, 0;"
		 "jalr x1, 0;"
		 "jalr x1, 0;"


		 );
*/
 cnt2=metal_hpm_read_counter(cpu, 0);

sum+=(cnt2-cnt1);


}

 //printf("hi_1757 \r\n");
 //printf("mean= %d\n",sum);
 base[m]=sum;
 sum=0;
 }
 for(int k=0;k<5;k++)
 {
	 printf("result[%d]=%d\r\n",k,result[k]);
	 //printf("base[%d]=%d\r\n",k,base[k]);
	 //result[k]=(result[k]-base[k]);
 }
 for(int k=0;k<5;k++)
 {
	 //printf("result[%d]=%d\r\n",k,result[k]);
	 printf("base[%d]=%d\r\n",k,base[k]);
 }
 return 0;
}
