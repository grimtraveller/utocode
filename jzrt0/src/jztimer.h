/**
 * @file	jztimer.h 
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
#ifndef JZTIMER_H
#define JZTIMER_H
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "jztype.h"
#define START_JZTIMER() \
	jztimer_st timer;\
	init_jztimer(&timer);\
	start_jztimer(&timer);
#define STOP_JZTIMER(x) \
	stop_jztimer(&timer); \
	x = timer.rtime;\

typedef struct _performance_timer_st
{
	BOOLEAN bOK;
	jzint64 unit;
	jzint64 start;
	jzint64 stop;
	jzint64 rtime;
} jztimer_st;
void init_jztimer(jztimer_st* ptimer);
void start_jztimer(jztimer_st* ptimer);
void stop_jztimer(jztimer_st* ptimer);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //XXXXX_H