#include <stdio.h>
#include <bPlusTreeIndex.h>

BPKeyTree indexBPKeyTree;		//ȫ�ֱ�����b+��

int saveNodeToFile(P_BPKeyNode kNode)
{
	fseek(indexBPKeyTree.pof, sizeof(BPKeyNode)*(kNode->nodeId - 1) + 2*sizeof(int), SEEK_SET);
	fwrite(kNode, sizeof(BPKeyNode),1,indexBPKeyTree.pof);

	return 0;
}

int getNodeFromFile(P_BPKeyNode kNode, int nodeId)
{
	fseek(indexBPKeyTree.pof, (sizeof(BPKeyNode))*(nodeId - 1) + 2*sizeof(int), SEEK_SET);
	fread(kNode, (sizeof(BPKeyNode)),1,indexBPKeyTree.pof);
	return 0;
}

int displayNode(P_BPKeyNode kNode)
{//��ʾ�ڵ�
	printf("%s  id:%d  next:%d  ������%d\n" ,kNode->isLeaf?"��Ҷ�ڵ�":"����Ҷ�ڵ�", kNode->nodeId, kNode->nextBrother, kNode->keyNumInNode);
	int i;
	for(i = 0; i < kNode->keyNumInNode; i++)
		printf("  key[%d]:%d\t",i,kNode->keyInNode[i]);
	puts("");
	for(i = 0; i < kNode->keyNumInNode; i++)
		printf("child[%d]:%d\t",i,kNode->children[i]);
	puts("");
	return 0;
}

int createIndexBPKeyTree (char *tableName, char *attr)
{//����B+��,��������Ӧ�ĳ�ʼ����B+���Ľṹ����һ��ȫ�ֱ�����
	//Initial the root node
	P_BPKeyNode rootNode;


	indexBPKeyTree.treeRoot = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
	indexBPKeyTree.nodeNum = 1;
	indexBPKeyTree.keyNum = 0;
	indexBPKeyTree.fistLeaf = 1;
	//Start: the file path of the index tree
	//memcpy(indexBPKeyTree.treeFileName, "../data/", sizeof("../data/"));
	strcpy(indexBPKeyTree.treeFileName, "../data/");
	strcat(indexBPKeyTree.treeFileName, tableName);
	strcat(indexBPKeyTree.treeFileName, ".");
	strcat(indexBPKeyTree.treeFileName, attr);
	puts(indexBPKeyTree.treeFileName);
	//End: the file path of the index tree
	rootNode = indexBPKeyTree.treeRoot;
	rootNode->keyNumInNode = 0;
	rootNode->isLeaf = 1;
	rootNode->nextBrother = -1;
	rootNode->nodeId = 1;
	indexBPKeyTree.rootFileId = 1;

	indexBPKeyTree.pof = fopen(indexBPKeyTree.treeFileName, "wb");
	fwrite(&indexBPKeyTree.nodeNum, sizeof(int),1,indexBPKeyTree.pof);     //�ļ�ͷ��д������int,���ǵ�һ��
	fwrite(&indexBPKeyTree.rootFileId, sizeof(int),1,indexBPKeyTree.pof); //�ļ�ͷ��д������int,���ǵڶ���
	saveNodeToFile(rootNode);

	fclose(indexBPKeyTree.pof);
	free(rootNode);
	indexBPKeyTree.treeRoot = NULL;
	return 0;
}

int initIndexBPKeyTree(char *tableName, char *attr)
{//��ʼ��BPTree������Ӧ�ļ���fp��¼��Ϊroot�����ڴ���Դ洢һ���ڵ���ڴ棬��������ڵ�
	indexBPKeyTree.treeRoot = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
	indexBPKeyTree.fistLeaf = 1;

	//memcpy(indexBPKeyTree.treeFileName, "../data/", sizeof("../data/"));
	strcpy(indexBPKeyTree.treeFileName, "../data/");
	strcat(indexBPKeyTree.treeFileName, tableName);
	strcat(indexBPKeyTree.treeFileName, ".");
	strcat(indexBPKeyTree.treeFileName, attr);
	indexBPKeyTree.pof = fopen(indexBPKeyTree.treeFileName, "rb+");
	fread(&indexBPKeyTree.nodeNum,sizeof(int),1, indexBPKeyTree.pof);
	fread(&indexBPKeyTree.rootFileId,sizeof(int),1,indexBPKeyTree.pof);

	getNodeFromFile(indexBPKeyTree.treeRoot, indexBPKeyTree.rootFileId);
	return 0;
}


int divideBPKeyNode (P_BPKeyNode newkNode, P_BPKeyNode fullkNode, int i)
{//�ڵ�ķ��ѣ�Ҫ��p�ڵ����ٻ��ܲ���һ���ڵ㣬c�ڵ������ģ���nΪ2*T;
	int j;
	P_BPKeyNode b;
	b = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
	b->isLeaf = fullkNode->isLeaf;
	b->keyNumInNode = Degree;
	b->nodeId = indexBPKeyTree.nodeNum+1;	//Ϊb��ֵid�ţ����ڱ�ʾ�ýڵ㣬��ͬʱid�ž�������ڵ����ļ���λ��
	b->nextBrother = fullkNode->nextBrother;			//Ϊb��next��ֵ����ԭ����c�ڵ��next
	//��c�ڵ�ĺ�벿�ֹؼ��ָ��Ƹ�b
	for (j = 0; j < Degree; j++)
	{
		b->keyInNode[j] = fullkNode->keyInNode[j+Degree];
		b->children[j] = fullkNode->children[j+Degree];
	}

	//����b�ڵ�Ķ�ӦԪ���Ѿ��������ˣ�������Ҫд���ļ�

	indexBPKeyTree.nodeNum++;
	fullkNode->keyNumInNode = Degree;	//c�ڵ�Ĺؼ�����Ŀ����
	fullkNode->nextBrother = b->nodeId;

	//��p�ڵ��i֮��Ľڵ����
	for (j = newkNode->keyNumInNode - 1; j > i; j--)
	{
		newkNode->keyInNode[j+1] = newkNode->keyInNode[j];
		newkNode->children[j+1] = newkNode->children[j];
	}
	
	//��b�ڵ����p��
	newkNode->keyInNode[i+1] = b->keyInNode[0];
	newkNode->children[i+1] = b->nodeId;

	newkNode->keyNumInNode++;	//p�ؼ��ָ�����һ
	//д��p
	saveNodeToFile(newkNode);
	saveNodeToFile(fullkNode);
	saveNodeToFile(b);
	free(b);
	return 0;
}


int insertBPNodeNotFull(P_BPKeyNode kNode, KeyDataType key, int id)
{//���룬Ҫ��s�ڵ㲻������

	int i = kNode->keyNumInNode-1;

	if (kNode->isLeaf)
	{//Ҷ�ڵ㣬�ҵĺ��ʵ�λ��
		while (i >= 0 && kNode->keyInNode[i] > key)
		{
			kNode->keyInNode[i+1] = kNode->keyInNode[i];
			kNode->children[i+1] = kNode->children[i];
			i--;
		}

		//����keyֵ
		kNode->keyInNode[i+1] = key;
		kNode->children[i+1] = id;
		kNode->keyNumInNode++;
		saveNodeToFile(kNode);
	}
	else
	{
		P_BPKeyNode tmp = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
		while (i >= 0 && kNode->keyInNode[i] > key)
		{
			i--;
		}
		if (i < 0)
		{//�����Ԫ����С��������Ԫ�ز����һ���ڵ㣬���޸Ķ�Ӧ��key
			i++;
			kNode->keyInNode[i] = key;
		}
		saveNodeToFile(kNode);
		getNodeFromFile(tmp, kNode->children[i]);		//��ȡ��Ӧ���ӽ��
		if (tmp->keyNumInNode == 2*Degree)
		{
			divideBPKeyNode(kNode, tmp, i);
			if (key > kNode->keyInNode[i+1])
				i++;
			getNodeFromFile(tmp, kNode->children[i]);		//���¶�ȡ���д��Ż�
		}
		insertBPNodeNotFull(tmp, key, id);
		free(tmp);
	}

	return 0;
}

Pointer equalSearch(P_BPKeyTree tree, KeyDataType key)
{//��ֵ��ѯ������keyֵ�����Ҷ�Ӧ��id�������ء���������ڸýڵ㣬����һ������
	int i;
	int result;
	P_BPKeyNode r;
	r = tree->treeRoot;
	if (key < r->keyInNode[0])	//����С�Ľڵ�С
		return -1;
	P_BPKeyNode tmp = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
	while (1)
	{
		i = r->keyNumInNode - 1;
		while (i >= 0 && r->keyInNode[i] > key)
			i--;

		if (r->isLeaf)	//��Ҷ�ӣ�����
			break;
		getNodeFromFile(tmp, r->children[i]);
		r = tmp;
	}//while

	if (r->keyInNode[i] < key)
		return -1;
	result = r->children[i];

	free(tmp);
	tmp = NULL;
	return result;
}

int rangeSearch (P_BPKeyTree tree, KeyDataType lowValue, KeyDataType highValue)
{//��Χ���ң�keyֵ���ڵ���lowValue��С�ڵ���highValue�����ط�Χ�ڵĸ�����
	int i;
	P_BPKeyNode r = tree->treeRoot;
	Pointer *result;
	P_BPKeyNode tmp = (P_BPKeyNode)malloc(sizeof(BPKeyNode));

	if (highValue < lowValue)	//low <= high�������н��
		return 0;
	if (highValue < r->keyInNode[0])
		return 0;

	if (lowValue < r->keyInNode[0])
		lowValue = r->keyInNode[0];
	
	while (1)
	{
		i = r->keyNumInNode - 1;
		while (i >= 0 && r->keyInNode[i] > lowValue)
			i--;
		if (r->isLeaf)	//��Ҷ�ӣ�����
			break;
		getNodeFromFile(tmp, r->children[i]);
		r = tmp;
	}//while
	
	if (r->keyInNode[i] < lowValue)
		i++;
	int num=100;
	result = (Pointer *)malloc(sizeof(Pointer)*num);
	int j = 0;

	while (1)
	{
		for (; i < r->keyNumInNode && r->keyInNode[i] <= highValue; i++)
		{
			if (j >= num)
			{
				num += 100;
				realloc(result, sizeof(Pointer)*num);
			}
			result[j++] = r->children[i];
		//	printf("sid:%d  iid: %d     id:%d\n", r->key[i],r->id, r->child[i]);
		}
		if (i <r->keyNumInNode || r->nextBrother < 0)
			break;
		
		getNodeFromFile(tmp, r->nextBrother);
		r = tmp;
		i = 0;
	}//while 
	free(tmp);
	tmp = NULL;
	return j;
}//rangeSearch

int insertKeyInBPKeyTree (P_BPKeyTree tree, KeyDataType key, int id)
{//�����в���ڵ�
	P_BPKeyNode currentRoot = tree->treeRoot;

	if (equalSearch(tree, key) > 0)
	{
		printf("Ԫ���Ѵ��ڣ�");
		return -1;
	}

	if (tree->treeRoot->keyNumInNode == 2*Degree)
	{//���ڵ����ˣ����·�����ڵ㣬�����г�ʼ��
		P_BPKeyNode newRoot = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
		newRoot->isLeaf = 0;
		newRoot->keyNumInNode = 1;
		newRoot->keyInNode[0] = currentRoot->keyInNode[0];
		newRoot->children[0] = currentRoot->nodeId;
		newRoot->nodeId = tree->nodeNum + 1;
	
		newRoot->nextBrother = -1;
		//���µĸ�д�����
		saveNodeToFile(newRoot);
	
		tree->nodeNum++;
	
		saveNodeToFile(currentRoot);

		divideBPKeyNode (newRoot, currentRoot, 0);

		//����Ϊs�����Խ��¸�copy��tree->rootָ����ָ����ڴ档��tree->root��һֱָ��һƬ�����˵��ڴ棬��ʱ�̱��������������ڵ㣩
		memcpy(tree->treeRoot, newRoot, sizeof(BPKeyNode));
		tree->rootFileId = newRoot->nodeId;

		insertBPNodeNotFull(newRoot, key, id);
		free(newRoot);	//�ͷ��ڴ�
	}
	else
		insertBPNodeNotFull(currentRoot, key, id);
	indexBPKeyTree.keyNum++;
	return 0;
}//insertBPNode


int endBPKeyTree()
{//��������������
	fseek(indexBPKeyTree.pof, 0, SEEK_SET);
	fwrite(&indexBPKeyTree.nodeNum, sizeof(int),1,indexBPKeyTree.pof);
	fwrite(&indexBPKeyTree.rootFileId, sizeof(int),1,indexBPKeyTree.pof);
	free(indexBPKeyTree.treeRoot);
	fclose(indexBPKeyTree.pof);
	return 0;
}


int replaceKeyInBPKeyTree(P_BPKeyTree tree, KeyDataType oldkey, KeyDataType newkey)
{//��oldkey�滻Ϊnewkey
	P_BPKeyNode currentRoot = tree->treeRoot;
	int i;
	P_BPKeyNode tmp = (P_BPKeyNode)malloc(sizeof(BPKeyNode));

	while (1)
	{
		i = currentRoot->keyNumInNode - 1;
		while (i >= 0 && currentRoot->keyInNode[i] > oldkey)
			i--;
		if (currentRoot->keyInNode[i] == oldkey)
		{
			currentRoot->keyInNode[i] = newkey;
			saveNodeToFile(currentRoot);
		}

		if (currentRoot->isLeaf)
			break;
		getNodeFromFile(tmp, currentRoot->children[i]);
		currentRoot = tmp;
	}
	free(tmp);
	return 0;
}

int adjustToDel(P_BPKeyNode p, P_BPKeyNode x, int i)
{//pָ��x�ĸ��ڵ㣬iָ���ǣ�x��p���±�
	int j;
	P_BPKeyNode left = NULL;
	P_BPKeyNode right = NULL;
	P_BPKeyNode tmp = (P_BPKeyNode)malloc(sizeof(BPKeyNode));

	if	(i > 0 )	//x�����ֵ�
	{
		getNodeFromFile(tmp, p->children[i-1]);
		left = tmp;
		if (left->keyNumInNode > Degree)
		{
			for (j = x->keyNumInNode; j > 0; j--)
			{
				x->keyInNode[j] = x->keyInNode[j-1];
				x->children[j] = x->children[j-1];
			}
			x->keyNumInNode++;
			x->keyInNode[0] = left->keyInNode[left->keyNumInNode-1];
			x->children[0] = left->children[left->keyNumInNode-1];
			saveNodeToFile(x);

			left->keyNumInNode--;
			saveNodeToFile(left);
			
			p->keyInNode[i] = x->keyInNode[0];
			saveNodeToFile(p);

			return 0;
		}
	}//if

	if (i < p->keyNumInNode - 1) //x�����ֵ�
	{
		getNodeFromFile(tmp, p->children[i+1]);
		right = tmp;
		left = NULL;
		if (right->keyNumInNode > Degree)
		{
			x->keyInNode[x->keyNumInNode] = right->keyInNode[0];
			x->children[x->keyNumInNode] = right->children[0];
			x->keyNumInNode++;
			saveNodeToFile(x);

			for (j = 0; j < right->keyNumInNode-1; j++)
			{
				right->keyInNode[j] = right->keyInNode[j+1];
				right->children[j] = right->children[j+1];
			}
			right->keyNumInNode--;
			saveNodeToFile(right);

			p->keyInNode[i+1] = right->keyInNode[0];
			saveNodeToFile(p);
			return 0;
		}
	}

	if (left == tmp)
	{
		for (j = 0; j < Degree; j++)
		{
			left->keyInNode[Degree+j] = x->keyInNode[j];
			left->children[Degree+j] = x->children[j];
		}
		left->keyNumInNode += Degree;
		left->nextBrother = x->nextBrother;
		saveNodeToFile(left);

		for (j = i; j < p->keyNumInNode - 1; j++)
		{
			p->keyInNode[j] = p->keyInNode[j+1];
			p->children[j] = p->children[j+1];
		}
		p->keyNumInNode--;
		saveNodeToFile(p);
		memcpy(x, left, sizeof(BPKeyNode));
	}
	else
	{
		for (j = 0; j < Degree; j++)
		{
			x->keyInNode[Degree+j] = right->keyInNode[j];
			x->children[Degree+j] = right->children[j];
		}
		x->keyNumInNode += Degree;
		x->nextBrother = right->nextBrother;
		saveNodeToFile(x);

		for (j = i+1; j < p->keyNumInNode -1; j++)
		{
			p->keyInNode[j] = p->keyInNode[j+1];
			p->children[j] = p->children[j+1];
		}
		p->keyNumInNode--;
		saveNodeToFile(p);
	}
	free(tmp);
	left = right = tmp = NULL;
	return 0;
}
//������������ǣ������ڵ�p������������Ӧ��Ҫ��
//�����p�Ǹ��ڵ�����Ҷ�ӽڵ㣬��û��Ҫ��
//�����p�Ǹ��ڵ㣨��Ҷ������p�ڵ���ӽڵ������С��2��B+��������������Ҫ�󣩡�
//�����p�ǷǸ��ڵ㣬��ڵ�p���ӽڵ�����������T

KeyDataType delKeyInBPNode(P_BPKeyNode kNode, KeyDataType key)
{//������ڵ�Ϊ��㣬�ҵ�k��ɾ����Ҫ��ȷ��k����
	int i;
	int j;
	i = kNode->keyNumInNode - 1;

	while (kNode->keyInNode[i] > key)
		i--;

	//��Ҷ�ڵ�(���p�������Ǹ��ڵ㣬������������)
	if (kNode->isLeaf)
	{
		for (j = i; j < kNode->keyNumInNode-1; j++)
		{
			kNode->keyInNode[j] = kNode->keyInNode[j+1];
			kNode->children[j] = kNode->children[j+1];
		}//while
		kNode->keyNumInNode--;
		indexBPKeyTree.keyNum--;
		saveNodeToFile(kNode);
		if (i == 0)	//˵��ɾ���Ĺؼ����Ǹýڵ�����С��
			replaceKeyInBPKeyTree(&indexBPKeyTree, key, kNode->keyInNode[i]);
		return kNode->keyInNode[i];
	}//if
	

	//p���ڽڵ�
	P_BPKeyNode x;

	x = (P_BPKeyNode)malloc(sizeof(BPKeyNode));
	
	getNodeFromFile(x, kNode->children[i]);
	if (x->keyNumInNode > Degree)	//x���ӽڵ�ĸ�������T����ֱ�ӵ���
		return delKeyInBPNode(x, key);
	else			//x���ӽڵ�ĸ�������T����Ҫ����
	{
		adjustToDel(kNode, x, i);
		return delKeyInBPNode(x, key);
	}//else
	

}

int delKeyInBPTree(P_BPKeyTree tree, KeyDataType key)
{//1.���һ�����ڵ�ͬʱ����Ҷ�ڵ㣬��û���ӽڵ����ƣ�����ӽڵ�ָ��Ĳ��������Ľڵ㣩
 //2.��Ҷ���ڵ����ٱ����������ӽڵ㣬�����Ľڵ�������T���ӽڵ㡣 
 
	if (equalSearch(tree, key) < 0)	//����Ƿ���k����ؼ���
		return -1;

	P_BPKeyNode currentRoot = tree->treeRoot;

	delKeyInBPNode(currentRoot, key);
	if (currentRoot->keyNumInNode == 1)
	{
		tree->rootFileId = currentRoot->children[0];
		getNodeFromFile(tree->treeRoot,currentRoot->children[0]);	//�������ڵ�
		tree->nodeNum--;
		//��Ӧ�ý�ԭʼ�ĸ��ڵ�Ӵ�����ɾ��
	}
	currentRoot = NULL;
	
	return 0;
}


int main ()
{
	int i = 1;
	puts("����������");
	createIndexBPKeyTree("student","idx");
	puts("��ʹ��������");
	initIndexBPKeyTree("student", "idx");

	puts("����key������");
	insertKeyInBPKeyTree(&indexBPKeyTree, 50, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 30, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 60, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 10, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 90, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 40, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 100, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 110, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 150, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 200, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 0, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 49, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 45, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, -1, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 210, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 220, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 230, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 240, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 250, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 260, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 270, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 280, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 290, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 300, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 310, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 320, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 330, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 340, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 350, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, 360, i++);
	insertKeyInBPKeyTree(&indexBPKeyTree, -360, i++);

	KeyDataType key;
	key = 340;
	printf("\n###�������������ؼ��֣�%d ####\n", key);
	int s = equalSearch(&indexBPKeyTree, key);
	if (s > 0)
		printf("id:%d\n",s);
	else
		printf("������Ԫ��:%d\n",key);
	printf("###�������������ؼ��ֽ�����%d ####\n", key);

//	puts("\n############Range search########\n");
//	rangeSearch(&indexBPKeyTree, 300, 500);
//	puts("\n############Range search########\n");



	puts("\n############ ��ӡ��������Ϣ ########\n");
	P_BPKeyNode temp = (P_BPKeyNode)malloc(sizeof(P_BPKeyNode));

	printf("�ڵ������%d\t��id:%d\tkey����:%d\n", indexBPKeyTree.nodeNum, indexBPKeyTree.rootFileId, indexBPKeyTree.keyNum);
	displayNode(indexBPKeyTree.treeRoot);
	for (i = 1; i < indexBPKeyTree.nodeNum; i++)
	{
		getNodeFromFile(temp,i);
		displayNode(temp);
	}

	puts("\n############ ��ӡɾ�����100����� ########\n");
	delKeyInBPTree(&indexBPKeyTree,100);
	printf("�ڵ������%d\t��id:%d\tkey����:%d\n", indexBPKeyTree.nodeNum, indexBPKeyTree.rootFileId, indexBPKeyTree.keyNum);
	displayNode(indexBPKeyTree.treeRoot);

	for (i = 1; i < indexBPKeyTree.nodeNum; i++)
	{
		getNodeFromFile(temp,i);
		displayNode(temp);
	}

	puts("\n############ �������� ########\n");
	endBPKeyTree();
	return 0;
}

