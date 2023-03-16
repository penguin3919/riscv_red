/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define REPEAT 1
uint32_t KEY[4];  // Key space for bit shifts

void encrypt(uint32_t* v) {
  uint32_t v0=v[0], v1=v[1], sum=0, i;             // set up
  uint32_t delta=0x9e3779b9; 						// a key schedule constant

  for (i=0; i < 32; i++) {                         // basic cycle start
    sum += delta;
    v0 += ((v1<<4) + KEY[0]) ^ (v1 + sum) ^ ((v1>>5) + KEY[1]);
    v1 += ((v0<<4) + KEY[2]) ^ (v0 + sum) ^ ((v0>>5) + KEY[3]);
  }                                                // end cycle
  v[0]=v0; v[1]=v1;
}

void decrypt (uint32_t* v) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  // set up
    uint32_t delta=0x9e3779b9;                     // a key schedule constant
    for (i=0; i<32; i++) {                         // basic cycle start
        v1 -= ((v0<<4) + KEY[2]) ^ (v0 + sum) ^ ((v0>>5) + KEY[3]);
        v0 -= ((v1<<4) + KEY[0]) ^ (v1 + sum) ^ ((v1>>5) + KEY[1]);
        sum -= delta;
    }                                              // end cycle
    v[0]=v0; v[1]=v1;
}
int main() {

	uint32_t v[] = {0xFFFFFFFF, 0xFFFFFFFF};
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

 for(int mmm=0;mmm<84000;mmm++)
 {
 encrypt(v);
decrypt(v);
 }


 cnt2=metal_hpm_read_counter(cpu, 0);
sum+=(cnt2-cnt1);

}

 //printf("hi_1757 \r\n");
 //printf("mean= %d\n",sum);
 result[m]=sum;
 sum=0;
 }

 printf("[ %X %X ]\r\n", v[0], v[1]);

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
