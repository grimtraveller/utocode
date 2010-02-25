#ifndef MMF_H
#define MMF_H
#include <windows.h>

typedef struct __MMF
{
	TCHAR szPath[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	__int64 fileSize;
	HANDLE hFile;
	HANDLE hMMF;
	LPVOID p;
} MMF;
typedef DWORD MMFStatus;
MMFStatus MMFCreate(const TCHAR* szFile, DWORD dwCreationDisposition, __int64 fileSize,MMF* mmf);
MMFStatus MMFFlush(LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush);
void MMFClose(MMF* mmf, BOOL bDelFile);
#endif	//MMF_H