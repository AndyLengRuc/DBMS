#define SIZE_PER_PAGE 8192
#define Max_Table_Size 100
#define Max_File_Number 1000 
//#define SIZE_BUFF 32
//#define Page_Per_Segment 128 


struct FileHead
{
	int file_deleted;
	long file_id;
	char fileName[64];
};

struct SysFile
{
/*
����ϵͳ�ļ�
ϵͳ�ļ��д����һЩϵͳ��Ϣ��������˸��������ļ��ĵ�ַ�� 
*/  
	long sizePerPage;           //ÿһ��ҳ�Ĵ�С
	long fileNumber;			//�������ݿ�������ļ����� 
    struct FileHead files[Max_File_Number]; 	//�������ݿ������������������ļ� 
};

struct DataFile  
{
/*
���������ļ� 
*/ 
	long fileId;				//�����ļ�ID 
	long pageOfFile;			//�ļ��Ĵ�С����������ҳ 
  
  //���пռ���� 
	long freeCount;             //��ǰ���õ�ҳ�� 
	long bitMapAddr;			//bitMap�ڴ洢�ļ��еĵ�ַ
	long sizeBitMap;			//bitMap�Ĵ�С,���ֽ�Ϊ��λ
	long *data;          //ָ��ǰ�ĵ�ַ
    char *pageDataMemoryHead;   //��������ͷָ��
};
/*
struct mapTable                 //�߼���ַ�������ַ��ӳ��� 
{
	long *logic;                 //�߼���ַ 
	long *physical;              //�����ַ 
}maTable[Max_Table_Size];       
*/
/*struct segment                //�εĶ��� 
{
	long segmentNo;           //�κ� 
	long pageAvaiOfSegment;		//��ǰ���ж��ٿ��õ�ҳ
}; */
struct Page
{
   int pageID;	//page ID 
   int fileID;  //file ID
   long sizeOfhead;           // ͷhead �Ĵ�С  
   long free_size;
   long recordNumber;         //Page�����ж��ټ�¼
};
