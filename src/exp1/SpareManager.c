#include "stdio.h"
#include "structfile.h"
#include "string.h"


int getFreePage(char *filename)
{
	long freePageAddress; 
	struct DataFile filehead;    //�ļ�ͷ 
	
	FILE *fp;
	fp=fopen(filename,"rb");
	fread(&filehead,sizeof(struct DataFile),1,fp);
	
    int Num=filehead.pageAvail;
    printf("�ļ���ҳ��%d",Num);
    
    if (Num<=0)
    {
    	return -1;		//���û�п��пռ���Է��䣬����-1 
    }
    else
	{
    		freePageAddress = filehead.pagefreeHead;
			filehead.pageHead=filehead.pagefreeHead;
			filehead.freeCount--;
			filehead.pagefreeTail++;
			filehead.pagefreeHead++;  
    }    
 	fwrite(&filehead, sizeof(struct DataFile), 1, fp); //������д���ļ� 
	fclose(fp);
	return freePageAddress;
	
};
