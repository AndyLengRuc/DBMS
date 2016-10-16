#define SIZE_PER_PAGE 8192
#define Max_Table_Size 100
//#define SIZE_BUFF 32
//#define Page_Per_Segment 128 

struct DataFile  
{
	long pageOfFile;			//	�ļ��Ĵ�С����������ҳ 
  //long pageOfSegment;         //   ÿ���εĴ�С����������ҳ 
    long sizePerPage;           //	ÿһ��ҳ�Ĵ�С

  //long totalPage;             //�ܵ�ҳ��
	long pageAvail;             //��ǰ���õ�ҳ�� 
	long bitMapAddr;			//	bitMap�ڴ洢�ļ��еĵ�ַ
	long sizeBitMap;			//	bitMap�Ĵ�С,���ֽ�Ϊ��λ
	long *data;                 //ָ��ǰ�ĵ�ַ	
  
  //���пռ���� 
	unsigned int freeCount;  //����page��  
    long *pagefreeHead;      //���е�ͷ  
    long *pagefreeTail;      //���е�β  
    //long *pageusedHead;      //��ʹ�õ�ͷ  
    //long *pageusedTail;      //��ʹ�õ�β 
	char *pageHead;      //page��ͷָ��,�����ͷ��ڴ�ʱ���ã���Ϊpage�ķ�����һ�����ڴ�)  
    char *pageDataMemoryHead; //��������ͷָ��   
};

struct mapTable                 //�߼���ַ�������ַ��ӳ��� 
{
	long *logic;                 //�߼���ַ 
	long *physical;              //�����ַ 
}maTable[Max_Table_Size];         

/*struct segment                //�εĶ��� 
{
	long segmentNo;           //�κ� 
	long pageAvaiOfSegment;		//��ǰ���ж��ٿ��õ�ҳ
}; */
struct Page 
{  
   long *head;                // ָ��head��ָ�� 
   long sizeOfhead;           // ͷ head �Ĵ�С  
   long recordNumber;         //�����ж��ټ�¼
   long *addressOfLastRecord; //��� һ����¼�ĵ�ַ 
};
