/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define REPEAT 1

void matmult(int m, int n, int a[m][n], int b[m][n],
       int out[m][n]) {
 unsigned char i, j, k;
 for (i = 0; i < m; ++i) {
  for (j = 0; j < n; ++j) {
   out[i][j] = 0;
   for (k = 0; k < m; ++k) {
	    out[i][j] += a[i][k] * b[k][j];
	    out[i][j] += a[i][k] * b[k][j];
	    out[i][j] += a[i][k] * b[k][j];
	    out[i][j] += a[i][k] * b[k][j];
	    out[i][j] += a[i][k] * b[k][j];
	    out[i][j] += a[i][k] * b[k][j];
	        out[i][j] += a[i][k] * b[k][j];
	        out[i][j] += a[i][k] * b[k][j];
	        out[i][j] += a[i][k] * b[k][j];
	        out[i][j] += a[i][k] * b[k][j];
	        out[i][j] += a[i][k] * b[k][j];
	            out[i][j] += a[i][k] * b[k][j];
	            out[i][j] += a[i][k] * b[k][j];
	            out[i][j] += a[i][k] * b[k][j];
	            out[i][j] += a[i][k] * b[k][j];
	            out[i][j] += a[i][k] * b[k][j];
	                out[i][j] += a[i][k] * b[k][j];
	                out[i][j] += a[i][k] * b[k][j];
	                out[i][j] += a[i][k] * b[k][j];
	                out[i][j] += a[i][k] * b[k][j];
   }
  }
 }
}

int main() {

int a=7;
int b=7;
int c[7][7];
int d[7][7];
int e[7][7];

 int cnt1=0;
 int cnt2=0;
 int sum=0;
 int result[5]={0,};
 int base[5]={0,};
 unsigned char f=0,g=0;


 struct metal_cpu *cpu;
 /* Get CPU device hsrale. */
 cpu = metal_cpu_get(metal_cpu_get_current_hartid());

 /* Enable module */
 if (metal_hpm_init(cpu) != 0) {
 return 1;
 }
	for(f=0;f<a;f++){
			for(g=0;g<b;g++)
			{
				c[f][g]=1;
				d[f][g]=1;
			}
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

 for(int mmm=0;mmm<2000;mmm++)
 {

matmult(a,b,c,d,e);
 }

 cnt2=metal_hpm_read_counter(cpu, 0);
sum=sum+(cnt2-cnt1);

//printf("sum=%d\r\n",sum);

 //printf("hi_1757 \r\n");
 //printf("mean= %d\n",sum);





 }
 result[m]=sum;
 sum=0;
 }
	for(f=0;f<a;f++){
		for(g=0;g<b;g++)
		{
			printf("%d ",e[f][g]);
		}
		printf("\r\n");
	}


 for(int m=0;m<5;m++)
 {
 for(int it=0; it<REPEAT; it++)
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
// c=(i%4)*i;
// a=(c%10)+2;
// b=(c%10)+2;

}

 //printf("hi_1757 \r\n");
 //printf("mean= %d\n",sum);
 base[m]=sum;
 sum=0;
 }
 for(int kt=0;kt<5;kt++)
 {
	 printf("result[%d]=%d\r\n",kt,result[kt]);
	 //printf("base[%d]=%d\r\n",kt,base[kt]);
	 //result[k]=(result[k]-base[k]);
 }
 for(int kt=0;kt<5;kt++)
 {
	 //printf("result[%d]=%d\r\n",kt,result[kt]);
	 printf("base[%d]=%d\r\n",kt,base[kt]);
 }
 return 0;
}
