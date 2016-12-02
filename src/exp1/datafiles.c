#include "stdio.h"
#include "structfile.h"
#include "memblock.h"


/**
 * @brief ���������ļ�����д�����ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 

 int isFileExist(char *fileName)
 {

 	//�жϸ������ļ��Ƿ���ڣ����ڷ���TRUE, ���򷵻�FALSE

	if( !(access(fileName,0)) ) 
		return TRUE;
	else 
		return FALSE;
 }

 int createSysFile(char sysFileName[40], long fileSize)
 {
/**
 * @brief ����ϵͳ�ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 	FILE *fp;
 	char ch;
 	struct SysFile sf1;
	
	if(isFileExist(sysFileName) == FALSE)
	{
		/** ��ʼ���ļ�ͷ��Ϣ **/ 
		sf1.sizePerPage = SIZE_PER_PAGE;
		sf1.buffNum = MAX_BUFF_SIZE; 
	 	sf1.fileNumber = 1;  //��ʹ�����ݿ�ʱ�������ļ�����Ϊ1
	 	
	 	fp=fopen(sysFileName, "wb");
	 	
	 	struct FileMeta file1;
	 	file1.file_deleted = 0;
	 	strcpy(file1.fileName,sysFileName);
	 	sf1.files[0]=file1;
	 	
	 	fwrite(&sf1, sizeof(struct SysFile), 1, fp );
	 	rewind(fp);
		fseek( fp, (fileSize/SIZE_PER_PAGE)*SIZE_PER_PAGE, SEEK_SET);	//ָ��ָ���ļ�δβ 
		ch = '\0';
		fwrite(&ch, sizeof(char), 1, fp);
	 	fclose(fp);

	}
 	return 0;
 };
 
int saveSysFile(struct SysFile *sf1)
 {
/**
 * @brief ����ϵͳ�ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/22 
 **/
	FILE *fp;
 	fp=fopen("../data/system01.dbf", "rb+");
 	rewind(fp);
 	fwrite(sf1, sizeof(struct SysFile), 1, fp ); //������д���ļ� 
	fclose(fp);
 }
 
 int readSysFile(struct SysFile *sf1)
 {
/**
 * @brief ��ϵͳ�ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/22 
 **/
	FILE *fp;
 	fp=fopen("../data/system01.dbf", "rb");
 	rewind(fp);
 	fread(sf1, sizeof(struct SysFile),1,fp);//���ļ����ݶ��뵽����
	fclose(fp);
 }
 
 int createDataFile(char dataFileName[40], long fileSize, struct SysFile *sf1)
 {
/**
 * @brief ���������ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 	FILE *fp;
 	char ch;
 	struct DataFileHead df1;
	if(isFileExist(dataFileName) == FALSE)
	{
		/** ��ʼ���ļ�ͷ��Ϣ **/ 
		df1.pageOfFile = fileSize/SIZE_PER_PAGE;
	 	df1.freeCount = fileSize/SIZE_PER_PAGE-1;
		
		int i;
		df1.isPageFree[0]; 
		for(i=0; i<Max_Page_Per_File; i++)
		{
			df1.isPageFree[i] = '0';  //��ʹ��ʱ���е�page���ǿյ� 
		}
	 	/** ��ʼ���ļ�ͷ��Ϣ **/

	 	fp=fopen(dataFileName, "wb");
	 	
	 	fwrite(&df1, sizeof(struct DataFileHead), 1, fp );	//д�ļ�ͷ��Ϣ 
		fseek( fp, df1.pageOfFile*SIZE_PER_PAGE, SEEK_SET);	//����һ�������ļ� 
		ch = '\0';
		fwrite(&ch, sizeof(char), 1, fp);
	 	fclose(fp);
	 	
	 	
	 	//�����������ļ�֮�󣬸���ϵͳ�ļ���Ϣ 
	 	struct FileMeta df;
	 	df.file_id=sf1->fileNumber;
	 	df.file_deleted = 0;
	 	strcpy(df.fileName, dataFileName);
	 	
	 	sf1->files[df.file_id] = df;
	 	sf1->fileNumber++;
	 	
		saveSysFile(sf1);
	}
 	return 0;
 };
 
 int getFreePage(long fid, struct SysFile *sf)
 {
 	long freePageNo;
 	struct DataFileHead dfh;
 	char *filename;
 	
	readSysFile(sf); 	
	filename = filename=sf->files[fid].fileName;
	FILE *fp;
	fp=fopen(filename, "rb+");
	fread(&dfh, SIZE_PER_PAGE, 1,fp);	//��ȡ�ļ�ͷ��Ϣ
	
	if(dfh.freeCount == 0)  //���ļ��Ѿ�û�п��п� 
	{
		return -1;   //û�п��п������£�����-1; 
	}
	else
	{
		int i;
		for(i=1; i<= dfh.pageOfFile; i++)
		{
			if(dfh.isPageFree == TRUE)
				return i;	//�ҵ���һ�����п�ʱ�����ؿ�� 
		} 	
	}

	return freePageNo;
 };
 
 int writeNewPageToFile(struct Page page1, long fid, struct SysFile *sf, FILE *fp)
 {
/**
 * @brief дһ���¿鵽�����ļ�
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 	printf("Write a page to file.\n");
 	//FILE *fp;
 	/*FILE *fp;
 	char *filename;*/
 	/*filename=sf->files[fid].fileName;*/
 	/*fp=fopen(filename, "wb+");*/
 	rewind(fp);
 	fseek(fp, SIZE_PER_PAGE, SEEK_SET);
	fwrite(&page1, SIZE_PER_PAGE, 1, fp ); //������д���ļ� 
 	/*fclose(fp);*/
 };


 int readPageFromFile(long fid, long pageno, struct MemBlock *pofm, long pageId, struct SysFile *sf1)
 {
/**
 * @brief �������ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
	char *filename;
	filename=sf1->files[fid].fileName;
	
	printf("Read a page from file.\n");
	FILE *fp;
	//readSysFile(&sf1); 
 	struct DataFileHead df1;
	puts(filename);
	puts("hello world");
 	fp=fopen(filename, "rb");
 	rewind(fp);

 	fseek(fp, SIZE_PER_PAGE*pageno, SEEK_SET);
	fread(pofm->data[pageId][0], SIZE_PER_PAGE, 1, fp);//���ļ����ݶ��뵽����
	fclose(fp);
 };
 

 
int DisplayDBInfo(struct SysFile *sf1)
{
	printf("************Sys file information*************\n");
	printf("Size per page: %d\n", sf1->sizePerPage);
	printf("Buffer size: %d\n", sf1->buffNum);
	printf("\nData file number of db: %d\n", sf1->fileNumber);
	printf("File list:\n");
	
	int i;
	for(i=0; i<=sf1->fileNumber-1;i++)
	{
			printf("File name: %s\n", sf1->files[i].fileName);	
	}

	printf("************Sys file information*************\n");
}


 int initDataFile()
 {
	//��ʹ�����ݿ��ļ� 
 	createSysFile("../data/system01.dbf", 2*SIZE_PER_PAGE);
 	struct SysFile sf1;
 	readSysFile(&sf1);
	createDataFile("../data/datafile01.dbf", 2*SIZE_PER_PAGE, &sf1);
	saveSysFile(&sf1);
	
	DisplayDBInfo(&sf1);
 };
 
 int initNewPage(struct Page *page1)
 {
	page1->free_size = SIZE_PER_PAGE- 100;
	page1->recordNumber = 0;
 }
