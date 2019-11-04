#include<stdio.h>
#include<stdlib.h>
void* yalloc;
unsigned int noAddrBytes;

unsigned int allocBytesForAddr(void* yalloc,int totalBytes)
{
	return (((totalBytes*20/100)/2)*2);
}
void setAddrBytesZero(void* yalloc)
{
	unsigned char *temp=(unsigned char*)yalloc;
	for(int i=0;i<noAddrBytes;i++)
		temp[i]=0x00;
	return;
}
int checkFreeByets(int bytes)
{
	int i,j;
	int cnt = bytes;
	int startAddr =-1;
	char *temp = (char*)yalloc;
	for(i=0;i<(noAddrBytes/2);i++)
	{
		for(j=0;j<=7;j++)
		{
			if((temp[i] & (1<<j))==0)
			{
				cnt--;
				if(startAddr==-1)
				{
					startAddr = i*8 + (j);
				}
				if(cnt==0)
					return startAddr;
			}
			else
			{
				cnt = bytes;
				startAddr = -1;
			}
		}
	}
	return -1;//no free block || no consecutive free block
}

void myMalloc(int bytes)
{	
	int startAddr,addrByte,addrBit;
	if(bytes<=0)
	{
		fprintf(stdout,"\n\terror: required bytes must greater than 0 !!\n");
		return;
	}
	if((startAddr=checkFreeByets(bytes))==-1)
	{
		fprintf(stdout,"\n\terror: required bytes are not available !!\n");
		return;
	}
	printf("\nstart=%d",startAddr);
	addrByte=startAddr/8;
	addrBit=startAddr%8;
	unsigned char* temp=(unsigned char*)yalloc;
	temp[addrByte] |= (1<<addrBit);	
	temp[addrByte+(noAddrBytes/2)] |= (1<<addrBit);	
	bytes--;
	addrBit++;
	if(addrBit==8)
	{
		addrBit=0;
		addrByte++;
	}
	while(bytes>0)
	{
		temp[addrByte] |= (1<<addrBit);	
		temp[addrByte+(noAddrBytes/2)] &= ~(1<<addrBit);	
		bytes--;
		addrBit++;
		if(addrBit==8)
		{
			addrBit=0;
			addrByte++;
		}
	}
	return;
	//return (alloc+startAddr);
}
void myFree(int startAddr)
{
	if(startAddr<0)
	{
		fprintf(stdout,"\n\terror: start address is wrong!!\n");
		return;
	}
	unsigned char* temp=(unsigned char*)yalloc;
	int addrByte,addrBit;
	addrByte=startAddr/8;
	addrBit=startAddr%8;
	
	temp[addrByte] &= ~(1<<addrBit);
	temp[addrByte+(noAddrBytes/2)] &= ~(1<<addrBit);	
	
	addrBit++;
	if(addrBit==8)
	{
		addrBit=0;
		addrByte++;
	}
while((temp[addrByte]&(1<<addrBit))>0&&(temp[addrByte+noAddrBytes/2]&(1<<addrBit))==0
&& addrByte<(noAddrBytes/2))
	{
		temp[addrByte] &= ~(1<<addrBit);
		addrBit++;
		if(addrBit==8)
		{
			addrBit=0;
			addrByte++;
		}		
	}
}
void displayAddr()
{
	unsigned char* temp=(unsigned char*)yalloc;
		printf("Actual Address Bytes\n");	
	for(int i=0;i<noAddrBytes/2;i++)
	{
		for(int j=0;j<8;j++)
		{
			if((temp[i] & (1<<j))==0)
				printf("0 ");	
			else
				printf("1 ");	
		}
		printf("\n");	
	}
		printf("Address Bytes Locations\n");	
	for(int i=noAddrBytes/2;i<noAddrBytes;i++)
	{
		for(int j=0;j<8;j++)
		{
			if((temp[i] & (1<<j))==0)
				printf("0 ");	
			else
				printf("1 ");	
		}
		printf("\n");	
	}
}

int  main(int argc,char** argv)
{
	if(argc!=2)
	{
		fprintf(stdout,"\n enter required bytes in argument !!");
		return -1;
	}
	if((yalloc=malloc(atoi(argv[1])))==NULL)
	{
		fprintf(stdout,"\n error in memory allocation !!");
		return -1;
	}
	if((noAddrBytes=allocBytesForAddr(yalloc,atoi(argv[1])))<1)
	{
		fprintf(stdout,"\n error in address byte allocation !!");
		return -1;
	}
	setAddrBytesZero(yalloc);
	fprintf(stdout,"\nallocated addr  = %ld",(long int)yalloc);
	fprintf(stdout,"\nno Addr Bytes   = %d",noAddrBytes);
	fprintf(stdout,"\nAvailable Bytes = %d",noAddrBytes/2*8);

	unsigned int bytes,choice;
	do{
		fprintf(stdout,"\n1.Allocate Bytes 2.Free Bytes 3.Display 4.Exit");
		fprintf(stdout,"\nEnter your choice:");
		scanf("%d",&choice);
		if(choice==1)
		{
			fprintf(stdout,"\nEnter how many bytes allocate:");
			scanf("%d",&bytes);
			myMalloc(bytes);		
		}
		if(choice==2)
		{
			fprintf(stdout,"\nEnter start addr of bytes for deallocation:");
			scanf("%d",&bytes);
			myFree(bytes);
		}
		if(choice==3)
			displayAddr();
		
	}while(choice!=4);
}
