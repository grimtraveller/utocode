/**
 * @file	zjrand.h 
 * @brief	implement rand functions
 * @details	
 * @author	zuohaitao
 * @date	2010-07-03 
 * @warning	
 * @bug	
 */
#ifndef ZJRAND_H
#define ZJRAND_H
#include "zjtype.h"
#include <time.h>
EXTERN_C_BEGIN
#define zjrand()	rand()
#define zjsrand()	srand((unsigned int)time(NULL))
extern int zjrand_scope(int min, int max);
extern ZJBOOL zjrand_boolean();
EXTERN_C_END
#endif //ZJRAND_H
