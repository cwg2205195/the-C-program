#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<windows.h>
#include<winnt.h>
#define SIZE 100
int main(int argc,char * argv[])
{
	FILE *fp;
	char *buf,*p,str[SIZE],*t;
	int offset=0,fsize,i; 		//fsize记录文件大小，offset用于取内存的偏移，i用于控制循环计数 
	if(2!=argc)
	{
		printf("Usage: dumps.exe file_path\n");
		return 0;
	}
	if(NULL== (fp=fopen(argv[1],"r")))
	{
		printf("Can not open file ~\n");
		return 0;
	}
	
	//获取文件大小 分配内存，并写入内存中 
	fseek(fp,0,SEEK_END);
	fsize=ftell(fp);		
	fseek(fp,0,SEEK_SET);
	p=buf=(char*)malloc(fsize);
	fread(buf,1,fsize,fp);
	
	//
	while((p-buf)<fsize)
	{
		memset(str,0,SIZE);//清空输出缓冲区 
		//搜索字符串    出来的情况：1缓冲区满了 2遇到0 ？ 
		for(i=0;i<SIZE;i++)
		{
			if(!i&&isascii(*p))
				t=p;
			if(isgraph(*p))
			{
				str[i]=*p;
			}
			else if (0==*p)
			{
				str[i]=*p;
				p++;
				offset++;
				break;
			}
			else
			{
				str[i]=0;
				offset++;
				p++;
				break;
			} 
			offset++;
			p++;
		}
		str[SIZE-1]=0;
		//输出结果？
		if(strlen(str)>=5)
			{
				printf("%08X : ",t-buf);
				printf("%s\n",str);
				//puts(str);
			}
	}
	free(buf);
	fclose(fp);
	return 0;
}
