#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){

    FILE *pFile = NULL;
    pFile = fopen("example-hpm.elf","rb");
    
    if(pFile == NULL)
    {
        return 0;
    }
    else
    {
        unsigned int phoffset;
        unsigned int shoffset;
        unsigned short phentsize;
        unsigned short shentsize;
        unsigned short phnum;
        unsigned short shnum;
        unsigned short strndx;
        int stroff=0;
        int strsize=0;
        int name;
        int temp=0;
        int goal=0;
        int goaloff=0;
        int goalsize=0;
        
        fseek(pFile,28,SEEK_SET);
        fread(&phoffset,4,1,pFile);
        //printf("%x\n",phoffset);
        
        fseek(pFile,32,SEEK_SET);
        fread(&shoffset,4,1,pFile);
        //printf("%d\n",shoffset);
        
        fseek(pFile,42,SEEK_SET);
        fread(&phentsize,2,1,pFile);
        //printf("%x\n",phentsize);
        
        fseek(pFile,44,SEEK_SET);
        fread(&phnum,2,1,pFile);
        //printf("%x\n",phnum);
        
        fseek(pFile,46,SEEK_SET);
        fread(&shentsize,2,1,pFile);
        //printf("%x\n",shentsize);

        fseek(pFile,48,SEEK_SET);
        fread(&shnum,2,1,pFile);
        //printf("%x\n",shnum);
        
        fseek(pFile,50,SEEK_SET);
        fread(&strndx,2,1,pFile);
        
        temp=shoffset+(strndx*shentsize)+16;
        
        fseek(pFile,temp,SEEK_SET);
        fread(&stroff,4,1,pFile);
        
        fseek(pFile,temp+4,SEEK_SET);
        fread(&strsize,4,1,pFile);
        
        char* strtab=(char*)malloc(strsize);
        fseek(pFile,stroff,SEEK_SET);
        fread(strtab,1,strsize,pFile);
        //for(int ii=0;ii<strsize;ii++) printf("%c",strtab[ii]);
        //printf("\n");
        for(int i=0;i<shnum;i++)
        {
            fseek(pFile,shoffset+(i*shentsize),SEEK_SET);
            fread(&name,4,1,pFile);
            if(!strncmp(strtab+name,".text",5))
            {
                goal=i;
                break;
            }
            //printf("%s\n",strtab+name);
        }
        printf("goal= %d\n",goal);
        
        temp=shoffset+(goal*shentsize)+16;
        
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
        /*
        phentsize=*((unsigned short*)(buffer+42));
        printf("%x\n",phentsize);
        phnum=*((unsigned short*)(buffer+44));
        printf("%x\n",phnum);

        //fseek(pFile,phoffset+(phnum*phentsize),SEEK_SET);
        fseek(pFile,308,SEEK_SET);
        fgets(buffer,500,pFile);
        printf("len: %d\n",strlen(buffer));
        printf("%s\n",buffer);
        fgets(buffer,500,pFile);
        printf("len: %d\n",strlen(buffer));
        printf("%s\n",buffer);
        */
        free(strtab);
        free(text0);
        fclose(pFile);
    }
    return 0;
}
