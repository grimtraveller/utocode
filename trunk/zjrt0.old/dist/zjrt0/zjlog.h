#ifndef ZJLOG_H
#define ZJLOG_H
#include <stdio.h>
#include "zjtype.h"
#define ZJ_LOGBUFF_LEN	1024

typedef enum ZJLOGLEVELE 
{
	ZJLOG_DEBUG = 0,
	ZJLOG_INFO ,
	ZJLOG_WARN ,
	ZJLOG_ERR ,
	ZJLOG_FATAL,
	ZJLOG_NONE
}ZJLogLvE;
typedef struct ZJLOGST
{
	FILE* fp;
	char file[ZJ_MAX_PATH];
	ZJLogLvE level;
	char buffer[ZJ_LOGBUFF_LEN];
}ZJLogSt;
int zjInitLog(char* file, int lv);
void zjSetLogLevel(int lv);
int zjPrintLog(int lv, char* format, ...);
void zjCloseLogFile();
#endif //ZJLOG_H
