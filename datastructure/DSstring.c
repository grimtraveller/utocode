/**
  @file		DSstring.c	
  @brief	implement string functions
  @details	see more ./Makefile
  @author	zuohaitao
  @date		2010-03-28
  @warning	many functions are NOT implement
  @bug		NO test
  */

#include "public.h"
#include "DSstring.h"
#include <string.h>
void SetStringN(char *s, const char c, int n)
{
	memset(s, c, n);
	*(s+n-1) = '\0';
}

char* CpyStringN(char* des, const char* src, int n)
{
	char* p = strncpy(des, src, n-1);
	*(des+n-1) = '\0';
	return p;
}

BOOLEAN IsStringEmpty(const char* s)
{
	return (GetStringLen(s) == 0 ? TRUE : FALSE);
}

int CmpStringN(const char*s, const char* t, int n)
{
	return strncmp(s, t, n);

}
int GetStringLen(const char* s)
{
	return strlen(s);
}

char* AppandString(char* des, char* src, int n)
{
	char* p;
	p = strncat(des, src, n-1);
	des+n-1 = '\0';
	return p;
}

char* GetSubString(char* sub, const char* str, int pos, int len)
{
	if (pos+len > strlen(str))
	{
		return NULL;
	}
	return CpyStringN(sub, str+pos, len);
}

char* FindString(const char* s, const char* t)
{
	return strstr(s, t);
}

void ReplaceString(char* s, const char* strOld, const char* strNew)
{
	int nNew = strlen(strNew);
	int mvlen = strlen(strNew) - strlen(strOld);
	char* pbegin = strstr(s, strOld);
	memmove(pbegin+mvlen, pbegin, strlen(pbegin));
	memcpy(pbegin, strNew, nNew);
}

void InsertString(char* s, int pos, const char* t)
{
	int slen, tlen;
	slen = strlen(s);
	tlen = strlen(t);
	mvlen += (slen > pos ? slen : pos);
	memmove(s+mvlen+tlen, s+mvlen, slen-pos);
	memcpy(s+mvlen, t, tlen);
}




