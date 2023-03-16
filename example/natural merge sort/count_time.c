/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define REPEAT 1
#define N 50

void natmer()
{
int a[N+1]={0,};
int b[N+1]={0,};
int c[N+1]={0,};
int m=0,n=0,p=0,q=0,k=0,i=0;
int temp=0;

for(int i=0;i<N+1;i++)
{
	 a[i]=N+1-i;
}

//boundary

b[0]=-1;
c[0]=-1;

for(int i=1;i<N;i++)
{
	if(!!((a[i+1]+(~a[i])+1)>>31))
		a[i]^=-1;
}
//last
a[N]^=-1;

while(n!=N)
{
m=n;
	if(m==0)
	{

	for(i=1;i<N+1;i++)
	{
		if(a[i]<0)
		{	m=i;
			break;
		}
	}
	a[m]^=-1;

	for(i=1;i<=m;i++)
	{
		b[i]=a[i];
	}

	}

for(i=m+1;i<N+1;i++)
{
	if(a[i]<0)
	{
		n=i;
		break;
	}
}
if(m>n)return;

a[n]^=-1;

q=k=n;
p=m;


while(p!=0 || q!=m)
{
	temp=!((b[p]+(~a[q])+1)>>31);
	if(temp || q==m)
	{
		c[k]=b[p];
		k--;
		p--;
	}
	else if((!temp) || p==0)
	{
		c[k]=a[q];
		k--;
		q--;
	}
}

for(i=1; i<N+1;i++)
{
	b[i]=c[i];
}

}

for(i=1; i<N+1;i++)
{
	a[i]=c[i];
}

}




int main() {

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
for(int mmm=0;mmm<3000;mmm++)
{
 natmer();
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
