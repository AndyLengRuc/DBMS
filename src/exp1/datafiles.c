#include "stdio.h"

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
 
 int createDataFile()
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
 	fp=fopen("datafile1.dat", "wb");
	fseek( fp, 8192*2, SEEK_SET );		//����һ��2����������ļ� 
	ch = '\0';
	fwrite(ch, sizeof(char), 1, fp );	
 	fclose(fp);
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
	fwrite(&s1, sizeof(struct student), 1, fp); //������д���ļ� 
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
	fread(&s2,sizeof(struct student),1,fp);//���ļ����ݶ��뵽����
	fclose(fp);
	printf("%s", s2.id);
	printf("%s", s2.name);
 };