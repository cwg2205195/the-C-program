#include<windows.h>
#include<winnt.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
int fd;
FILE *fp;
IMAGE_DOS_HEADER DOS_head;// dos head
IMAGE_NT_HEADERS32 NT_head;//  NT head
IMAGE_NT_HEADERS64 NT_head64;//64 bits head
IMAGE_SECTION_HEADER *section_p,*tmp;//section information
IMAGE_OPTIONAL_HEADER32 opt32;
IMAGE_OPTIONAL_HEADER64 opt64;
IMAGE_IMPORT_DESCRIPTOR *iats;
typedef struct IID_List
{
	IMAGE_IMPORT_DESCRIPTOR *p;
	struct IID_List *next;	
}IID_LIST;
IID_LIST iid_head;
void file_head(int offset,IMAGE_NT_HEADERS32 NT_head)	//Done
{
	/*typedef struct _IMAGE_FILE_HEADER {
      WORD Machine;
      WORD NumberOfSections;
      DWORD TimeDateStamp;
      DWORD PointerToSymbolTable;
      DWORD NumberOfSymbols;
      WORD SizeOfOptionalHeader;
      WORD Characteristics;
    } IMAGE_FILE_HEADER,*/
    /* typedef struct _IMAGE_IMPORT_DESCRIPTOR {
      __C89_NAMELESS union {
	DWORD Characteristics;
	DWORD OriginalFirstThunk;
      } DUMMYUNIONNAME;
      DWORD TimeDateStamp;

      DWORD ForwarderChain;
      DWORD Name;
      DWORD FirstThunk;
    } IMAGE_IMPORT_DESCRIPTOR;*/
    offset+=4;
	printf("\nNT_HEADER32\n\n\n");
	printf("Offset		value		description\n");
	printf("------------------------------------\n");
	printf("%08X	%04X		machine code \n",offset,NT_head.FileHeader.Machine);
	printf("%08X	%04X		Number of Sections\n",offset+sizeof(NT_head.FileHeader.NumberOfSections),NT_head.FileHeader.NumberOfSections);
	printf("%08X	%04X		size of optional header \n",offset+16,NT_head.FileHeader.SizeOfOptionalHeader);
	printf("%08X	%04X		characteristic \n",offset+18,NT_head.FileHeader.Characteristics);
	printf("------------------------------------\n");
	printf("\n\n\n");
	return ;
}
void Optional_head(int offset,IMAGE_NT_HEADERS32 NT_head)
{
	/*
	typedef struct _IMAGE_OPTIONAL_HEADER {

      WORD Magic;
      BYTE MajorLinkerVersion;
      BYTE MinorLinkerVersion;
      DWORD SizeOfCode;
      DWORD SizeOfInitializedData;
      DWORD SizeOfUninitializedData;
      DWORD AddressOfEntryPoint;
      DWORD BaseOfCode;
      DWORD BaseOfData;
      DWORD ImageBase;
      DWORD SectionAlignment;
      DWORD FileAlignment;
      WORD MajorOperatingSystemVersion;
      WORD MinorOperatingSystemVersion;
      WORD MajorImageVersion;
      WORD MinorImageVersion;
      WORD MajorSubsystemVersion;
      WORD MinorSubsystemVersion;
      DWORD Win32VersionValue;
      DWORD SizeOfImage;
      DWORD SizeOfHeaders;
      DWORD CheckSum;
      WORD Subsystem;
      WORD DllCharacteristics;
      DWORD SizeOfStackReserve;
      DWORD SizeOfStackCommit;
      DWORD SizeOfHeapReserve;
      DWORD SizeOfHeapCommit;
      DWORD LoaderFlags;
      DWORD NumberOfRvaAndSizes;
      IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
    } IMAGE_OPTIONAL_HEADER32
	*/
	printf("\n\n\n\t\t\tNT_Optional_Head\n\n\n");
	if(NT_head.OptionalHeader.Magic==0x10b)
		printf("\t\t\t32 bit head\nEntry point@\t%08X\n",NT_head.OptionalHeader.ImageBase+NT_head.OptionalHeader.AddressOfEntryPoint);
	else if  (NT_head.OptionalHeader.Magic==0x20b)
		printf("\t\t\t64 bit head\n");
	else
		{
			printf("\t\t\tUnknow head !\nExit!\n");
			return;
		}
	printf("Offset		value		description\n");
	printf("------------------------------------\n");
	printf("%08X	%08X	Entry point(RVA) \n",offset+16,NT_head.OptionalHeader.AddressOfEntryPoint);
	printf("%08X	%08X	Image Base(RVA) \n",offset+28,NT_head.OptionalHeader.ImageBase);
	printf("%08X	%08X	Base of Code \n",offset+20,NT_head.OptionalHeader.BaseOfCode);
	printf("%08X	%08X	Base of Data \n",offset+24,NT_head.OptionalHeader.BaseOfData);
	printf("%08X	%08X	Size of Code \n",offset+4,NT_head.OptionalHeader.SizeOfCode);
	printf("%08X	%08X	Size of Initialized data \n",offset+8,NT_head.OptionalHeader.SizeOfInitializedData);
	printf("%08X	%08X	Size of Uninitialized data \n",offset+12,NT_head.OptionalHeader.SizeOfUninitializedData);
 	printf("%08X	%08X	Section Alignment \n",offset+32,NT_head.OptionalHeader.SectionAlignment);
 	printf("%08X	%08X	File Alignment \n",offset+36,NT_head.OptionalHeader.FileAlignment);
 	printf("%08X	%08X	Size of Image \n",offset+48,NT_head.OptionalHeader.SizeOfImage);
 	printf("%08X	%08X	Size of Headers \n",offset+52,NT_head.OptionalHeader.SizeOfHeaders);
	printf("%08X	%08X	Number of Rva And Sizes \n",offset+80,NT_head.OptionalHeader.NumberOfRvaAndSizes);
 	printf("------------------------------------\n");
 } 
 void IAT(IMAGE_DATA_DIRECTORY data)
{
	unsigned int offset=data.VirtualAddress,offset1;
	IMAGE_IMPORT_DESCRIPTOR IID;
	IMAGE_SECTION_HEADER  *tmp=section_p;
	char ch;
	int i=4;
	printf("\n\n3 RVA=%X && ftell=%d\n\n",offset,ftell(fp));
	while(offset>section_p->VirtualAddress)
	{
		printf("Section Info: RVA=%X RawOffset =%X\n",section_p->VirtualAddress,section_p->PointerToRawData);
		section_p++;
	}
	section_p--;
	printf("Section Info: RVA=%X RawOffset =%X\n",section_p->VirtualAddress,section_p->PointerToRawData);
	offset=offset-section_p->VirtualAddress+section_p->PointerToRawData; //计算出文件中的偏移地址 
	printf("\n\n1 RVA=%X && ftell=%d\n\n",offset,ftell(fp));
	if(!fseek(fp,offset,DOS_head.e_lfanew))
		printf("Fseek successful!\n");
	fread(&IID,sizeof(IMAGE_IMPORT_DESCRIPTOR),1,fp);
//	lseek(fd,offset,DOS_head.e_lfanew);
//	read(fd,&IID,sizeof(IMAGE_IMPORT_DESCRIPTOR)); 
	printf("\n\n2 RVA=%X && ftell=%d\n\n",offset,ftell(fp));
	while(IID.Name>section_p->VirtualAddress)
	section_p++;
	section_p--;
	offset1=IID.Name-section_p->VirtualAddress+section_p->PointerToRawData;
	printf("In IAT \n Name offset @ %x %d\n",offset1,offset1);
//	printf("Offset=%08X=%d\n",offset,offset);
//	printf("Size=%X=%d",data.Size,data.Size);
	section_p=tmp;
	return;
}
void dataDir(IMAGE_NT_HEADERS32 NT_head,IMAGE_SECTION_HEADER *section_p)
{
	int i=0,j; 
	IMAGE_SECTION_HEADER *tmp=section_p;//back up the original pointer
	IAT(NT_head.OptionalHeader.DataDirectory[1]);
	printf("\n------------------------------------\n");
	printf("Offset		value		description\n");
	for(i;i<NT_head.FileHeader.NumberOfSections;i++)
	{
		j=0;
		printf("Name:");
		while((section_p+i)->Name[j]!=0)
		printf("%c",(section_p+i)->Name[j++]);
		printf("\n%08X	%08X	Raw Offset\n",0,(section_p+i)->PointerToRawData);
		printf("%08X	%08X	Raw Size\n",0,(section_p+i)->SizeOfRawData);
		printf("%08X	%08X	Virtual Address\n\n",0,(section_p+i)->VirtualAddress);
	//	section_p+=sizeof(IMAGE_SECTION_HEADER);
	//break;
	}
	/*typedef struct _IMAGE_DATA_DIRECTORY {
      DWORD VirtualAddress;
      DWORD Size;
    } IMAGE_DATA_DIRECTORY,*PIMAGE_DATA_DIRECTORY;*/
    section_p=tmp;//  restore the original pointer
	return;
}
void menu(int offset,IMAGE_NT_HEADERS32 NT_head,IMAGE_SECTION_HEADER *section_p) 
{
	int choice;
	printf("What info do u need?\n1----->File info \n2----->Optional info\n3----->Clean\n4---->Section Info\n5---->Exit\n");
	scanf("%d",&choice);
	while(choice!=5)
	switch(choice)
	{
		case 1:
			file_head(offset,NT_head);
			printf("What info do u need?\n1----->File info \n2----->Optional info\n3----->Clean\n4---->Section Info\n5---->Exit\n");
			scanf("%d",&choice);
			break;
		case 2:
			Optional_head(offset+sizeof(NT_head.FileHeader),NT_head);
			printf("What info do u need?\n1----->File info \n2----->Optional info\n3----->Clean\n4---->Section Info\n5---->Exit\n");
			scanf("%d",&choice);
			break;
		case 3:
			system("cls") ;
			printf("What info do u need?\n1----->File info \n2----->Optional info\n3----->Clean\n4---->Section Info\n5---->Exit\n");
			scanf("%d",&choice);
			break;
		case 4:
			dataDir(NT_head,section_p);
			printf("What info do u need?\n1----->File info \n2----->Optional info\n3----->Clean\n4---->Section Info\n5---->Exit\n");
			scanf("%d",&choice);
			break;
		
		case 5:
			return;
			//file_head(offset,NT_head);
	//Optional_head(offset+sizeof(NT_head.FileHeader),NT_head);
	}
	return;
}
int main()
{
	char buf[100],ch;
	int i,n,flag=0,j=0,tmp1,tmp2,section_addr[16]={0},count,file_size;
	long offset,offset1;
	IID_LIST *cp,*pp;
	DWORD offset2;
	// ------------Open a file ---------------
	printf("Open file path:");
	scanf("%s",buf);
	if((fp=fopen(buf,"r"))==NULL)	//打开文件 
	{
		printf("can not open file \n");
		exit(0);
	}
	fd=fileno(fp);
	fseek(fp,0,SEEK_END);//get the file size
	file_size=ftell(fp);
	rewind(fp);
	// -----------read DOS head-----------------
	fread(&DOS_head,sizeof(IMAGE_DOS_HEADER),1,fp);
	if(DOS_head.e_magic!=0x5a4d)	//判断是否为MZ 
	{
		printf("Dos head missing ! Or it is not a PE \n Exit .");
		exit(0);
	}
	//--------------Read NT head-------------------
	offset=DOS_head.e_lfanew;	//nt头的偏移 
	fseek(fp,offset,SEEK_SET);
	fread(&NT_head,sizeof(IMAGE_NT_HEADERS32),1,fp);
	if(NT_head.Signature!=0x4550)	//判断是否为PE 
	{
		printf("Not PE ! \n Exit .\n");
		exit(0);
	}
	//if it's a 64 bits file
	if(NT_head.FileHeader.SizeOfOptionalHeader==sizeof(IMAGE_NT_HEADERS64))
		{
			fseek(fp,offset,SEEK_SET);
			fread(&NT_head64,sizeof(IMAGE_NT_HEADERS64),1,fp);
			flag=1;//64bits  flag=1
		}
		
	//------------Read Section informations ------------------
	n=NT_head.FileHeader.NumberOfSections;
	section_p=(IMAGE_SECTION_HEADER*)malloc(n*sizeof(IMAGE_SECTION_HEADER));//Allocating section space
	fread(section_p,sizeof(IMAGE_SECTION_HEADER),n,fp);		//读取n个节区的内容 
	tmp=section_p;		//back up the original pointer
	for(i=0;i<n;i++)
		{
			section_addr[i]=section_p->VirtualAddress;
			section_p++;
		}
	section_p=tmp;		//resrote section pointer
	
	//---------------Read IAT (IMPORT ADDRESS TABLE)-------------------
	if(!flag)	//32bits PE file
		{		
			//offset1 serves as pointer to  IAT(IID:IMAGE_IMPORT_DESCRIPOTR)
			offset1=NT_head.OptionalHeader.DataDirectory[1].VirtualAddress;
			i=0;
			while(offset1-section_addr[i]>0)i++;
			if(i>0)
				i--;	//Calculating the raw offset
			offset1=offset1-(section_p+i)->VirtualAddress+(section_p+i)->PointerToRawData;
			fseek(fp,offset1,SEEK_SET);//Go to the IID 
			//Allocate some space for the IIDs
			iats=(IMAGE_IMPORT_DESCRIPTOR*)malloc(NT_head.OptionalHeader.DataDirectory[1].Size);
			fread(iats,NT_head.OptionalHeader.DataDirectory[1].Size,1,fp);
			count=NT_head.OptionalHeader.DataDirectory[1].Size/sizeof(IMAGE_IMPORT_DESCRIPTOR);

			

			for(i=0;i<count;i++)
			{	//(((((((output the DLL names )))))))))
				tmp1=(iats+i)->Name;	//tmp1 serves as the offset to the Name
				j=0;
				while(tmp1-section_addr[j]>0)j++;
				if(j>0)
					j--;
				tmp1=tmp1-section_addr[j]+(section_p+j)->PointerToRawData;
				if(tmp1>file_size) //in case the raw offset larger than the file_size
					break;
				fseek(fp,tmp1,SEEK_SET);
				while((ch=fgetc(fp))!='.')
					if(isalpha(ch)||(ch>='0'&&ch<='9'))
					putc(ch,stdout);
					else
						break;
				if(i<count-1)printf(".dll\n");
				
				//(((output the API name address)))
				tmp2=(iats+i)->OriginalFirstThunk;
				j=0;
				while(tmp2-section_addr[j]>0)j++;
				if(j>0)
					j--;
				tmp2=tmp2-section_addr[j]+(section_p+j)->PointerToRawData;
				fseek(fp,tmp2,SEEK_SET);
				fread(&offset2,sizeof(DWORD),1,fp);
			/*	j=0;
				while(offset2-section_addr[j]>0)
					if(j+1<NT_head.FileHeader.NumberOfSections)j++;
					else	break;
				if(j>0)
					j--;
				offset2=offset2-section_addr[j]+(section_p+j)->PointerToRawData;*/
				printf("API Name RVA @ %X\n",offset2);
			}
		}
	else		//64bits PE　file 
		{
			n=NT_head64.FileHeader.NumberOfSections;
		}	

	menu(offset,NT_head,section_p);
	exit(0);
}
