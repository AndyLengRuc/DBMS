#include "stdio.h"
#include "string.h" 
long getConfig(char *name);
long Find(char *name, char *data);
int Search(char *name, char *data);
long GetNum(char* cnum);
int main()
{
/*	char name[50];
	scanf("%s",name);
	long n = match(name);
	printf("%ld",n); */
	return 0;
}

long getConfig(char *name){  //ͨ�������ַ�����ȥ�����ļ����Ҷ�Ӧ����ֵ 
	char data[1024], *buf ,*p;
	long num = -1;
	FILE *fp;
	fp = fopen("config.txt","r"); //���ĵ���λ�� D:\Project\RDBMS\DBMS\mingw2.95 
	if(!fp){
		printf("can't open config file!");
		return -1;
	}
	while(1){
		p = fgets(data,sizeof(data),fp);		
		if(!p){//ȫ����ѯ���û���ҵ� 
			break;
		}
	//	printf("%s",data);  //������� 
		if(Search(name,data)){ //�Ƿ��ڸ��д��ڣ������� ���������Ѱ�ң��������� 
			num = Find(name,data); 
			break;
		}
	}
	fclose(fp);
	return num;
}
long Find(char *name, char *data){ //���ؾ����Ӧ����������ֵ 
	long sum = 1;
//	printf("%s\n",name);
//	printf("%s   length = %d\n",data,strlen(name));
	char *p = data+strlen(name)+1;//p ��Խ'='��ָ����ֵ 
//	printf("%s\n",p);
	char *q;//qָ��*�� ����ĩβ 
//	printf("len:%d\n",q-p);
	while(1){        // 
		char cnum[5] = {'\0'};
		q = strchr(p,'*');
		if(!q)
			q = p+strlen(p)-1;
		strncpy(cnum,p,q-p);
		sum = sum * GetNum(cnum);
		if(*q == '\n') //�ַ������� 
			break;
		p = q + 1;
		q = strchr(p,'*');
		
		}
	return sum;
} 
int Search(char *name, char *data){ //�����Ƿ��ڸ��������� 
	if(strstr(data,name) != NULL && *(data + strlen(name)) == '=')
		return 1;
	else
		return 0;
} 
long GetNum(char* cnum){//�����ַ���ת�������� 
	long sum = 0;
	while(*cnum != '\0'){
		sum = sum * 10 + *cnum - '0';
		cnum = cnum + 1;
	}
	return sum;
}

 