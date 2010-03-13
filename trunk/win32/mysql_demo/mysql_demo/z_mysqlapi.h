#ifndef Z_MYSQLAPI
#define	Z_MYSQLAPI
#include "public.h"
#include <winsock.h>
#include <mysql.h>
#include <stdio.h>
#ifdef WIN32
#pragma comment(lib, "libmysql.lib")
#endif

status z_connect_mysql(MYSQL* mysql, const char *host, const char *user, const char *passwd, const char *db);
void z_close_mysql(MYSQL *mysql);
#endif	//Z_MYSQLAPI