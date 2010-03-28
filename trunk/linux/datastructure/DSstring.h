/**
  @file		DSstring.h
  @brief	declaration string functions
  @details	usage - see Makefile
  @author	zuohaitao
  @date		2010-03-28
  @warning	
  @bug		
  */

#ifndef DSSTRING_H
#define DSSTRING_H
#include "public.h"
/*
 * @name	SetStringN()
 * @brief	like memset
 *
 *			set n - 1 char in string value and *(s+n-1) = '\0' 
 * @param	char* s [I/ ] string
 * @param	const char c [I/ ] set value
 * @param	int n [I/ ] size
 * @return	void
 */
extern void SetStringN(char* s, const char c, int n);

/**
  @name			CpyStringN()
  @brief		like strncpy
				strncpy n - 1 chars and set *(des+n-1) = '\0' 
  @param		char* des [ /O] destination address
  @param		char* src [I/ ] source address
  @param		char* n   [I/ ] destination max size
  @return		char* destination point
  */
extern char* CpyStringN(char* des, const char* src, int n);

/*
  @name		DestroyString()
  @brief	no use
  @param		 [I/O] 
  @return		
  */
//void DestroyString(char* s);

/**
  @name		IsStringEmpty()
  @brief	like strlen()
			
  @param	const char* [I/ ] string
  @return	BOOLEAN TRUE = string is empty, or FALSE.
  */
extern BOOLEAN IsStringEmpty(const char* s);

/**
  @name		CmpStringN
  @brief	like strncmp()
  @param	[I/O] 
  @return		
  */

int CmpStringN(const char* s, const char* t, int n);
/**
  @name		
  @brief		like strlen
  @param		 [I/O] 
  @return		
  */
extern int GetStringLen(const char* s);
/**
  @name		AppendStringN
  @brief	like strcat
  @param		 [I/O] 
  @return		
  */

extern char* AppendStringN(char* des, const char* src, int n);
/**
  @name		GetSubString
  @brief	get sub string from string
  @param	char* sub [I/ ] sub string
			const char* str [I/ ] main string
			int pos [ /O] sub string position in main
			int len [ /O] sub string len
  @return	char* substring point
  */
extern char* GetSubString(char* sub, const char* str, int pos, int len);
/**
  @name		FindString()	
  @brief	like strstr
  @param		 [I/O] 
  @return		
  */

extern char* FindString(const char* s, const char* t);

/**
  @name		ReplaceString()
  @brief	replace strNew to strOld in s
  @param	char* s[I/ ] main string
  @param	char* strOld[I/ ] old string
  @param	char* strNew[I/ ] old string
  @return	void	
  */
extern void ReplaceString(char* s, const char* strOld, const char* strNew);
/**
  @name		InsertString
  @brief	insert sub string to main string pos position
  @param	char* s [I/ ] main string 
  @param	int pos [I/ ] position 
  @param	char* t [I/ ] sub string
  @return	void	
  */
void InsertString(char* s, int pos, const char* t);

/**
  @name		DeletsString()	
  @brief	delete len chars from position in string
  @param	char* s [I/O] string
  @param	int pos [I/O] position
  @param	int len [I/O] length chars to delete
  @return	void
  */
void DeleteString(char* s, int pos, int len);

/**
  @name		ClearString
  @brief	clean string
  @param	char* s [I/ ] 
  @return	void
  */
void ClearString(char* s);

#endif	//DSSTRING_H
