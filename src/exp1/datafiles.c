#include "stdio.h"
#include "structfile.c"
#include "displayDB.c"

/**
 * @brief ���������ļ�����д�����ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 
 struct student
 {
 	char id[11];
 	char name[10];
 };
 
 int createSysFile(char sysFileName[40], long fileSize)
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
 	struct SysFile sf1;
	
	/** ��ʼ���ļ�ͷ��Ϣ **/ 
	sf1.sizePerPage = SIZE_PER_PAGE;
 	sf1.fileNumber = 1;  //��ʹ�����ݿ�ʱ�������ļ�����Ϊ1
 	/** ��ʼ���ļ�ͷ��Ϣ **/
 	
 	printf("Create the system file.\n");
 	fp=fopen(sysFileName, "wb");
 	
 	struct File file1;
 	file1.file_deleted = 0;
 	strcpy(file1.fileName,sysFileName);
 	sf1.files[0]=file1;
 	
 	fwrite(&sf1, sizeof(struct SysFile), 1, fp );
	fseek( fp, (fileSize/sf1.sizePerPage)*sf1.sizePerPage, SEEK_SET);	//����һ�������ļ� 
	ch = '\0';
	fwrite(&ch, sizeof(char), 1, fp);
 	fclose(fp);
 	printf("Create the system file successfully.\n");

 	return 0;
 };
 
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
 	struct DataFile df1;
	
	/** ��ʼ���ļ�ͷ��Ϣ **/ 
	df1.pageOfFile = fileSize/8192;
 	df1.freeCount = fileSize/8192-1;
	df1.pageDataMemoryHead = 2; 
 	/** ��ʼ���ļ�ͷ��Ϣ **/
 	
 	printf("Create datafile.\n");
 	fp=fopen(dataFileName, "wb");
 	
 	fwrite(&df1, sizeof(struct DataFile), 1, fp );
	fseek( fp, df1.pageOfFile*SIZE_PER_PAGE, SEEK_SET);	//����һ�������ļ� 
	ch = '\0';
	fwrite(&ch, sizeof(char), 1, fp);
 	fclose(fp);
 	
 	
 	
 	//�����������ļ�֮�󣬸���ϵͳ�ļ���Ϣ 
 	sf1->fileNumber++;
 	sf1->files[sf1->fileNumber-1].file_deleted = 0;
	strcpy(&(sf1->files[sf1->fileNumber-1].fileName), dataFileName);
	//puts(dataFileName);
 	printf("Create datafile successfully.\n");
 	return 0;
 };
 

 int writePageToFile()
 {
/**
 * @brief д�����ļ� 
 *
 * @param 
 * @return  int 
 *
 * @author Andy
 * @date 2016/10/16 
 **/
 printf("Write a page to file.\n");
 	FILE *fp;
 	struct student s1;

	strcpy( s1.id, "2016000669");
 	strcpy( s1.name, "���ѷ�");
 	fp=fopen("datafile1.dbf", "rb+");
 	fseek(fp, 8192, SEEK_SET);
	fwrite(&s1, sizeof(struct student), 1, fp ); //������д���ļ� 
 	fclose(fp);
 }; 


 int readDataFile()
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
 	printf("Read a page from file.\n");
	FILE *fp;
 	struct student s2;
 	struct DataFile df2;
 	fp=fopen("../data/datafile1.dbf", "rb");
 	rewind(fp);
 	fread(&df2, sizeof(struct DataFile),1,fp);//���ļ����ݶ��뵽����
 	
 	fseek(fp, 8192, SEEK_SET);
	fread(&s2, sizeof(struct student),1,fp);//���ļ����ݶ��뵽����

	fclose(fp);

	printf("************DB file information*************\n");
	printf("Page of file: %d\n", df2.pageOfFile);
	printf("Size per page: %d\n", SIZE_PER_PAGE);
	printf("Number of free page: %d\n", df2.freeCount);
	
	printf("\n************Data information*************\n");
	printf("Student id:%s\n", s2.id);
	printf("Student name:%s\n", s2.name);
 };
 
 
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
 	printf("Read system file.\n");
	FILE *fp;
 	fp=fopen("../data/system01.dbf", "rb");
 	rewind(fp);
 	fread(sf1, sizeof(struct SysFile),1,fp);//���ļ����ݶ��뵽����
	fclose(fp);
 }
 
int DisplayDBInfo(struct SysFile *sf1)
{
	printf("************Sys file information*************\n");
	printf("Data file number of db: %d\n", sf1->fileNumber);
	printf("Size per page: %d\n", sf1->sizePerPage);
	printf("File of the db:\n");
	
	int i;
	for(i=0; i<=sf1->fileNumber-1;i++)
	{
			printf("File name: %s\n", sf1->files[i].fileName);	
	}

	printf("************Sys file information*************\n");
}


 int initDB()
 {
 	createSysFile("../data/system01.dbf", 1024*1024);
 	struct SysFile sf1;
 	readSysFile(&sf1); 
 	DisplayDBInfo(&sf1);
 	
	createDataFile("../data/dataFile01.dbf", 1024*1024, &sf1);
	
	DisplayDBInfo(&sf1);
	//writePageToFile();
	//readDataFile();
 };
 
 