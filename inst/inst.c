#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INS_NUM 57
int ins_list[INS_NUM+1]={0,};
unsigned short ins[128]={0,};

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

#define ADDI 0x00240413
#define	SLTI 0x07442413
#define	ANDI 0x00247413
#define	ORI	0x00246413
#define	XORI 0x00244413
#define	SLLI 0x00241413
#define	SRLI 0x00245413
#define	SRAI 0x40245413
#define	LUI	0x00002437
#define	AUIPC 0x00000417
#define	ADD	0x01340433
#define	SLT	0x01342433
#define	SLTU 0x01343433
#define	AND 0x01347433
#define	OR 0x01346433
#define	XOR	0x01344433
#define	SLL	0x01341433
#define	SRL	0x01345433
#define	SUB	0x41340433
#define	SLTIU 0x043bb793
#define	SRA	0x41345433
#define	NOP	0x00000013
#define	JAL	0x004000ef
#define	JALR 0x000780e7
#define	BEQ	0x008b8263
#define	BNE	0x01741263
#define	BLT	0x01744263
#define	BGE	0x008bd263
#define	BLTU 0x01746263
#define	BGEU 0x008bf263
#define	CSRRW 0x34051573
#define	CSRRS 0x301027f3
#define	CSRRC 0x34063573
#define	LB 0xaa078783
#define	LH 0xaa079783
#define	LW 0xaa07a783
#define	LBU	0xaa07c783
#define	LHU	0xaa07d783
#define	SB 0xaaf78023
#define	SH 0xaaf79023
#define	SW 0xaaf7a023
#define	MUL	0x03740433
#define	REM	0x03746433
#define	MULH 0x03741433
#define	MULHSU 0x03742433
#define	MULHU 0x03743433

void ins_in(){

ins_list[1]=ADDI;
ins_list[2]=SLTI;
ins_list[3]=ANDI;
ins_list[4]=ORI;
ins_list[5]=XORI;
ins_list[6]=SLLI;
ins_list[7]=SRLI;
ins_list[8]=SRAI;
ins_list[9]=LUI;
ins_list[10]=AUIPC;
ins_list[11]=ADD;
ins_list[12]=SLT;
ins_list[13]=SLTU;
ins_list[14]=AND;
ins_list[15]=OR;
ins_list[16]=XOR;
ins_list[17]=SLL;
ins_list[18]=SRL;
ins_list[19]=SUB;
ins_list[20]=SLTIU;
ins_list[21]=SRA;
ins_list[22]=NOP;
ins_list[23]=JAL;
ins_list[24]=JALR;
ins_list[25]=BEQ;
ins_list[26]=BNE;
ins_list[27]=BLT;
ins_list[28]=BGE;
ins_list[29]=BLTU;
ins_list[30]=BGEU;
ins_list[31]=CSRRW;
ins_list[32]=CSRRS;
ins_list[33]=CSRRC;
ins_list[40]=LB;
ins_list[41]=LH;
ins_list[42]=LW;
ins_list[43]=LBU;
ins_list[44]=LHU;
ins_list[45]=SB;
ins_list[46]=SH;
ins_list[47]=SW;
ins_list[50]=MUL;
ins_list[52]=REM;
ins_list[53]=MULH;
ins_list[54]=MULHSU;
ins_list[55]=MULHU;

    return;
}
void cal4(){
    int fun7=0;
    int fun3=0;
    int fun3_2=0;
    int opc=0;
    int temp=0;
    int temp2=0;
    int option1=-1;

    for(int iv=1;iv<INS_NUM+1;iv++)
    {
    if(ins_list[iv]==0) continue;
    opc=(ins_list[iv])&0x7f;
    temp2=ins_list[iv]>>25;
    option1+=((opc==0x37)||(opc==0x17)||(opc==0x6f)||(opc==0x67)||(opc==0x3)||(opc==0x23));
    fun7=(option1&((temp2>>5)&0x3))+2*((opc==0x33)&&(temp2&1));
    fun3=option1&((ins_list[iv]>>12)&0x7);
    fun3_2=(ins_list[iv]>>12)&0x7;
    
    temp=opc-(fun7*8)-fun3;
    if(temp<0) temp+=100;
    if(opc==0x23) temp+=50+fun3_2;
    if(opc==0x3) temp+=70+fun3_2;
    if(ins_list[iv]==0x13) temp+=20;
    
    //printf("%d opc, option1, fun7, fun3 =result/ %d %d %d %d %d\n",i,opc,option1,fun7,fun3,temp);
    
    ins[temp]=iv;
    option1=-1;
    }

    return;
}
int result0(int inst0){
    int fun7=0;
    int fun3=0;
    int fun3_2=0;
    int opc=0;
    int temp=0;
    int temp2=0;
    int temp3=0;
    int option1=-1;
    int result2=0;

    opc=inst0&0x7f;
    temp2=inst0>>25;
    option1+=((opc==0x37)||(opc==0x17)||(opc==0x6f)||(opc==0x67)||(opc==0x3)||(opc==0x23));
    fun7=(option1&((temp2>>5)&0x3))+2*((opc==0x33)&&(temp2&1));
    fun3=option1&((inst0>>12)&0x7);
    fun3_2=(inst0>>12)&0x7;
    
    temp=opc-(fun7*8)-fun3;
    if(temp<0) temp+=100;
    if(opc==0x23) temp+=50+fun3_2;
    if(opc==0x3) temp+=70+fun3_2;
    if(inst0==0x13) temp+=20;
    temp3=ins[temp];
    
    return ((0-!((inst0&0xfe00707f)-(temp3&0xfe00707f)))|temp3);
    //printf("%d opc, option1, fun7, fun3 =result/ %d %d %d %d %d\n",i,opc,option1,fun7,fun3,temp);
    //return ins[temp];
}
void readinst23(unsigned char* ptr2,int size){
    //int first=9;
    //int last=strlen(ptr2)-4;
    int value=0;
    int hex=1;
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
       printf("inst: %d\n",result0(value));
       value=0;
       hex=1;
    }
       
}

int check1(){

    int fine=0;

    for(int i=1;i<128;i++)
    {
        if(ins[i]!=0) fine++;
  //      printf("ins[%d]=%d\n",i,ins[i]); 
    }
    printf("number of 1 : %d\n",fine);
    //return (fine==INS_NUM);
    return (fine==46);
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

int main(){
   int read=0;
   int read_size=0;
   unsigned char* text=NULL;
   FILE *ppFile = NULL;
   //int size=0;
   ins_in();
   cal4();
   
   if(check1()==1) printf("fine\n"); else printf("not fine\n");   
   
   ppFile = fopen("example-hpm.elf","rb");
   read=readelf(ppFile,&text,&read_size);
   if(read == 0)
   {
       readinst23(text,read_size);
   }

  // for(int v=0;v<read_size;v++){
  //     printf("%x",text[v]);
  //     if((v+1)%4==0) printf("\n");
  // }
   free(text);
   fclose(ppFile);

   //printf("SB: 45, result=%d\n",result0(SB));
   //printf("SH: 46, result=%d\n",result0(SH));
   //printf("funct7 = %d\n, funct3=%d\n, opcode=%d\n",funct7,funct3,opcode);

       
    return 0;
}

