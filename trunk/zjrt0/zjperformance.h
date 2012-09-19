#ifndef ZJPERFORMANCE_H
#define ZJPERFORMANCE_H
#include "zjtype.h"

/*
 * if the function fails, the return value is FALSE.
 */
ZJBOOL zjqueryPerformanceUnit(zjint64* unit);
ZJBOOL zjqueryPerformanceCounter(zjint64* counter);
zjint64 zjcalcPerformance(zjint64 start, zjint64 end, zjint64 unit);
#endif //ZJPERFORMANCE_H
