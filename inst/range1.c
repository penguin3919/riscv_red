#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int b_link=0;
int link0=0;
int b_freq=0;
short print_end=0;

void unit2(int line, unsigned int inst2){
    short opcode=inst2&0x7f;
    short funct3=(inst2>>12)&0x7;
    short funct7=(inst2>>25)&0x7f;
    short ir=(funct3==1)|(funct3==5);
    short rs1=0;
    short rs2=0;
    short rd=0;
    short shamt=0;
    short format=-1;//R,I,S,B,U,J
    int imm=0;
    int temp=0;
    int temp2=0;
    int jump0=0;
    char inst3[5];
    char* load[6]={"lb","lh","lw","nop","lbu","lhu"};
    char* branch[8]={"beq","bne","nop","nop","blt","bge","blt","bge"};
    char* store[3]={"sb","sh","sw"};
    char* ir19[8]={"addi","slli","srai","nop","xori","srli","ori","andi"};
    char* r51[8]={"add","sll","sub","sra","xor","srl","or","and"};
    char* mul51[8]={"mul","mul","mul","mul","div","divu","rem","remu"};

    strncpy(inst3,"hell",5);
    switch(opcode)
    {
        case 55:
            strncpy(inst3,"lui",5);
            format=4;
            rd=(inst2>>7)&0x1f;
            imm=(inst2>>12)&0xfffff;
            break;
        case 111:
            strncpy(inst3,"jal",5);
            format=5;
            rd=(inst2>>7)&0x1f;
            imm=(inst2>>12)&0xfffff;
            imm=((imm>>9)&0x3ff)|
                ((imm&0xff)<<11)|
                (((imm>>8)&0x1)<<10)|
                (((imm>>19)&0x1)<<19);
            imm=(imm<<12)>>12;
            jump0=line+1+(imm/2);
            if(b_link==jump0) b_freq++;
            if(link0==jump0) print_end=1;
            if(b_freq==2) 
            {
                //print_end=1;
                link0=jump0;
            }
            b_link=jump0;
            break;
        case 103:
        case 3:
            if(opcode==103) strncpy(inst3,"jalr",5); 
            else strncpy(inst3,load[funct3],5);
            format=1;
            rd=(inst2>>7)&0x1f;
            rs1=(inst2>>15)&0x1f;
            imm=(inst2>>20)&0xfff;
            imm=(imm<<20)>>20;
            break;
        case 99:
            strncpy(inst3,branch[funct3],5);
            format=3;
            imm=(inst2>>7)&0x1f;
            imm=(((funct7>>6)&0x1)<<11)|
                ((imm&0x1)<<10)|
                ((funct7&0x3f)<<4)|
                ((imm>>1)&0xf);
            imm=(imm<<20)>>20;
            rs1=(inst2>>15)&0x1f;
            rs2=(inst2>>20)&0x1f;
            jump0=line+1+(imm/2);
            if(b_link==jump0) b_freq++;
            if(link0==jump0) print_end=1;
            if(b_freq==2) 
            {
                //print_end=1;
                link0=jump0;
            }
            b_link=jump0;
            break;
        case 35:
            strncpy(inst3,store[funct3],5);
            format=2;
            imm=(funct7<<5)|((inst2>>7)&0x1f);
            imm=(imm<<20)>>20;
            rs1=(inst2>>15)&0x1f;
            rs2=(inst2>>20)&0x1f;
            break;
        case 19:
            if(ir==1) //R
            {
            temp2=(-3)*((funct7>>5)&1);
            strncpy(inst3,ir19[funct3+temp2],5);
            format=0;
            rd=(inst2>>7)&0x1f;
            rs1=(inst2>>15)&0x1f;
            shamt=(inst2>>20)&0x1f;
            }
            else //I
            {
            strncpy(inst3,ir19[funct3],5);
            format=1;
            rd=(inst2>>7)&0x1f;
            rs1=(inst2>>15)&0x1f;
            imm=(inst2>>20)&0xfff;
            imm=(imm<<20)>>20;
            }
            break;
        case 51:
            if(funct7!=1)
            {
            if((funct7>>5)-funct7) temp2=-2+(4*!funct3);
            strncpy(inst3,r51[funct3+temp2],5);
            }
            else strncpy(inst3,mul51[funct3],5);
            format=0;
            rd=(inst2>>7)&0x1f;
            rs1=(inst2>>15)&0x1f;
            rs2=(inst2>>20)&0x1f;
            
    }
    //printf("%d\n",opcode);
    //printf("%s\n",inst3); //inst3
     
    //if(b_freq==4) printf("b_link=%d, b_freq=%d\n",b_link,b_freq);
   //print_instruction_start 
/*
    if(print_end!=1)
    {
    printf("jump%d: ",line+1);
    switch(format)
    {
        case 0://R
            if(opcode==19) 
            {
            printf("%s x%d, x%d, %d\n",inst3,rd,rs1,shamt);
            break;
            }
            printf("%s x%d, x%d, x%d\n",inst3,rd,rs1,rs2);
            break;
        case 1://I
            printf("%s x%d, x%d, %d\n",inst3,rd,rs1,imm);
            break;
        case 2://S
            printf("%s x%d, x%d, %d\n",inst3,rs1,rs2,imm);
            break;
        case 3://B
            printf("%s x%d, x%d, jump%d\n",inst3,rs1,rs2,jump0);
            break;
        case 4://U
            printf("%s x%d, %d\n",inst3,rd,imm);
            break;
        case 5://J
            printf("%s x%d, jump%d\n",inst3,rd,jump0);
            break;
        default:
            printf("no op found\n");

    }
    }
*/
   //print_instruction_end
    return;
}
int read23(unsigned char* ptr){
    int hex=1;
    int sum=0;
    int temp=0;
    for(int i=7;i>=0;i--)
    {
        temp=*(ptr+i);
        temp=temp-48+((~((temp-96)>>31))&(-39));
        sum+=hex*temp;
        hex<<=4;
    }
    //printf("%x\n",sum);
    return sum;
}

int main(){

    //unsigned int inst=0xfa010113;
    //unsigned int inst=0x000780e7;

    //unit2(0x008bfe63);
    FILE* ppFile=NULL;
    //ppFile=fopen("read02.txt","r");
    ppFile=fopen("a215.txt","r");
    char aa[9];
    int a=0;
    int length=0;
    int i=0;

    fseek(ppFile,0,SEEK_END);
    length=ftell(ppFile)/9;
    fseek(ppFile,0,SEEK_SET);
    //printf("size: %d\n",length);

    for(i=0;i<length;i++) 
    {
    if(print_end==1) break;
    fread(aa,9,1,ppFile);
    if(aa!=NULL)
    {
    a=read23(aa);
    //printf("%s\n",argv[1]);
    unit2(i,a);
    }
    }
printf("link, freq: %d, %d\n",b_link,b_freq);
printf("lines: %d\n",i);
    return 0;
}
