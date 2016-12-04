%{
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lqp.h"
#include "dict.h"
#define print_runtime(t1, t2) printf("This process used %.3lf seconds.\n", (double)((t2) - (t1)) / CLOCKS_PER_SEC)

extern int yylex();
extern int yyerror();

//����ĸ��ֵ��ú����б���
 
/* info used for create table */

static char *tbname;        
static int attrcnt;
static struct table_attr attr[MAXATTR_TB];

/* info used for create index and drop index */

static char *attrname;

/* info used for select statement */

static int selcnt;
static struct relattr selattrs[MAXATTR_SEL];
static int tbcnt;
static char *rel[MAXTB_FROM];
static int condcnt;
static int lorr;
static struct condition cond[MAXCOND_SEL];
static char *op;

/* used for order */

static int orderby;
static struct relattr orderattr;

/* info used for delete and insert */

static int valcnt;
static struct value val[MAXATTR_TB];

/* is first line */

int is_ares;
int is_uni;
int is_where;
int is_order;
int is_fin;
int stddbmsstart = 0;
int sys_exit = 0;

/* used for compute process time */

static clock_t before, after;
%}

//
%union {
	int intval;
	char *strval;
}

%start sql_list

//������������� 

%token <strval> NAME
%token <strval> STRING
%token <intval> NUMBER
%token <strval> COMPARISION

//���������ȼ� 

%left AND
%left COMPARISION /* < <= > >= <> = */
%left '+' '-'
%left '*' '/'

//�ؼ���
 
%token SELECT FROM WHERE ORDER BY ASC DESC
%token ALL UNIQUE DISTINCT
%token CREATE TABLE DROP INDEX
%token INSERT INTO VALUES DELETE
%token CHARACTER INTEGER DATE
%token SHOW TABLES
%token EXIT

%%

	/* start place */
sql_list:
		sql
		{
			stddbmsstart = is_fin = 1;
			return 0;
		}
	|	EXIT
		{
			sys_exit = 1;
			return 0;
		}
	;
sql:
		';'
	|	table_def      //����table����   
	|	table_drop     //ɾ��table���� 
	|	index_create   //������������ 
	|	index_drop     //ɾ���������� 
	|	select_stat    //select��� 
	|	insert_stat    //insert��� 
	|	delete_stat    //delete��� 
	|	table_show     //��ʾ���д����ı� 
	;

	/* show all tables */
table_show:
		SHOW TABLES ';'
		{
			before = clock();
			table_show();
			after = clock();
			print_runtime(before, after);
		}
	;
	/* create table */
table_def:    //������ 
		CREATE TABLE table '(' table_attr_list ')' ';'
		{
			before = clock();
			create_table_lqp(tbname, attrcnt, attr);
			after = clock();
			print_runtime(before, after);
		}
	;

table:        //��Ķ��� 
		NAME
		{
			tbname = $1;
			rel[tbcnt] = $1;
			is_where = 1;
		}
	;
table_attr_list:   //����ж���
		column_def
	|	table_attr_list ',' column_def
	;
column_def:
		column data_type     //����+�������� 
		{
			attrcnt++;   //�е����� 
		}
	;
column:
		NAME            //�����Ķ��� 
		{
			attrname = $1;
			attr[attrcnt].name = $1;
			selattrs[selcnt].tbname[0] = '\0';
			strcpy(selattrs[selcnt].attrname, $1);
			if (is_where == 1) {
				if (lorr == 0) {
					cond[condcnt].lhs_attr.tbname[0] = '\0';
					strcpy(cond[condcnt].lhs_attr.attrname, $1);
					lorr = 1;
				} else {
					cond[condcnt].rhs_attr.tbname[0] = '\0';
					strcpy(cond[condcnt].rhs_attr.attrname, $1);
				}
			}
			if (is_order == 1) {
				orderattr.tbname[0] = '\0';
				strcpy(orderattr.attrname, $1);
			}
		}
	|	NAME '.' NAME         //ǰһ��NAME�Ǳ�������һ��NAME��һ������ 
		{
			strcpy(selattrs[selcnt].tbname, $1);   //1�Ǳ�����3������ 
			strcpy(selattrs[selcnt].attrname, $3);
			if (is_where == 1) {
				if (lorr == 0) {
					strcpy(cond[condcnt].lhs_attr.tbname, $1);
					strcpy(cond[condcnt].lhs_attr.attrname, $3);
					lorr = 1;
				} else {
					strcpy(cond[condcnt].rhs_attr.tbname, $1);
					strcpy(cond[condcnt].rhs_attr.attrname, $3);
				}
			}
			if (is_order == 1) {
				strcpy(orderattr.tbname, $1);
				strcpy(orderattr.attrname, $3);
			}
		}
	;
data_type:  //�������͵Ķ��� 
		CHARACTER '(' NUMBER ')'   //number���� 
		{
			attr[attrcnt].type = CHAR;
			attr[attrcnt].len = $3;
		}
	|	INTEGER                    //���� 
		{
			attr[attrcnt].type = INT;
			attr[attrcnt].len = sizeof(int);
		}
	|	DATE                     //ʱ������ 
		{
			attr[attrcnt].type = CHAR;
			attr[attrcnt].len = 10;
		}
	;
	/* drop table */
table_drop:         //ɾ���� 
		DROP TABLE table ';'
		{
			before = clock();
			drop_table_lqp(tbname);
			after = clock();
			print_runtime(before, after);
		}
	;
	/* create index */
index_create:
		CREATE INDEX table '(' column ')' ';'
		{
			index_create_lqp(tbname, attrname);
		}
	;
	/* drop index */
index_drop:
		DROP INDEX table '(' column ')' ';'
		{
			index_drop_lqp(tbname, attrname);
		}
	;
	/* select statements */
select_stat:
		select_clause FROM fromlist where_clause order_clause ';'
		{
			before = clock();
			select_lqp(selcnt, selattrs, tbcnt, rel, condcnt, cond, orderby, orderattr, is_uni, is_ares);
			after = clock();
			print_runtime(before, after);
		}
	;
select_clause:   //ѡ���� 
		SELECT unique sellist
	|	SELECT unique '*'       
		{
			is_ares = 1;
		}
	;
unique:
		/* empty */
		{
			is_uni = 0;
		}
	|	ALL
		{
			is_uni = 0;
		}
	|	DISTINCT     //ȥ�ز��� 
		{
			is_uni = 1;
		}
	|	UNIQUE
		{
			is_uni = 1;
		}
	;
order_clause:  //���� 
		/* empty */
	|	ORDER BY column
		{
			orderby = 1;
		}
	|	ORDER BY column ASC
		{
			orderby = 1;
		}
	|	ORDER BY column DESC
		{
			orderby = 2;
		}
	;
fromlist:    //from֮��tables 
		table   //������ 
		{
			is_where = 1;
			tbcnt++;
		}
	|	fromlist ',' table   //����� 
		{
			is_where = 1;
			tbcnt++;
		}
	;
sellist:  //select ������ 
		column    //��������ʾ 
		{
			selcnt++;
		}
	|	sellist ',' column  //�������ʾ 
		{
			selcnt++;
		}
	;
where_clause:      //������� 
		/* empty */   //��������ѯ 
		{
			is_order = 1;
		}
	|	WHERE condition    //��������ѯ 
		{
			is_order = 1;
		}
	;
condition:  //�������� 
		expr
		{
			if (strcmp(op, "=") == 0) cond[condcnt].op = EQ;
			else if (strcmp(op, ">=") == 0) cond[condcnt].op = GE;
			else if (strcmp(op, "<=") == 0) cond[condcnt].op = LE;
			else if (strcmp(op, ">") == 0) cond[condcnt].op = GT;
			else if (strcmp(op, "<") == 0) cond[condcnt].op = LT;
			else if (strcmp(op, "<>") == 0) cond[condcnt].op = NE;
			condcnt++;
		}
	|	condition AND expr   //�߼��� 
		{
			if (strcmp(op, "=") == 0) cond[condcnt].op = EQ;
			else if (strcmp(op, ">=") == 0) cond[condcnt].op = GE;
			else if (strcmp(op, "<=") == 0) cond[condcnt].op = LE;
			else if (strcmp(op, ">") == 0) cond[condcnt].op = GT;
			else if (strcmp(op, "<") == 0) cond[condcnt].op = LT;
			else if (strcmp(op, "<>") == 0) cond[condcnt].op = NE;
			condcnt++;
		}
	;
expr:   //�������ʽ 
		column COMPARISION column  //��֮��Ƚ� 
		{
			cond[condcnt].is_rhs_attr = 1;
			op = $2;      //��������2 
			lorr = 0;
		}
	|	column COMPARISION NUMBER  //�к����ֱȽ� 
		{
			cond[condcnt].is_rhs_attr = 0;
			op = $2;
			lorr = 0;
			cond[condcnt].rhs_value.type = INT;
			memcpy(cond[condcnt].rhs_value.data, &$3, 4);
		}
	|	column COMPARISION STRING  //�к��ַ����Ƚ� 
		{
			cond[condcnt].is_rhs_attr = 0;
			op = $2;
			lorr = 0;
			cond[condcnt].rhs_value.type = CHAR;
			strcpy(cond[condcnt].rhs_value.data, $3);
		}
	;
	/* insert statements */

insert_stat:     //insert ��� 
		
		INSERT INTO table VALUES '(' insert_list ')' ';'
		{
			before = clock();
			insert_lqp(tbname, valcnt, val);
			after = clock();
			print_runtime(before, after);
		}
	;
insert_list: //����ֵ�б� 
		NUMBER  //�������� 
		{
			val[valcnt].type = INT;
			memcpy(val[valcnt].data, &$1, 4);
			valcnt++;
		}
	|	STRING  //�����ַ��� 
		{
			val[valcnt].type = CHAR;
			strcpy(val[valcnt].data, $1);
			valcnt++;
		}
	|	insert_list ',' NUMBER  //���������� 
		{
			val[valcnt].type = INT;
			memcpy(val[valcnt].data, &$3, 4);
			valcnt++;
		}
	|	insert_list ',' STRING  //������string 
		{
			val[valcnt].type = CHAR;
			strcpy(val[valcnt].data, $3);
			valcnt++;
		}
	;
	/* delete statement */
	
delete_stat:  // delete ���
 
		DELETE FROM table where_clause ';'
		{
			before = clock();
			delete_lqp(tbname, condcnt, cond);
			after = clock();
			print_runtime(before, after);
		}
	;
%%
int init_param() //��ʼ������  
{
	attrcnt = selcnt = tbcnt = 0;
	condcnt = valcnt = 0;
	lorr = 0;
	is_ares = 0;
	is_fin = 0;
	is_where = 0;
	is_order = 0;
	is_uni = 0;
	orderby = 0;
	return 0;
}

