#include "stdio.h"
#include "structfile.h"
int main()
{
	//�ļ� 
	createSysFile("../data/system01.dbf", 2*SIZE_PER_PAGE);
 	struct SysFile sf1;
 	readSysFile(&sf1);
	createDataFile("../data/datafile01.dbf", 2*SIZE_PER_PAGE, &sf1);
	saveSysFile(&sf1);	
	DisplayDBInfo(&sf1);
	
	writeDate(1);
	//���� 
	
	/*struct MemBlock *pofm;
	linkQueue q;
 	initQueue (q);
	pofm = allocateBuff(32*SIZE_PER_PAGE);
	queryFreeBuff(pofm, sf1, q, 0, 1);
	*/
	//initDataFile();   //���ݿ��ļ���ʹ�� 
	//initMemory();	  //��������ʹ�� 
}