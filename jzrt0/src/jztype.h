/**
  @file		jztyep.h
  @brief	common macro, type, operation, and myself type for portable 
  @details	
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug		
  */
#ifndef JZTYPE_H
#define JZTYPE_H
/**
 * @brief inalid point
 */
#ifndef NULL
#define NULL	0
#endif
/**
 * @brief value only is 0 (FALSE) or 1 (TRUE)
 */
#define BOOLEAN		long
/**
 * @brief true the value of boolean
 */
#define	TRUE		1
/**
 * @brief false the value of boolean
 */
#define FALSE		0
/**
 * @brief success the value of status
 */
#define	OK			1
/**
 * @brief error the value of status
 */
#define	ERROR		0
/**
 * @brief unable error the value of status
 */
#define	INFEASIBLE	-1
/**
 * @brief overflow error the value of status
 */
#define	OVERFLOW	-2
/**
 * logic operation 'and'
 */
#define	AND		&&
/**
 * logic operation 'or'
 */
#define	OR		||
/**
 * @brief function status normally, is OK ERROR 
 */
typedef int status;
//USELESS
#define MACHINE_WORD_LEN (sizeof(long)/sizeof(char))
#define JZ_MAX_PATH	260

/*USELESS
 * @brief zjmem size type
 */
typedef unsigned int size_jz;
#endif
