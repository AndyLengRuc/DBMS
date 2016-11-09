#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#define Degree 3			//b+���Ķ���  The half degree of the b plus tree
#define KeyDataType int
#define Pointer int

//�ڵ�ṹ�� The structure of the tree node
typedef struct BPKeyNode	//b plus key node
{
	int nodeId;//��¼����ڵ����ļ����еı��
	int keyNumInNode;	//��¼����ڵ��ж��ٸ��ؼ���
	int isLeaf;		//�ж��Ƿ�Ϊҳ�ڵ�
	KeyDataType keyInNode[2*Degree];//�ؼ��֣�����Ӧÿ�����ӽڵ���йؼ�����С�Ĺؼ��֣�
	Pointer children[2*Degree];//ָ�룬��¼ÿ���������ļ��ĵڼ���λ��
	Pointer nextBrother;//ָ�룬��¼��һ���ֵ�
}BPKeyNode,*P_BPKeyNode;

//���Ľṹ��
typedef struct BPKeyTree
{
	P_BPKeyNode treeRoot;
	int rootFileId;//��¼���ڵ�����ļ��еı�ţ���id
	int nodeNum;	//��¼���ж��ٸ��ڵ�
	long keyNum;	    //��¼���ж��ٸ�key 
	char treeFileName[100];		//���ڴ洢B+���Ľڵ��ļ�������
	FILE *pof;			//��д��name�ļ�ʱ��ʹ��  pointer of file
	int firstLeaf;		//��С���������ڵ�Ҷ�ڵ�
}BPKeyTree,*P_BPKeyTree;
