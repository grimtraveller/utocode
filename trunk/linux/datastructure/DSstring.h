#ifndef DSSTRING_H
#define DSSTRING_H
#include "public.h"
//like memset
extern void SetStringN(char* T, const char c, int n);
//like strcpy
extern char* CpyStringN(char* des, const char* src, int n);
//no use
//void DestroyString(char* s);
//
extern BOOLEAN IsStringEmpty(const char* s);
//like strcmp
int CmpStringN(const char* s, const char* t, int n);
//like strlen
extern int GetStringLen(const char* s);
//like strcat
extern char* AppendStringN(char* des, const char* src, int n);
//
extern char* GetSubString(char* sub, const char* str, int pos, int len);
//like strstr
extern char* FindString(const char* s, const char* t);
//
extern void ReplaceString(char* s, const char* strOld, const char* strNew);
//
void InsertString(char* s, int pos, const char* t);
//
void DeleteString(char* s, int pos, int len);
//
void ClearString(char* s);
#endif	//DSSTRING_H
