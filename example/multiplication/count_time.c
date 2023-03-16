/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define REPEAT 1


int main() {

	int a=3;
	int b=2;

 int cnt1=0;
 int cnt2=0;
 int sum=0;
 int result[5]={0,};
 int base[5]={0,};



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

 for (int i=0;i<14000000;i++)
 	 {
 		a*=3;
 	 b*=i*a;


 	 }



 cnt2=metal_hpm_read_counter(cpu, 0);
sum=sum+(cnt2-cnt1);

 }
 result[m]=sum;
  sum=0;
  printf("%d %d\r\n",a,b);

 a=3;
 b=2;
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
