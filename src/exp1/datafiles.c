#include "stdio.h"
#include "structfile.c"

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
 	char id[10];
 	char name[10]; 
 };
 
 int createDataFile(long fileSize)
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
 	df1.sizePerPage = 8192;
 	df1.freeCount = fileSize/8192-1;
 	df1.pagefreeHead = 2;
	df1.pagefreeTail = 128;
	df1.pageDataMemoryHead = 2; 	
 	/** ��ʼ���ļ�ͷ��Ϣ **/
 	
 	
 	fp=fopen("datafile1.dat", "wb"); 	
 	fwrite(&df1, sizeof(struct DataFile), 1, fp );
 	
	fseek( fp, df1.pageOfFile*df1.sizePerPage, SEEK_SET);	//����һ��2����������ļ� 
	ch = '\0';
	fwrite(&ch, sizeof(char), 1, fp);	
 	fclose(fp);
 	printf("Create file over.");
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
 	fp=fopen("datafile1.dat", "wb");
 	fseek( fp, 8192, SEEK_SET);
 	//fread(&s1,8192,1,fp);
	fwrite(&s1, sizeof(struct student), 1, fp); //������д���ļ� 
	fseek( fp, 1024*1024, SEEK_SET);	//����һ��2����������ļ� 
	char ch = '\0';
	fwrite(&ch, sizeof(char), 1, fp);	
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
 	fp=fopen("datafile1.dat", "rb");
	fread(&s2,8192,1,fp);//���ļ����ݶ��뵽����
	fclose(fp);
	printf("%s", s2.id);
	printf("%s", s2.name);
 };
 