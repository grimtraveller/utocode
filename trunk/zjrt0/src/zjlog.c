#include "zjlog.h"
#include "zjtype.h"
static ZJLogSt gLog = {0};

int zjInitLog(char* file, int level)
{
	gLog.fp = fopen(file, "wb");
	if (NULL == gLog.fp)
	{
		return ZJERROR;
	}
	strcpy(gLog.file, file);
	gLog.level = level;
	memset(gLog.buffer, 0, ZJ_LOGBUFF_LEN);
	return ZJOK;
}

void zjSetLogLevel(int lv)
{
	gLog.level = (ZJLogLvE)lv;
}

int zjPrintLog(int level, char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(gLog.buffer, format, args);
	va_end(args);
#ifdef WIN32
	OutputDebugString(gLog.buffer);
#endif //WIN32
	if(NULL == gLog.fp) 
	{
		gLog.fp = fopen(gLog.file, "wb");
	}
	if (NULL != gLog.fp)
	{
		fwrite(gLog.buffer, strlen(gLog.buffer), 1, gLog.fp);
		fflush(gLog.fp);
	}
	else
	{
		return ZJERROR;
	}
	return ZJOK;
}

void zjCloseLogFile()
{
	fclose(gLog.fp);
}
