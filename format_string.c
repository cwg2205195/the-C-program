#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
        char buf[10];
        strcpy(buf,argv[1]);
        printf("The right way :buf@%p\n%s\n",&buf,buf);
        printf("The wrong way : \n");
        printf(buf);
        return 0;
}
