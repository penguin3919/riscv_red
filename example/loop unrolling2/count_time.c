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


void IJUK(int a[LENA][LENA],int b[LENA][LENA],int c[LENA][LENA])
{
  int i,j,k,khi;

  khi=(LENA/NROLL)*NROLL;
  khi--;

  for(i=0; i< LENA;i++)
    {
  	  for(j=0;j<LENA;j++)
  	  {
  		b[i][j]=1;
  		c[i][j]=1;
  	  }
  	  }

  for (i=0;i < LENA; i++)
    for (j=0; j < LENA; j++)
       for (k=0; k <khi;k+=NROLL)
       {
         a[i][k]=a[i][k]+b[i][j]*c[j][k];
         a[i][k+1]=a[i][k+1]+b[i][j]*c[j][k+1];
         //a[i][k+2]=a[i][k+2]+b[i][j]*c[j][k+2];
         //a[i][k+3]=a[i][k+3]+b[i][j]*c[j][k+3];
       }
  for (i=0; i < LENA; i++)
    for (j=0; j < LENA; j++)
       for (k=khi+1;k < LENA;k++)
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
 IJUK(ia,ib,ic);
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
