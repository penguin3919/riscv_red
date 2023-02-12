#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
       //printf("%x\n",value);
       //printf("inst: %d\n",result0(value));
       printf("%08x\n",value);
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

int main(){
   int read=0;
   int read_size=0;
   unsigned char* text=NULL;
   FILE *ppFile = NULL;
   //int size=0;
   
   ppFile = fopen("example-hpm.elf","rb");
   read=readelf(ppFile,&text,&read_size);
   if(read == 0)
   {
       readinst23(text,read_size);
   }

   //for(int v=0;v<read_size;v++){
   //    printf("%x",text[v]);
   //    if((v+1)%4==0) printf("\n");
  // }
   free(text);
   fclose(ppFile);

   //printf("SB: 45, result=%d\n",result0(SB));
   //printf("SH: 46, result=%d\n",result0(SH));
   //printf("funct7 = %d\n, funct3=%d\n, opcode=%d\n",funct7,funct3,opcode);

       
    return 0;
}

