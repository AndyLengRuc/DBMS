
#define SIZE_DATA_SPACE (192*1024*1024)
#define SIZE_PER_PAGE (4*1024)
#define SIZE_BIT_MAP  (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8))

#define TABLE_DESC_ADDR 0
#define BITMAP_ADDR 1024 
#define DATA_ADDR	(BITMAP_ADDR+SIZE_BIT_MAP)
#define INODE_ADDR (BITMAP_ADDR+SIZE_BIT_MAP+SIZE_DATA_SPACE)

#define PAGE_PER_INODE 28
#define MAX_FILE_NUM 64
#define FIRST_FID	1

#define SIZE_BUFF 32
#define BUFF_NOT_HIT -1
#define P_AVAI		1
#define P_UNAVAI	0
#define P_EDIT		1
#define P_UNEDIT	0
#define ALLO_FAIL	-1

struct iNode 
{
	long fid;					//	��Node�����ĸ��ļ�
	long count;					//	��Node�м�¼�˶��ٸ�ҳ
	long nextAddr;				//	��һ��Node��ָ��
	long preAddr;				//	ǰһ��Node��ָ��
	long pageNo[PAGE_PER_INODE];	//	��¼ÿһ��ҳ��ҳ��
};
struct FileDesc
{
	long fileID;				//	�ļ��ı�ʶ
	long fileAddr;				//	�ļ���Node��ֵַ
	long filePageNum;			//	�ļ�ռ�ö���ҳ
};
struct SysDesc
{
	long sizeOfFile;			//	�洢�ļ��У��������Ĵ�С��Ĭ����Ϊ196M
	long sizePerPage;           //	ÿһ��ҳ�Ĵ�С
	long totalPage;				//	�ܹ���ҳ��
	long pageAvai;				//	��ǰ�ж��ٿ��õ�ҳ

	long bitMapAddr;			//	bitMap�ڴ洢�ļ��еĵ�ַ
	long sizeBitMap;			//	bitMap�Ĵ�С,���ֽ�Ϊ��λ

	long dataAddr;				//	�洢�ļ����������Ĵ�С
	long iNodeNum;				//	Node������
	long iNodeAddr;				//	Node�ڴ洢�ļ��ĵ�ַ
	long iNodeCur;				//	Ŀǰʹ�ù���Node����

	long curfid;				//	Ŀǰ���Է����fid��
	long curFileNum;			//	Ŀǰ�ж��ٸ��ļ������Ϊ MAX_FILE_NUM
	struct FileDesc fileDesc[MAX_FILE_NUM];			//	��ÿһ���ļ���������
};
struct buffMap
{
    long pageNo;					//	�û��������д洢�������ļ���ҳ��
	long vstTime;					//	���ʸû��������ʱ��
	int edit;						//	�û��������е������Ƿ��޸�
};
struct buffSpace
{
	 /*
		Ӧ�ö�̬���仺�������ݵĿռ�
		��Ϊ������̬�����ܷ��䵽�Ŀռ�̫С
	*/
	char data[SIZE_BUFF][SIZE_PER_PAGE];	//	���������ݿ飬Ŀǰ����SIZE_BUFF�飬ÿһ��Ĵ�СΪҳ�Ĵ�С	
	struct buffMap map[SIZE_BUFF];		//	��¼ÿһ�������������Ϣ
	long curTimeStamp;					//	Ŀǰ�����ʱ���
};