#define SIZE_PER_PAGE 8192
#define Max_Table_Size 100
#define Max_File_Number 100
#define Max_Page_Per_File 1024
#include "io.h"
//#define Page_Per_Segment 128 

#define TRUE 1
#define FALSE 0

#define OK 1
#define ERROR 0 

struct FileMeta
{
	int file_deleted;
	long file_id;
	char fileName[64];  //ÿ���ļ������Ϊ64λ 
};

struct SysFile
{
/*
����ϵͳ�ļ�
ϵͳ�ļ��д����һЩϵͳ��Ϣ��������˸��������ļ��ĵ�ַ�� 
*/  
	long sizePerPage;           //ÿһ��ҳ�Ĵ�С
	long buffNum;				//���������� 
	long fileNumber;			//�������ݿ�������ļ����� 
    struct FileMeta files[Max_File_Number]; 	//�������ݿ������������������ļ� 
};

struct DataFileHead
{
/*
���������ļ� 
*/ 
	long fileId;				//�����ļ�ID 
	long pageOfFile;			//�ļ��ܹ���������ҳ 
  
  //���пռ���� 
	long freeCount;             //��ǰ���õ�ҳ�� 
	char isPageFree[Max_Page_Per_File];	//���ÿһ��page�Ƿ�Ϊ�� 
};


struct EMP
{
	int eno;
	char ename[20];
	float weight;
};
 
struct Page
{
   int pageID;	//page ID 
   long free_size;
   long recordNumber;         //Page�����ж��ټ�¼
   struct EMP emp[247];
};

