/**
 * @file	zjtimer.h 
 * @brief	test performance
 * @details	
 *			usage
 *				init()
 *				start()
 *				stop()
 * @author	zuohaitao
 * @date	2010-06-22 
 * @warning	
 * @bug	
 */
#ifndef ZJTIMER_H
#define ZJTIMER_H
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "zjtype.h"
#define START_ZJTIMER() \
	zjtimer_st timer;\
	init_zjtimer(&timer);\
	start_zjtimer(&timer);
#define STOP_ZJTIMER(x) \
	stop_zjtimer(&timer); \
	x = timer.rtime;\

typedef struct _performance_timer_st
{
	ZJBOOL bOK;
	zjint64 unit;
	zjint64 start;
	zjint64 stop;
	zjint64 rtime;
} zjtimer_st;
void init_zjtimer(zjtimer_st* ptimer);
void start_zjtimer(zjtimer_st* ptimer);
void stop_zjtimer(zjtimer_st* ptimer);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //XXXXX_H
