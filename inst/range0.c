#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int b_link=0;
int link0=0;
int b_freq=0;
int line0=0;
int max0=0;

short zero_set=0;
short mid_end=0;
short mid_freq=0;
short print_end=0;
int print_size=0;

typedef struct {
    unsigned int st_name;
    unsigned int st_value;
    unsigned int st_size;
    unsigned char st_info;
    unsigned char st_other;
    unsigned short st_shndx;
} symbolt;

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

void text_to_4byte_inst(FILE* mid, unsigned char* ptr2,int size){
    int value=0;
    int hex=1;
    char temp[10];
    unsigned char a=0;

    for(int ii=0;ii<size;ii+=4)
    {
       for(int iii=0;iii<4;iii++)
       {
           a=*(ptr2+ii+iii);
           value+=hex*a;
           hex<<=8;
       }
       sprintf(temp,"%08x\n",value);
       fwrite(temp,1,9,mid);
       value=0;
       hex=1;
    }
    
       
}

int readelf(FILE* pFile, unsigned char** text0,int* size,unsigned int** sym0,int* sym_size)
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
        int temp3=0;
        int temp3_size=0;
        int goal=0;
        int goal2=0;
        int goal3=0;
        int goaloff=0;
        int goaloff2=0;
        int goalsize=0;
        int sym_count=3;

        int num3=0;//number of symbol
        symbolt symbols;
        int main_addr=0;
        int text_addr=0;
        int counter_addr=0;

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
            if((goal==0)&&(!strncmp(strtab+name,".text",5)))
            {
                goal=i;
            }

            if((goal!=0)&&(!strncmp(strtab+name,".symtab",7)))
            {
                goal2=i;
            }
            if((goal!=0)&&(!strncmp(strtab+name,".strtab",7)))
            {
                goal3=i;
                break;
            }
        }
       //read .text  
        temp=ehdr.shoff+(goal*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&goaloff,4,1,pFile);
        
        fseek(pFile,temp-4,SEEK_SET);
        fread(&text_addr,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&goalsize,4,1,pFile);
        *size=goalsize;
        *text0=(unsigned char*)malloc(goalsize);
        fseek(pFile,goaloff,SEEK_SET);
        fread(*text0,1,goalsize,pFile);

       //read .symtab
        temp=ehdr.shoff+(goal2*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&goaloff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&goalsize,4,1,pFile);
        
       //read .strtab
        temp3=ehdr.shoff+(goal3*ehdr.shentsize)+16;
        fseek(pFile,temp3,SEEK_SET);
        fread(&goaloff2,4,1,pFile);
        fseek(pFile,temp3+4,SEEK_SET);
        fread(&temp3_size,4,1,pFile);
        
        char* strtab2=(char*)malloc(temp3_size);
        fseek(pFile,goaloff2,SEEK_SET);
        fread(strtab2,1,temp3_size,pFile);
       

        //sym_count->init_val=3
        num3=goalsize/sizeof(symbols);
        *sym0=(unsigned int*)malloc((num3*2)+2);
        
        *sym_size=num3;
        for(int i=0;i<num3;i++)
        {
            temp=goaloff+(i*sizeof(symbols));
            fseek(pFile,temp,SEEK_SET);
            fread(&symbols,sizeof(symbols),1,pFile);
            if(!strncmp(strtab2+symbols.st_name,"main",4))
            {
                *((*sym0))=((symbols.st_value-text_addr)/4)+1;
                *((*sym0)+1)=symbols.st_size/4;
            }
            if(!strncmp(strtab2+symbols.st_name,"metal_hpm_read_counter",22))
            {
                *((*sym0)+2)=((symbols.st_value-text_addr)/4)+1;
            }
            else if((symbols.st_info&0xf)==2 && (symbols.st_info>>4)==1)
            {
                *((*sym0)+sym_count)=((symbols.st_value-text_addr)/4)+1;
                *((*sym0)+sym_count+1)=symbols.st_size/4;
                sym_count+=2;
            }
            
        }

        free(strtab);
    } 
    
        return 0;
}

//hex_to_instruction(mid2File,i,a,sym,sym_size);
void hex_to_instruction(FILE* mid2,int line, unsigned int inst2, unsigned int* sym2,int sym2_size){
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
    char* ir19[8]={"addi","slli","srai","sltiu","xori","srli","ori","andi"};
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

            if((jump0 >= max0) && (jump0!=*(sym2+2)) && (b_freq==2 || line<*(sym2)))
            {
                for(int m=3;m<sym2_size;m+=2)
                {
                    if(*(sym2+m)==jump0) 
                    {
                        max0=jump0+*(sym2+m+1);
                    }
                }
            }

            if(b_link==*(sym2+2)) b_freq++;
            if(zero_set==1 && link0==jump0) mid_end=1;
            if(zero_set==0 && b_freq==2) 
            {
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

            if((jump0 >= max0) && (jump0!=*(sym2+2)) && (b_freq==2 || line<*(sym2)))
            {
                for(int m=3;m<sym2_size;m+=2)
                {
                    if(*(sym2+m)==jump0)
                    {
                        max0=jump0+*(sym2+m+1);
                    }
                }
            }

            if(b_link==*(sym2+2)) b_freq++;
            if(zero_set==1 && link0==jump0) mid_end=1;
            if(zero_set==0 && b_freq==2) 
            {
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
     
    if(mid_end==1 && mid_freq==0) 
    {
        sprintf(temp4,"jal x0, end00\n");
    }

    if(print_end==1)
    {
        sprintf(temp4,"end00: addi x0, x0, 0\n"); 
    }

    if(mid_end==0 && print_end!=1 && (b_freq==3 || line<*(sym2) || line>=(-1+*(sym2)+*(sym2+1))|| b_freq==2))
    {
    print_size++;
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
            sprintf(temp4,"nop\n");
        }
    }

    fwrite(temp4,1,strlen(temp4),mid2);
    if(mid_end==1){
        mid_freq++;
     mid_end=0;
     b_freq=0;
    }
    return;
}
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
   for(m=0;m<print_size+2;m++)
   {//1
   fgets(aa0,40,ppFile0);
   select0=0;

   for(i=4;i<strlen(aa0);i++)
   {
       if(!strncmp(aa0+i,strj,4))
       {
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

   if(sum0>max0)
   {
       aa0[start0]=48;
       aa0[start0+1]=48;
       aa0[start0+2]='\n';
       aa0[start0+3]='\0';
       }
   }
   fwrite(aa0,1,strlen(aa0),endend);
   }//1
   fgets(aa0,40,ppFile0);
   fwrite(aa0,1,strlen(aa0),endend);
    

    return 0;
}

int main(){

    int read=0;
    int read_size=0;
    int sym_size=0;
    unsigned char* text=NULL;
    unsigned int* sym=NULL;
    FILE* ppFile=NULL;

    char aa[9];
    char temp2[20];
    int a=0;
    int length=0;
    int i=0;
    int max=0;
    
    FILE* endFile=NULL;
    FILE* midFile=NULL;
    FILE* mid2File=NULL;
    FILE* endend0=NULL;
//inst3.c
   ppFile = fopen("example-hpm.elf","rb");
   midFile=fopen("mid.txt","w");
//   add sym, sym_size ++symbol table
   read=readelf(ppFile,&text,&read_size,&sym,&sym_size);
  
   if(read == 0)
   {
       text_to_4byte_inst(midFile,text,read_size);
   }
   fclose(midFile);
    endFile=fopen("mid.txt","r");

    fseek(endFile,0,SEEK_END);
    length=ftell(endFile)/9;
    fseek(endFile,0,SEEK_SET);
    mid2File=fopen("mid2.txt","w");
    sprintf(temp2,"jal x0, jump00\n");
    fwrite(temp2,1,strlen(temp2),mid2File);
    max0=sym[0]+sym[1];

    for(i=0;i<max0;i++) 
    {
    fread(aa,9,1,endFile);
    if(aa!=NULL)
     {
    a=hex_to_dec(aa);
    if(i==max0-1) print_end=1;
    hex_to_instruction(mid2File,i,a,sym,sym_size);
     }
    }

    fclose(mid2File);
    fclose(endFile);

    endFile=fopen("mid2.txt","r");
    endend0=fopen("fin3.txt","w");
    max=max0-1;
    max=delete_exceeded_label_range(endFile,endend0,max);
    
    free(text);
    fclose(endFile);
    fclose(endend0);
    fclose(ppFile);
    return 0;
}

