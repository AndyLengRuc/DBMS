#include "stdio.h"
#include "structfile.h"
#include "memblock.h"

int main()
{

	//�ļ� 
	createSysFile("../data/system01.dbf", 2*SIZE_PER_PAGE);
 	struct SysFile sf1;
 	readSysFile(&sf1);
	createDataFile("../data/datafile01.dbf", 2*SIZE_PER_PAGE, &sf1);
	saveSysFile(&sf1);	
	DisplayDBInfo(&sf1);

		
	//�����ʹ�� 
	struct MemBlock *buff;
	buff = malloc(sizeof(struct MemBlock));
	allocateBuff(buff);	  //��������ʹ��

	//���ҿ��л�������ʾ 
	int i, j;
	for(i=0; i<=40; i++)
	{
		j = queryFreeBuff(buff, &sf1);
		printf("%d, ", j);
	};
	printf("\n");
	
	//д������ 
	writeDate(&sf1, buff, 1);
}
