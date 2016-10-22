
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dbHead.h"



/**
 * @brief ��ѯһ��ҳ���Ƿ��ڻ�������
 *
 * @param [in] head  : struct dbSysHead *
 * @param [in] query : long     Ҫ��ѯ��ҳ��
 * @return  int 
 * @retval  �����ҳ�ڻ������У��򷵻����ڻ������е��±�
 *
 * @author linzhixia
 * @date 2013/11/6 
 **/
int queryPage( struct dbSysHead *head, long query )
{
	int i;

	for( i=0; i<SIZE_BUFF; i++ ) {
		if( (head->buff).map[i].pageNo == query ) {
			return i;
		}
	}
	return BUFF_NOT_HIT;
}



/**
 * @brief �滻�������е�ҳ
 *
 * @param [in\out] head  : struct dbSysHead *
 * @param [in] mapNo : int     Ҫ������ҳ�ڻ������е��±�
 * @param [in] pageNo : long    Ҫ�����ҳ��
 * @return  int 
 *
 * @author linzhixia
 * @date 2013/11/6 
 **/
int replacePage( struct dbSysHead *head, int mapNo, long pageNo )
{
	if( P_EDIT == (head->buff).map[mapNo].edit ) {
		rewind(head->fpdesc);
		fseek( head->fpdesc, head->desc.dataAddr + (head->buff).map[mapNo].pageNo*SIZE_PER_PAGE, SEEK_SET );
		fwrite( head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc );
	}
	rewind(head->fpdesc);
	fseek( head->fpdesc, head->desc.dataAddr + pageNo*SIZE_PER_PAGE, SEEK_SET  );
	fread( head->buff.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc );
	head->buff.map[mapNo].edit = P_UNEDIT;
	head->buff.map[mapNo].pageNo = pageNo;
	return 0;
}



/**
 * @brief �����㷨������ʹ�õ����滻�����û��ʹ�õ�ҳ
 *
 * @param [in] head  : struct dbSysHead *
 * @return  int 
 *
 * @author linzhixia
 * @date 2013/11/6 
 **/
int scheBuff( struct dbSysHead *head )
{
	int i;
	int min;

	min = 0;
	for( i=0; i<SIZE_BUFF; i++ ){
		if( head->buff.map[i].pageNo < 0 ) {
			return i;
		}
		else {
			if( head->buff.map[i].vstTime < head->buff.map[min].vstTime ) {
					min = i;
			}
		}
	}
	return min;
}



/**
 * @brief �����дһ��ҳ������ҳ���ڻ������У�������滻�㷨�Ѹ�ҳ������������
 *
 * @param [in] head  : struct dbSysHead *
 * @param [in] query : long
 * @return  int 
 * @retval  ���ظ�ҳ�ڻ������е��±�
 *
 * @author linzhixia
 * @date 2013/11/6 
 **/
int reqPage( struct dbSysHead *head, long query )
{
	int i;
	int mapNo;

	mapNo = queryPage( head, query );
	if( BUFF_NOT_HIT == mapNo ) {
		mapNo = scheBuff( head );
		replacePage( head, mapNo, query );
	}
	head->buff.curTimeStamp++;
	head->buff.map[mapNo].vstTime = head->buff.curTimeStamp;
	if( (1<<30) == head->buff.curTimeStamp ) {
		for( i=0; i<SIZE_BUFF; i++ ) {
			head->buff.map[i].vstTime = 0;
			head->buff.curTimeStamp = 0;
		}
	}
	return mapNo;
}