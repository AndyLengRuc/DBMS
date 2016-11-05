#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#define T 3			//b+���Ķ���
#define KeyType int
#define Pointer int
//�ڵ�ṹ��
typedef struct BPNode
{
	unsigned int id;//��¼����ڵ����ļ����еı��
	unsigned int n;	//��¼����ڵ��ж��ٸ��ؼ���
	int leaf;		//�ж��Ƿ�Ϊҳ�ڵ�
	KeyType key[2*T];//�ؼ��֣�����Ӧÿ�����ӽڵ���йؼ�����С�Ĺؼ��֣�
	Pointer child[2*T];//ָ�룬��¼ÿ���������ļ��ĵڼ���λ��
	Pointer next;//ָ�룬����¼��һ���ֵ�
}BPNode,*P_BPNode;

//���Ľṹ��
typedef struct BPTree
{
	P_BPNode root;
	unsigned int locate;//��¼���ڵ�����ļ��еı�ţ���id
	unsigned int num;	//��¼���ж��ٸ��ڵ�
	char name[100];		//���ڴ洢B+���Ľڵ��ļ�������
	FILE *fp;			//��д��name�ļ�ʱ��ʹ��
	int start;		//��С���������ڵ�Ҷ�ڵ�
}BPTree,*P_BPTree;

BPTree indexBPTree;		//ȫ�ֱ�����b+��


int writeNode(P_BPNode w)
{
	fseek(indexBPTree.fp, sizeof(BPNode)*(w->id - 1) + 2*sizeof(int), SEEK_SET);
	fwrite(w, sizeof(BPNode),1,indexBPTree.fp);

	return 0;
}

int readNode(P_BPNode r, Pointer id)
{
	fseek(indexBPTree.fp, (sizeof(BPNode))*(id - 1) + 2*sizeof(int), SEEK_SET);
	fread(r, (sizeof(BPNode)),1,indexBPTree.fp);
	return 0;
}
int pNode(P_BPNode n);

int createIndexBPTree (char *tableName, char *attr)
{//����B+��,��������Ӧ�ĳ�ʼ����B+���Ľṹ����һ��ȫ�ֱ�����
	P_BPNode root;
	indexBPTree.root = (P_BPNode)malloc(sizeof(BPNode));
	indexBPTree.num = 1;
	indexBPTree.start = 1;
	memcpy(indexBPTree.name, "../data/", sizeof("../data/"));
	strcat(indexBPTree.name, tableName);
	strcat(indexBPTree.name, ".");
	strcat(indexBPTree.name, attr);
	puts(indexBPTree.name);
	root = indexBPTree.root; 
	root->n = 0;
	root->leaf = 1;
	root->next = -1;
	root->id = 1;
	indexBPTree.locate = 1;

	indexBPTree.fp = fopen(indexBPTree.name, "wb");
	fwrite(&indexBPTree.num, sizeof(int),1,indexBPTree.fp);
	fwrite(&indexBPTree.locate, sizeof(int),1,indexBPTree.fp);
	writeNode(root);
	
	
	fclose(indexBPTree.fp);
	free(root);
	indexBPTree.root = NULL;
	return 0;
}


int splitBPNode (P_BPNode p, P_BPNode c, int i)
{//�ڵ�ķ��ѣ�Ҫ��p�ڵ����ٻ��ܲ���һ���ڵ㣬c�ڵ������ģ���nΪ2*T;
	int j;
	P_BPNode b;
	b = (P_BPNode)malloc(sizeof(BPNode));
	b->leaf = c->leaf;
	b->n = T;
	b->id = indexBPTree.num+1;	//Ϊb��ֵid�ţ����ڱ�ʾ�ýڵ㣬��ͬʱid�ž�������ڵ����ļ���λ��
	b->next = c->next;			//Ϊb��next��ֵ����ԭ����c�ڵ��next
	//��c�ڵ�ĺ�벿�ֹؼ��ָ��Ƹ�b
	for (j = 0; j < T; j++)
	{
		b->key[j] = c->key[j+T];
		b->child[j] = c->child[j+T];
	}

	//����b�ڵ�Ķ�ӦԪ���Ѿ��������ˣ�������Ҫд���ļ�

	indexBPTree.num++;
	c->n = T;	//c�ڵ�Ĺؼ�����Ŀ����
	c->next = b->id;



	//��p�ڵ��i֮��Ľڵ����
	for (j = p->n - 1; j > i; j--)
	{
		p->key[j+1] = p->key[j];
		p->child[j+1] = p->child[j];
	}
	
	//��b�ڵ����p��
	p->key[i+1] = b->key[0];
	p->child[i+1] = b->id;

	p->n++;	//p�ؼ��ָ�����һ
	//д��p
	writeNode(p);
	writeNode(c);
	writeNode(b);
	free(b);
	return 0;
}//splitBPNode


int insertBPNodeNotFull(P_BPNode s, KeyType k, unsigned int id)
{//���룬Ҫ��s�ڵ㲻������

	int i = s->n-1;

	if (s->leaf)
	{//Ҷ�ڵ㣬�ҵĺ��ʵ�λ��
		while (i >= 0 && s->key[i] > k)
		{
			s->key[i+1] = s->key[i];
			s->child[i+1] = s->child[i];
			i--;
		}

		s->key[i+1] = k;
		s->child[i+1] = id;
		s->n++;
		writeNode(s);
	}
	else
	{
		P_BPNode tmp = (P_BPNode)malloc(sizeof(BPNode));
		while (i >= 0 && s->key[i] > k)
		{
			i--;
		}
		if (i < 0)
		{//�����Ԫ����С��������Ԫ�ز����һ���ڵ㣬���޸Ķ�Ӧ��key
			i++;
			s->key[i] = k;
		}
		writeNode(s);
		readNode(tmp, s->child[i]);		//��ȡ��Ӧ��
		if (tmp->n == 2*T)
		{
			splitBPNode(s, tmp, i);
			if (k > s->key[i+1])
				i++;
			readNode(tmp, s->child[i]);		//���¶�ȡ�����д��Ż�	
		}
		insertBPNodeNotFull(tmp, k, id);
		free(tmp);
	}
	
	return 0;
}

Pointer equalSearch(P_BPTree tree, KeyType k)
{//��ֵ��ѯ������keyֵ�����Ҷ�Ӧ��id�������ء���������ڸýڵ㣬����һ������
	int i;
	int result;
	P_BPNode r;
	r = tree->root;
	if (k < r->key[0])	//����С�Ľڵ�С
		return -1;
	P_BPNode tmp = (P_BPNode)malloc(sizeof(BPNode));
	while (1)
	{
		i = r->n - 1;
		while (i >= 0 && r->key[i] > k)
			i--;

		if (r->leaf)	//��Ҷ�ӣ�����
			break;
		readNode(tmp, r->child[i]);
		r = tmp;
	}//while

	if (r->key[i] < k)
		return -1;
	result = r->child[i];

	free(tmp);
	tmp = NULL;
	return result;
}//equalSearch

int rangeSearch (P_BPTree tree, KeyType low, KeyType high)
{//��Χ���ң�keyֵ���ڵ���low��С�ڵ���high�����ط�Χ�ڵĸ�����
	unsigned int i;
	P_BPNode r = tree->root;
	Pointer *result;
	P_BPNode tmp = (P_BPNode)malloc(sizeof(BPNode));

	if (high < low)	//low <= high�������н��
		return 0;
	if (high < r->key[0])
		return 0;

	if (low < r->key[0])
		low = r->key[0];
	
	while (1)
	{
		i = r->n - 1;
		while (i >= 0 && r->key[i] > low)
			i--;
		if (r->leaf)	//��Ҷ�ӣ�����
			break;
		readNode(tmp, r->child[i]);
		r = tmp;
	}//while
	
	if (r->key[i] < low)
		i++;
	unsigned int num=100;
	result = (Pointer *)malloc(sizeof(Pointer)*num);
	unsigned int j = 0;

	while (1)
	{
		for (; i < r->n && r->key[i] <= high; i++)
		{
			if (j >= num)
			{
				num += 100;
				realloc(result, sizeof(Pointer)*num);
			}
			result[j++] = r->child[i];
		//	printf("sid:%d  iid: %d     id:%d\n", r->key[i],r->id, r->child[i]);
		}
		if (i <r->n || r->next < 0)
			break;
		
		readNode(tmp, r->next);
		r = tmp;
		i = 0;
	}//while 
	free(tmp);
	tmp = NULL;
	return j;
}//rangeSearch

int insertKeyInBPTree (P_BPTree tree, KeyType k, Pointer id)
{//�����в���ڵ�
	P_BPNode r = tree->root;

	if (equalSearch(tree, k) > 0)
	{
		printf("Ԫ���Ѵ��ڣ�");
		return -1;
	}

	if (tree->root->n == 2*T)
	{//���ڵ����ˣ����·�����ڵ㣬�����г�ʼ��
		P_BPNode s = (P_BPNode)malloc(sizeof(BPNode));
		s->leaf = 0;
		s->n = 1;
		s->key[0] = r->key[0];
		s->child[0] = r->id;
		s->id = tree->num + 1;
	
		s->next = -1;
		//���µĸ�д�����
		writeNode(s);
	
		tree->num++;
	
		writeNode(r);

		splitBPNode (s, r, 0);

		//����Ϊs�����Խ��¸�copy��tree->rootָ����ָ����ڴ档��tree->root��һֱָ��һƬ�����˵��ڴ棬��ʱ�̱��������������ڵ㣩
		memcpy(tree->root, s, sizeof(BPNode));
		tree->locate = s->id;

		insertBPNodeNotFull(s, k, id);
		free(s);	//�ͷ��ڴ�		
	}
	else
		insertBPNodeNotFull(r, k, id);
	return 0;
}//insertBPNode

int initIndexBPTree(char *tableName, char *attr)
{//��ʼ��BPTree������Ӧ�ļ���fp��¼��Ϊroot�����ڴ���Դ洢һ���ڵ���ڴ棬��������ڵ�
	indexBPTree.root = (P_BPNode)malloc(sizeof(BPNode));
	indexBPTree.start = 1;

	memcpy(indexBPTree.name, "../data/", sizeof("../data/"));
	strcat(indexBPTree.name, tableName);
	strcat(indexBPTree.name, ".");
	strcat(indexBPTree.name, attr);
	indexBPTree.fp = fopen(indexBPTree.name, "rb+");
	fread(&indexBPTree.num,sizeof(int),1, indexBPTree.fp);
	fread(&indexBPTree.locate,sizeof(int),1,indexBPTree.fp);

	readNode(indexBPTree.root, indexBPTree.locate);
	return 0;
}

int endBPTree()
{//��������������
	fseek(indexBPTree.fp, 0, SEEK_SET);
	fwrite(&indexBPTree.num, sizeof(int),1,indexBPTree.fp);
	fwrite(&indexBPTree.locate, sizeof(int),1,indexBPTree.fp);
	free(indexBPTree.root);
	fclose(indexBPTree.fp);
	return 0;
}

int pNode(P_BPNode n)
{//����ڵ�
	printf("%s  id:%d  next:%d  ������%d\n" ,n->leaf?"��Ҷ�ڵ�":"����Ҷ�ڵ�", n->id, n->next, n->n);
	unsigned int i;
	for(i = 0; i < n->n; i++)
		printf("  key[%d]:%d\t",i,n->key[i]);
	puts("");
	for(i = 0; i < n->n; i++)
		printf("child[%d]:%d\t",i,n->child[i]);
	puts("");
	return 0;
}//pNode

int replaceKeyInBPTree(P_BPTree tree, KeyType oldkey, KeyType newkey)
{//��oldkey�滻Ϊnewkey
	P_BPNode r = tree->root;
	int i;
	P_BPNode tmp = (P_BPNode)malloc(sizeof(BPNode));

	while (1)
	{
		i = r->n - 1;
		while (i >= 0 && r->key[i] > oldkey)
			i--;
		if (r->key[i] == oldkey)
		{
			r->key[i] = newkey;
			writeNode(r);
		}

		if (r->leaf)
			break;
		readNode(tmp, r->child[i]);
		r = tmp;
	}
	free(tmp);
	return 0;
}

int adjustToDel(P_BPNode p, P_BPNode x, unsigned int i)
{//pָ��x�ĸ��ڵ㣬iָ���ǣ�x��p���±�
	unsigned int j;
	P_BPNode left = NULL;
	P_BPNode right = NULL;
	P_BPNode tmp = (P_BPNode)malloc(sizeof(BPNode));

	if	(i > 0 )	//x�����ֵ�
	{
		readNode(tmp, p->child[i-1]);
		left = tmp;
		if (left->n > T)
		{
			for (j = x->n; j > 0; j--)
			{
				x->key[j] = x->key[j-1];
				x->child[j] = x->child[j-1];
			}
			x->n++;
			x->key[0] = left->key[left->n-1];
			x->child[0] = left->child[left->n-1];
			writeNode(x);

			left->n--;
			writeNode(left);
			
			p->key[i] = x->key[0];
			writeNode(p);

			return 0;
		}
	}//if

	if (i < p->n - 1) //x�����ֵ�
	{
		readNode(tmp, p->child[i+1]);
		right = tmp;
		left = NULL;
		if (right->n > T)
		{
			x->key[x->n] = right->key[0];
			x->child[x->n] = right->child[0];
			x->n++;
			writeNode(x);

			for (j = 0; j < right->n-1; j++)
			{
				right->key[j] = right->key[j+1];
				right->child[j] = right->child[j+1];
			}
			right->n--;
			writeNode(right);

			p->key[i+1] = right->key[0];
			writeNode(p);
			return 0;
		}
	}

	if (left == tmp)
	{
		for (j = 0; j < T; j++)
		{
			left->key[T+j] = x->key[j];
			left->child[T+j] = x->child[j];
		}
		left->n += T;
		left->next = x->next;
		writeNode(left);

		for (j = i; j < p->n - 1; j++)
		{
			p->key[j] = p->key[j+1];
			p->child[j] = p->child[j+1];
		}
		p->n--;
		writeNode(p);
		memcpy(x, left, sizeof(BPNode));
	}
	else
	{
		for (j = 0; j < T; j++)
		{
			x->key[T+j] = right->key[j];
			x->child[T+j] = right->child[j];
		}
		x->n += T;
		x->next = right->next;
		writeNode(x);

		for (j = i+1; j < p->n -1; j++)
		{
			p->key[j] = p->key[j+1];
			p->child[j] = p->child[j+1];
		}
		p->n--;
		writeNode(p);
	}
	free(tmp);
	left = right = tmp = NULL;
	return 0;
}
//������������ǣ������ڵ�p������������Ӧ��Ҫ��
//�����p�Ǹ��ڵ�����Ҷ�ӽڵ㣬��û��Ҫ��
//�����p�Ǹ��ڵ㣨��Ҷ������p�ڵ���ӽڵ������С��2��B+��������������Ҫ�󣩡�
//�����p�ǷǸ��ڵ㣬��ڵ�p���ӽڵ�����������T
KeyType delKeyInBPNode(P_BPNode p, KeyType k)
{//������ڵ�Ϊ��㣬�ҵ�k��ɾ����Ҫ��ȷ��k����
	unsigned int i;
	unsigned int j;
	i = p->n - 1;

	while (p->key[i] > k)
		i--;

	//��Ҷ�ڵ�(���p�������Ǹ��ڵ㣬������������)
	if (p->leaf)
	{
		for (j = i; j < p->n-1; j++)
		{
			p->key[j] = p->key[j+1];
			p->child[j] = p->child[j+1];
		}//while
		p->n--;
		writeNode(p);
		if (i == 0)	//˵��ɾ���Ĺؼ����Ǹýڵ�����С��
			replaceKeyInBPTree(&indexBPTree, k, p->key[i]);
		return p->key[i];
	}//if
	

	//p���ڽڵ�
	P_BPNode x;

	x = (P_BPNode)malloc(sizeof(BPNode));
	
	readNode(x, p->child[i]);
	if (x->n > T)	//x���ӽڵ�ĸ�������T����ֱ�ӵ���
		return delKeyInBPNode(x, k);
	else			//x���ӽڵ�ĸ�������T����Ҫ����
	{
		adjustToDel(p, x, i);
		return delKeyInBPNode(x, k);
	}//else

}//delKeyInNode

int delKeyInBPTree(P_BPTree tree, KeyType k)
{//1.���һ�����ڵ�ͬʱ����Ҷ�ڵ㣬��û���ӽڵ����ƣ�����ӽڵ�ָ��Ĳ��������Ľڵ㣩
 //2.��Ҷ���ڵ����ٱ����������ӽڵ㣬�����Ľڵ�������T���ӽڵ㡣 
 
	if (equalSearch(tree, k) < 0)	//����Ƿ���k����ؼ���
		return -1;

	P_BPNode r = tree->root;

	delKeyInBPNode(r, k);
	if (r->n == 1)
	{
		tree->locate = r->child[0];
		readNode(tree->root,r->child[0]);	//�������ڵ�
		tree->num--;
		//��Ӧ�ý�ԭʼ�ĸ��ڵ�Ӵ�����ɾ��
	}
	r = NULL;
	return 0;
}//delKeyInBPTree


int main ()
{
	unsigned int i = 1;
	createIndexBPTree("student","idx");
	initIndexBPTree("student", "idx");

	insertKeyInBPTree(&indexBPTree, 50, i++);
	insertKeyInBPTree(&indexBPTree, 30, i++);
	insertKeyInBPTree(&indexBPTree, 60, i++);
	insertKeyInBPTree(&indexBPTree, 10, i++);
	insertKeyInBPTree(&indexBPTree, 90, i++);
	insertKeyInBPTree(&indexBPTree, 40, i++);
	insertKeyInBPTree(&indexBPTree, 100, i++);



	insertKeyInBPTree(&indexBPTree, 110, i++);
	insertKeyInBPTree(&indexBPTree, 150, i++);
	insertKeyInBPTree(&indexBPTree, 200, i++);
	insertKeyInBPTree(&indexBPTree, 0, i++);
	insertKeyInBPTree(&indexBPTree, 49, i++);
	insertKeyInBPTree(&indexBPTree, 45, i++);
	insertKeyInBPTree(&indexBPTree, -1, i++);
	insertKeyInBPTree(&indexBPTree, 210, i++);
	insertKeyInBPTree(&indexBPTree, 220, i++);
	insertKeyInBPTree(&indexBPTree, 230, i++);
	insertKeyInBPTree(&indexBPTree, 240, i++);
	insertKeyInBPTree(&indexBPTree, 250, i++);
	insertKeyInBPTree(&indexBPTree, 260, i++);
	insertKeyInBPTree(&indexBPTree, 270, i++);
	insertKeyInBPTree(&indexBPTree, 280, i++);
	insertKeyInBPTree(&indexBPTree, 290, i++);


	insertKeyInBPTree(&indexBPTree, 300, i++);
	insertKeyInBPTree(&indexBPTree, 310, i++);
	insertKeyInBPTree(&indexBPTree, 320, i++);
	insertKeyInBPTree(&indexBPTree, 330, i++);
	insertKeyInBPTree(&indexBPTree, 340, i++);
	insertKeyInBPTree(&indexBPTree, 350, i++);
	insertKeyInBPTree(&indexBPTree, 360, i++);
	insertKeyInBPTree(&indexBPTree, -360, i++);

	
	KeyType k;
	k = 340;

	int s = equalSearch(&indexBPTree, k);
	if (s > 0)
		printf("id:%d\n",s);
	else
		printf("������Ԫ��:%d\n",k); 
	

	rangeSearch(&indexBPTree, 500, 500);

	insertKeyInBPTree(&indexBPTree, 50, i++);
	insertKeyInBPTree(&indexBPTree, 55, i++);
	insertKeyInBPTree(&indexBPTree, 60, i++);
	insertKeyInBPTree(&indexBPTree, 100, i++);
	insertKeyInBPTree(&indexBPTree, 110, i++);
	insertKeyInBPTree(&indexBPTree, 115, i++);
	insertKeyInBPTree(&indexBPTree, 200, i++);

	insertKeyInBPTree(&indexBPTree, 210, i++);
	insertKeyInBPTree(&indexBPTree, 215, i++);
	insertKeyInBPTree(&indexBPTree, 220, i++);

	
	P_BPNode t = (P_BPNode)malloc(sizeof(BPNode));

	printf("������%d  ��id:%d\n", indexBPTree.num, indexBPTree.locate);
	pNode(indexBPTree.root);
	
	
	delKeyInBPTree(&indexBPTree,100);
	printf("%d\n\n",indexBPTree.num);
	
	pNode(indexBPTree.root);
	
	for (i = 1; i < indexBPTree.num; i++)
	{
		readNode(t,i); 
		pNode(t);
	}
	endBPTree();
	return 0;
}

