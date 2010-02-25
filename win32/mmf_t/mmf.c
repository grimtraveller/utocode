#include "mmf.h"
//Create Memory File Map
//TCHAR* szFile [in] File Name
//DWORD dwCreationDisposition [in] Creation Flag
//usually, OPEN_ALWAYS.more see MSDN CreateFile() Function
//__int64 fileSize [in] File Size
//MMF mmf [out] a struct of Memory Map File 
MMFStatus MMFCreate(const TCHAR* szFile, DWORD dwCreationDisposition, __int64 fileSize,MMF* mmf)
{
	LONG high = 0;
	LONG low = 0;
	TCHAR szName[MAX_PATH] = {0};
	TCHAR szExt[MAX_PATH] = {0};
	//init MMF
	mmf->fileSize = 0;
	mmf->hFile = INVALID_HANDLE_VALUE;
	mmf->hMMF = NULL;
	mmf->p = NULL;
	memset(mmf->szFile, 0, MAX_PATH);

	//Create File
	strncpy(mmf->szFile, szFile, MAX_PATH);
	mmf->hFile = CreateFile(mmf->szFile, 
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,
		dwCreationDisposition, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE == mmf->hFile)
	{
		return GetLastError();
	}
	//Set File Size
	mmf->fileSize = fileSize;
#ifndef INVALID_SET_FILE_POINTER
#define	INVALID_SET_FILE_POINTER  (DWORD)0xFFFFFFFF
#endif

	high = (LONG)(mmf->fileSize >> 32);
	low = (LONG)(mmf->fileSize & 0x00000000FFFFFFFF);
	if (INVALID_SET_FILE_POINTER ==	SetFilePointer(mmf->hFile,  low, &high, FILE_BEGIN))
	{
		CloseHandle(mmf->hFile);
		mmf->hFile = INVALID_HANDLE_VALUE;
		return GetLastError();
	}
	if (!SetEndOfFile(mmf->hFile))
	{
		CloseHandle(mmf->hFile);
		mmf->hFile = INVALID_HANDLE_VALUE;
		return GetLastError();
	}
	//Maping File
	_splitpath(szFile, NULL, NULL, szName, szExt);
	strcat(szName, szExt);
	
	mmf->hMMF = CreateFileMapping(mmf->hFile, NULL, PAGE_READWRITE, high, low, szName);
	if (NULL == mmf->hMMF)
	{
		CloseHandle(mmf->hFile);
		mmf->hFile = INVALID_HANDLE_VALUE;	
		return GetLastError();
	}
	
	//Get Point
	mmf->p = MapViewOfFile(mmf->hMMF, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (NULL == mmf->p)
	{
		CloseHandle(mmf->hFile);
		mmf->hFile = INVALID_HANDLE_VALUE;	
		CloseHandle(mmf->hMMF);
		mmf->hMMF = NULL;
		return GetLastError();
	}
	
 	return 0;
}

//Writes to the disk a byte range within a mapped view of a file.
MMFStatus MMFFlush(LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush)
{
	if (FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush))
	{
		return 0;
	}
	else
	{
		return GetLastError();
	}
}

//Close Memory Map File
void MMFClose(MMF* mmf, BOOL bDelFile)
{
	if (NULL != mmf->p)
	{
		UnmapViewOfFile(mmf->p);
		mmf->p = NULL;
	}
	
	if (NULL != mmf->hMMF)
	{
		CloseHandle(mmf->hMMF);
		mmf->hMMF = NULL;
	}

	if (INVALID_HANDLE_VALUE != mmf->hFile)
	{
		CloseHandle(mmf->hFile);
		mmf->hFile = NULL;
	}

	if (bDelFile)
	{
		DeleteFile(mmf->szFile);
	}
	mmf->fileSize = 0;
	memset(mmf->szFile, 0, MAX_PATH);
}