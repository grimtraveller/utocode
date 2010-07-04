/**
 * @file	jzrand.h 
 * @brief	implement rand functions
 * @details	
 * @author	zuohaitao
 * @date	2010-07-03 
 * @warning	
 * @bug	
 */
#ifndef JZRAND_H
#define JZRAND_H
#include "jztype.h"
#include <time.h>
EXTERN_C_BEGIN
#define jzrand()	rand()
#define jzsrand()	srand((unsigned int)time(NULL))
extern int jzrand_scope(int min, int max);
extern BOOLEAN jzrand_boolean();
EXTERN_C_END
#endif //JZRAND_H
