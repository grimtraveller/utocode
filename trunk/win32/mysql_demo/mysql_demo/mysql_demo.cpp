// mysql_demo.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#pragma warning(disable:4996)

#include <winsock.h>
#include <mysql.h>
#include <stdio.h>
#pragma comment(lib, "libmysql.lib")
/*返回值*/
typedef int status;
//成功
#define Z_OK				0
//内存申请失败
#define Z_ERR_MALLOC		-1


//定义最大中文字数
#define MAX_WORD_COUNT		3
//
//
status exec_sql(MYSQL* conn, char* cmd);
MYSQL* conn_db(MYSQL** conn, const char* serv, const char* usr, const char* passwd, const char* db);
void close_db(MYSQL* conn);
MYSQL_RES* use_db_res(MYSQL* conn);
MYSQL_RES* use_db_store(MYSQL* conn);
unsigned long get_rows_length(MYSQL* res);
MYSQL_ROW get_row(MYSQL_RES* res);
void free_db_res(MYSQL_RES* res);
//void operate_db_t();
//
//
void res2mems(char** cnwords, MYSQL_RES* res);
char* unit_cnword(char* cnword1, char* cnword2);
int split_pyname(char* pyname,const char* split_c, char** pyword);
//
//
void usr_input_py(char* py_name);
//
//

//TODO:栈上申请的数组过大，可适当缩小变为合适的大小
//TODO:关于char sql[1000]数组，动态申请更为合适
int _tmain(int argc, _TCHAR* argv[])
{
	//database connect
	MYSQL* conn = NULL;
	//info table res
	MYSQL_RES *res = NULL;
	//info table row 
	MYSQL_ROW row = {0};
	/*
	//TODO: 数据库连接参数通过命令好传入
	if (argc < 5)
	{
		fprintf(stdin, "USAGE: %s [server] [user] [password] [database_name]\n"
						"e.g. %s localhost root 123456 wg_db\n",
						argv[0], argv[0]);
	}
	char serv[20] = {0};
	char usr[20] = {0};
	char passwd[20] = {0};
	char db[20] = {0};
	*/
	/* connect database*/


	//char *serv = "localhost";
	//char* serv = "192.168.0.2";
	char* serv = "192.168.0.1";
	char *usr = "root";
	char *passwd = "password"; //此处改成你的密码
	char *db = "wg_db";

	int cn_char_count = 0;

	char sql[1000] = {0};

	int i;

	char* token = NULL;
	char* split_c = ",";

	//pinyin words 
	char pyname[100] = {0};
	//pinyin word
	char* pyword[MAX_WORD_COUNT] = {NULL};
	//the chinese words 
	char* cnwords[MAX_WORD_COUNT] = {NULL};
	//all names split by ','
	char* allunits = NULL;
	/* connect database */
	conn = conn_db(&conn, serv, usr, passwd, db);
	if (conn == NULL)
	{
		fprintf(stderr, "连接数据库失败\n");
		exit(1);
	}
	/* set character = gb2312 */
	if (0 != exec_sql(conn, "set names gb2312"))
	{
		goto LABEL_FAIL;
	}
	//DEBUG test the operation of database
	//operate_db_t();
	
	usr_input_py(pyname);
	cn_char_count = split_pyname(pyname," ", pyword);

	for (i = 0; i < cn_char_count; i++)
	{
		memset(sql, 0, 1000);
		strcat(sql, "SELECT cn_char FROM dict WHERE py_char=\"");
		strcat(sql, pyword[i]);
		strcat(sql, "\"");
		//printf("%s\n", sql);
		if (Z_OK != exec_sql(conn, sql))
		{
			fprintf(stdin, "查询数据库数据出错(SQL:%s)", sql);
			continue;
		}
		res = use_db_res(conn);
		res2mems(&cnwords[i], res);
		free_db_res(res);
		free(pyword[i]);
	}

	/* unit all cnword */
	allunits = cnwords[0];
	for (i = 1; i < cn_char_count; i++)
	{
		allunits = unit_cnword(allunits, cnwords[i]);
		//printf("%s\n", allunits);
	}
	for (i = 0; i < cn_char_count; i++)
	{
		free(cnwords[i]);
		cnwords[i] = NULL;
	}

	/*arrive database to printf all infos*/
	memset(sql, 0, 1000);
	strcat(sql, "SELECT * FROM info WHERE ");
	token = strtok(allunits, split_c);
	while( NULL != token)
	{
		strcat(sql, "name=\"");
		strcat(sql, token);
		strcat(sql, "\" OR ");
		token = strtok(NULL, split_c);
	}
	free(allunits);
	*(sql + strlen(sql) - strlen(" OR ")) = '\0';
	//DEBUG:printf("%s\n", sql);
	if (Z_OK != exec_sql(conn, sql))
	{
		goto LABEL_FAIL;
	}
	res = use_db_res(conn);
	while(1)
	{
		row = get_row(res);
		if (NULL == row)
		{
			break;
		}
		printf("%s\t%s\n", row[1], row[2]);
	}
	free_db_res(res);
	printf("按任意键退出程序...\n");
	getchar();
//LABEL_SUCCESS:
	close_db(conn);
	return 0;
LABEL_FAIL:
	close_db(conn);
	return -1;
}

//convert res from mysql struct to memory (a string split with ',')
//[out]char** cnwords	- string memory
//[in]MYSQL_RES* res	- mysql result struct
void 
res2mems(char** cnwords, MYSQL_RES* res)
{
	MYSQL_ROW row;
	int row_len;
	int len = 0;
	while(1)
	{
		row = get_row(res);
		if (NULL == row)
		{
			break;
		}
		row_len = strlen(row[0]);
		
		*cnwords = (char*)realloc(*cnwords, len + row_len + 1);		
		memcpy(*cnwords + len, row[0], row_len);
		len = len + row_len;
		*(*cnwords + len) = ',';
		len++;
		
	}
	*cnwords = (char*)realloc(*cnwords, len + 1);
	*(*cnwords + len) = '\0';
	return;
}

//unit stirng1 and string2
//char* cnword1		- a string split with ','
//char* cnword2		- a string split with ','
//a point a new string plit with ','
//Need free()
char*
unit_cnword(char* cnword1, char* cnword2)
{
	char* cnword1_tmp;
	char* cnword2_tmp;
	char* allcnwords = NULL;
	int alllen = 0;
	char* token1 = NULL;
	char *next_token1 = NULL;
	char* token2 = NULL;
	char *next_token2 = NULL;
	char split_c[] = ",";
	cnword1_tmp = (char*)malloc(strlen(cnword1)+1);
	if (NULL == cnword1_tmp)
	{
		fprintf(stderr, "申请内存失败 %d", errno);
		exit(Z_ERR_MALLOC);
	}
	strcpy(cnword1_tmp, cnword1);
	*(cnword1 + strlen(cnword1)) = 0;
	cnword2_tmp = (char*)malloc(strlen(cnword2)+1);
	if (NULL == cnword2_tmp)
	{
		fprintf(stderr, "申请内存失败 %d", errno);
		exit(Z_ERR_MALLOC);
	}
	strcpy(cnword2_tmp, cnword2);
	*(cnword2 + strlen(cnword2)) = 0;
	
	token1 = strtok_s(cnword1_tmp, split_c, &next_token1);
	
	while( (NULL != token1) || (NULL != token2) )
	{
		//DEBUG:printf("%s\n", token1);
		strcpy(cnword2_tmp, cnword2);
		*(cnword2 + strlen(cnword2)) = 0;
		next_token2 = NULL;
		token2 = strtok_s(cnword2_tmp, split_c, &next_token2);
		while( NULL != token2)
		{
			//DEBUG:printf("\t%s\n", token2);

			allcnwords = (char*)realloc(allcnwords, alllen+strlen(token1)+strlen(token2)+1);						
			if (NULL == allcnwords)
			{
				fprintf(stderr, "申请内存失败 %d", errno);
				exit(Z_ERR_MALLOC);
			}

			strcpy(allcnwords+alllen, token1);
			alllen += strlen(token1);
			strcpy(allcnwords+alllen, token2);
			alllen += strlen(token2);
			*(allcnwords + alllen) = ',';
		//	
			token2 = strtok_s(NULL, split_c, &next_token2);
			alllen++;
		}

		token1 = strtok_s(NULL, split_c, &next_token1);
	}
	allcnwords = (char*)realloc(allcnwords, alllen+1);
	*(allcnwords+alllen) = '\0';	 
	free(cnword1_tmp);
	free(cnword2_tmp);
	return allcnwords;
}

/*

/* MYSQL API */
MYSQL*
conn_db(MYSQL** conn, const char* serv, const char* usr, const char* passwd, const char* db)
{
	*conn = mysql_init(NULL);
	if (NULL == mysql_real_connect(*conn, serv, usr, passwd, db, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(*conn));
		return NULL;
	}
	return *conn;
}

void
close_db(MYSQL* conn)
{
	mysql_close(conn);
	return;
}

status
exec_sql(MYSQL* conn, char* cmd)
{
	status ret;
	ret = mysql_query(conn, cmd);
	if (0 != ret)
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
	return ret;
}

MYSQL_RES*
use_db_res(MYSQL* conn)
{
	return mysql_use_result(conn);
}

MYSQL_RES*
use_db_store(MYSQL* conn)
{
	return mysql_store_result(conn);
}

MYSQL_ROW
get_row(MYSQL_RES* res)
{
	return mysql_fetch_row(res);
}

void
free_db_res(MYSQL_RES* res)
{
	mysql_free_result(res);
}


/*User Interface*/
void
usr_input_py(char* py_name)
{
	while(1)
	{
		fprintf(stdout, "请输入所要操作的用户名:");
		fgets(py_name, 99, stdin);
		if (strlen(py_name) > 99)
		{
			fprintf(stderr, "用户名太长(用户名不大于100个字节)请重新输入\n");
		}
		else
		{
			break;
		}
	}
	*(py_name + strlen(py_name) - 1) = 0;
}

/*String API*/
int
split_pyname(char* pyname,const char* split_c, char** pyword)
{
	char* token = NULL;
	int len;
	int i = 0;
	token = strtok( pyname, split_c);
	while( NULL != token)
	{
		if (i >=3)
		{
			fprintf(stdin, "字数不能超过三个\n");
		}
		len = strlen(token);
		pyword[i] = (char*)malloc(len+1);
		if (NULL == pyword[i])
		{
			return Z_ERR_MALLOC;
		}
		sprintf(pyword[i], "%s", token);
		token = strtok(NULL, split_c);
		i++;
	}

	return i;
}

/* test function */
void
operate_db_t()
{
	MYSQL* conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *serv = "localhost";
	char *usr = "root";
	char *passwd = "password"; /* 此处改成你的密码 */
	char *db = "wg_db";
	/* connect the wg_db database*/
	conn = conn_db(&conn, serv, usr, passwd, db);
	if (conn == NULL)
	{
		exit(1);
	}
	/* set character = gb2312 */
	if (0 != exec_sql(conn, "set names gb2312"))
	{
		exit(1);
	}

	/* query dict table */
	if (0 != exec_sql(conn, "select cn_char from dict where py_char=\"zuo\""))
	{
		exit(1);
	}
	res = use_db_res(conn);
	while(1)
	{
		row = mysql_fetch_row(res);
		if (NULL == row)
		{
			break;
		}
		printf("%s\t%s\n", row[0], row[1]);
	}
	free_db_res(res);
	close_db(conn);
	return;
}
