#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int b_link=0;
int link0=0;
int b_freq=0;
int line0=0;
short zero_set=0;
short print_end=0;

typedef struct {
    unsigned char ident[16];
    unsigned short type;
    unsigned short machine;
    unsigned int version;
    unsigned int entry;
    unsigned int phoff;
    unsigned int shoff;
    unsigned int flags;
    unsigned short ehsize;
    unsigned short phentsize;
    unsigned short phnum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shstrndx;
} elfHeader;

void text_to_8bit_inst(FILE* mid, unsigned char* ptr2,int size){
    //int first=9;
    //int last=strlen(ptr2)-4;
    int value=0;
    int hex=1;
    char temp[10];
    unsigned char a=0;

    for(int ii=0;ii<size;ii+=4)
    {
       for(int iii=0;iii<4;iii++)
       {
           a=*(ptr2+ii+iii);
           //value+=hex*(a-48+(~((a-58)>>7)*7));
           value+=hex*a;
           hex<<=8;
       }
       //printf("%x\n",value);
       //printf("inst: %d\n",result0(value));
   //printf("wow\n");
       sprintf(temp,"%08x\n",value);
       fwrite(temp,1,9,mid);
       value=0;
       hex=1;
    }
    
       
}

int readelf(FILE* pFile, unsigned char** text0,int* size)
{
    if(pFile == NULL)
    {
        return -1;
    }
    else
    {
        elfHeader ehdr;
        int stroff=0;
        int strsize=0;
        int name;
        int temp=0;
        int goal=0;
        int goaloff=0;
        int goalsize=0;
        
        fseek(pFile,0,SEEK_SET);
        fread(&ehdr,52,1,pFile);
        
        temp=ehdr.shoff+(ehdr.shstrndx*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&stroff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&strsize,4,1,pFile);
        
        char* strtab=(char*)malloc(strsize);
        fseek(pFile,stroff,SEEK_SET);
        fread(strtab,1,strsize,pFile);

        for(int i=0;i<ehdr.shnum;i++)
        {
            fseek(pFile,ehdr.shoff+(i*ehdr.shentsize),SEEK_SET);
            fread(&name,4,1,pFile);
            if(!strncmp(strtab+name,".text",5))
            {
                goal=i;
                break;
            }
        }
        
        temp=ehdr.shoff+(goal*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&goaloff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&goalsize,4,1,pFile);
        *size=goalsize;
        *text0=(unsigned char*)malloc(goalsize);
        fseek(pFile,goaloff,SEEK_SET);
        fread(*text0,1,goalsize,pFile);
        free(strtab);
    } 
        return 0;
}

void hex_to_instruction(FILE* mid2,int line, unsigned int inst2){
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
    char temp3[10];
    char temp4[40];

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
            if(zero_set==1 && link0==jump0) print_end=1;
            if(zero_set==0 && b_freq==2) 
            {
                //print_end=1;
                line0=line+1;
                link0=jump0;
                zero_set=1;
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
            if(zero_set==1 && link0==jump0) print_end=1;
            if(zero_set==0 && b_freq==2) 
            {
                //print_end=1;
                line0=line+1;
                link0=jump0;
                zero_set=1;
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

    if(print_end!=1)
    {
        if(line!=0 && line==line0)
            sprintf(temp3,"jump00:");
        else sprintf(temp3,"jump%d:",line+1);
    
        switch(format)
        {
        case 0://R
            if(opcode==19) 
            {
            sprintf(temp4,"%s %s x%d, x%d, %d\n",temp3,inst3,rd,rs1,shamt);
            break;
            }
            sprintf(temp4,"%s %s x%d, x%d, x%d\n",temp3, inst3,rd,rs1,rs2);
            break;
        case 1://I
            if(opcode==3 || opcode==103)
                sprintf(temp4,"%s %s x%d, %d(x%d)\n",temp3,inst3,rd,imm,rs1);
            else
            sprintf(temp4,"%s %s x%d, x%d, %d\n",temp3,inst3,rd,rs1,imm);
            break;
        case 2://S
            sprintf(temp4,"%s %s x%d, %d(x%d)\n",temp3,inst3,rs2,imm,rs1);
            break;
        case 3://B
            sprintf(temp4,"%s %s x%d, x%d, jump%d\n",temp3,inst3,rs1,rs2,jump0);
            break;
        case 4://U
            sprintf(temp4,"%s %s x%d, %d\n",temp3,inst3,rd,imm);
            break;
        case 5://J
            sprintf(temp4,"%s %s x%d, jump%d\n",temp3,inst3,rd,jump0);
            break;
        default:
            sprintf(temp4,"%s no op found\n",temp3);

    }
    }
    fwrite(temp4,1,strlen(temp4),mid2);
    //fwrite(temp2,1,strlen(temp2),mid2File);

   //print_instruction_end
    return;
}
//int read23(unsigned char* ptr){
int hex_to_dec(unsigned char* ptr){
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
int delete_exceeded_label_range(FILE* ppFile0,FILE* endend,int max0)
{ 
   char aa0[40];
   
   int i=0;
   int m=0;
   int sum0=0;
   int ten0=1;

   const char* strj="jump";

   int select0=0;
   int start0=0;
   int end0=0;

   //line: max0+1
   for(m=0;m<max0+1;m++)
   {//1
   fgets(aa0,40,ppFile0);
   select0=0;

   for(i=4;i<strlen(aa0);i++)
   {
       if(!strncmp(aa0+i,strj,4))
       {
           //printf("%d wow",strlen(aa0));
           start0=i+4;
           select0=1;
           break;
       }
   }
   if(select0==1)
   {
       end0=strlen(aa0)-2;
       ten0=1;
       sum0=0;
   for(i=0;i<end0-start0+1;i++)
   {
       sum0+=ten0*(aa0[end0-i]-48);
       ten0*=10;
   }
   //printf("hi sum0: %d\n",sum0);

   if(sum0>max0)
   {
       aa0[start0]=48;
       aa0[start0+1]=48;
       aa0[start0+2]='\n';
       aa0[start0+3]='\0';
       }
   }
   //printf("%s\n",aa0);
   fwrite(aa0,1,strlen(aa0),endend);
   }//1
    
   //printf("start0: %c\n",aa0[start0]);
   //printf("end0: %c",aa0[strlen(aa0)-2]);

    return 0;
}

int main(){

    int read=0;
    int read_size=0;
    unsigned char* text=NULL;
    FILE* ppFile=NULL;
    FILE* midFile=NULL;

    char aa[9];
    char temp2[20];
    int a=0;
    int length=0;
    int i=0;
    FILE* endFile=NULL;
    
    FILE* mid2File=NULL;
    int max=0;

    FILE* endend0=NULL;
//inst3.c
   ppFile = fopen("example-hpm.elf","rb");
   midFile=fopen("mid.txt","w");
   read=readelf(ppFile,&text,&read_size);
   if(read == 0)
   {
       text_to_8bit_inst(midFile,text,read_size);
   }
   fclose(midFile);
//range0.c
    endFile=fopen("mid.txt","r");

    fseek(endFile,0,SEEK_END);
    length=ftell(endFile)/9;
    fseek(endFile,0,SEEK_SET);
    //printf("size: %d\n",length);
    mid2File=fopen("mid2.txt","w");
    sprintf(temp2,"jal x0, jump00\n");
    //printf("sizeof:temp2: %d\n",strlen(temp2));
    fwrite(temp2,1,strlen(temp2),mid2File);
    
    for(i=0;i<length;i++) 
    {
    if(print_end==1) break;
    fread(aa,9,1,endFile);
    if(aa!=NULL)
    {
    a=hex_to_dec(aa);
    //printf("%s\n",argv[1]);
    //printf("a: %x\n",a);
    hex_to_instruction(mid2File,i,a);
    }
    }
    fclose(mid2File);
    fclose(endFile);

    endFile=fopen("mid2.txt","r");
    endend0=fopen("fin0.txt","w");
    max=i-1;
    max=delete_exceeded_label_range(endFile,endend0,max);
  
    
    free(text);
    fclose(ppFile);
    fclose(endFile);
    fclose(endend0);
//printf("link, freq: %d, %d\n",b_link,b_freq);
//printf("lines: %d\n",i);
//printf("line0: %d\n",line0);
    return 0;
}
