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
 * @brief true 
 */
#define	TRUE		1
/**
 * @brief false
 */
#define FALSE		0

#define	OK			1
#define	ERROR		0
#define	INFEASIBLE	-1
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
//no used
#define MACHINE_WORD_LEN (sizeof(long)/sizeof(char))
#define JZ_MAX_PATH	260

/** NOUSE
 * @brief zjmem size type
 */
typedef unsigned int size_jz;
#endif
