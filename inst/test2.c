#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readinst(char* ptr2){
    int first=9;
    int last=strlen(ptr2)-4;
    int value=0;
    int hex=16;
    char a=0;

    for(int ii=first;ii<last;ii+=8)
    {
       for(int iii=0;iii<8;iii++)
       {
           a=*(ptr2+ii+iii);
           value+=hex*(a-48+(~((a-58)>>7)*7));
           if(iii%2==0) hex>>=4;
           else hex<<=12;
       }
       value=0;
       hex=16;
    }
       

}
int main()
{
   FILE *pFile = NULL;
   char buffer[43];
   int size=0;
   pFile= fopen("aaa.hex","r");
   if(pFile == NULL)
   {
       return 0;
   }
   else
   {
       fseek(pFile,0,SEEK_END);
       size=ftell(pFile);
       fseek(pFile,0,SEEK_SET);

       fgets(buffer,43,pFile);
       while(strlen(buffer)!=13)
       {
       fgets(buffer,43,pFile);
       readinst(buffer); 
       }
       printf("hello\n");
       fclose(pFile);
   }

    return 0;
}
