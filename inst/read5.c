#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(){

    FILE *pFile = NULL;
    pFile = fopen("example-hpm.elf","rb");
    
    if(pFile == NULL)
    {
        return 0;
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
        printf("%d\n",ehdr.phoff); 
        
        temp=ehdr.shoff+(ehdr.shstrndx*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&stroff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&strsize,4,1,pFile);
        
        char* strtab=(char*)malloc(strsize);
        fseek(pFile,stroff,SEEK_SET);
        fread(strtab,1,strsize,pFile);
        //for(int ii=0;ii<strsize;ii++) printf("%c",strtab[ii]);
        //printf("\n");
        for(int i=0;i<ehdr.shnum;i++)
        {
            fseek(pFile,ehdr.shoff+(i*ehdr.shentsize),SEEK_SET);
            fread(&name,4,1,pFile);
            if(!strncmp(strtab+name,".text",5))
            {
                goal=i;
                break;
            }
            //printf("%s\n",strtab+name);
        }
        printf("goal= %d\n",goal);
        
        temp=ehdr.shoff+(goal*ehdr.shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&goaloff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&goalsize,4,1,pFile);
        
        unsigned char* text0=(unsigned char*)malloc(goalsize);
        fseek(pFile,goaloff,SEEK_SET);
        fread(text0,1,goalsize,pFile);
        
        for(int v=0;v<goalsize;v++){
            printf("%x",text0[v]);
            if((v+1)%4==0) printf("\n");
        }

        free(strtab);
        free(text0);
        fclose(pFile);
    }
    return 0;
}
