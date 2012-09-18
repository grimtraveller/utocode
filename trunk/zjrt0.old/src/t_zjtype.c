#include "zjtype.h"
#include <stdio.h>
int
main(int argc, char* argv[])
{
	ZJBOOL bool_value;
	status status_value;
	zjint8 int8_value;
	bool_value = TRUE;
	status_value = ZJOK;
	int8_value = 127;
#if defined(WIN32)
	printf("sizeof(ZJBOOL)=%d\n", sizeof(ZJBOOL));
#elif defined(MAC64)
	printf("sizeof(ZJBOOL)=%lu\n", sizeof(ZJBOOL));
#else
	printf("sizeof(ZJBOOL)=%d\n", sizeof(ZJBOOL));
#endif 
	printf("TRUE == %d\n", TRUE);
	printf("FALSE == %d\n", FALSE);
	printf("ZJOK == %d\n", ZJOK);
	printf("ZJERROR == %d\n", ZJERROR);
	printf("ZJINFEASIBLE == %d\n", ZJINFEASIBLE);
	printf("ZJOVERFLOW == %d\n", ZJOVERFLOW);
	if (1 AND 1)
	{
		printf("&& == AND\n");
	}
	if (1 OR 0)
	{
		printf("|| == OR\n");
	}
	
#if defined(WIN32)
	printf("sizeof(status)=%d\n", sizeof(status));
#elif defined(MAC64)
	printf("sizeof(status)=%lu\n", sizeof(status));
#else
	printf("sizeof(status)=%d\n", sizeof(status));
#endif 
	
	printf("ZJ_MAX_PATH=%d\n", ZJ_MAX_PATH);
	
#if defined(WIN32)
	printf("sizeof(zjint8)=%d\n", sizeof(zjint8));
#elif defined(MAC64)
	printf("sizeof(zjint8)=%lu\n", sizeof(zjint8));
#else
	printf("sizeof(zjint8)=%d\n", sizeof(zjint8));
#endif 
	printf("ZJINT8_MIN=%d\n", ZJINT8_MIN);
	printf("ZJINT8_MAX=%d\n", ZJINT8_MAX);
	printf("ZJUINT8_MAX=%u\n", ZJUINT8_MAX);

#if defined(WIN32)
	printf("sizeof(zjint16)=%d\n", sizeof(zjint16));
#elif defined(MAC64)
	printf("sizeof(zjint16)=%lu\n", sizeof(zjint16));
#else
	printf("sizeof(zjint16)=%d\n", sizeof(zjint16));
#endif 
	printf("ZJINT16_MIN=%d\n", ZJINT16_MIN);
	printf("ZJINT16_MAX=%d\n", ZJINT16_MAX);
	printf("ZJUINT16_MAX=%u\n", ZJUINT16_MAX);

#if defined(WIN32)
	printf("sizeof(zjint32)=%d\n", sizeof(zjint32));
#elif defined(MAC64)
	printf("sizeof(zjint32)=%lu\n", sizeof(zjint32));
#else
	printf("sizeof(zjint32)=%d\n", sizeof(zjint32));
#endif 
	printf("ZJINT32_MIN=%d\n", ZJINT32_MIN);
	printf("ZJINT32_MAX=%d\n", ZJINT32_MAX);
	printf("ZJUINT32_MAX=%u\n", ZJUINT32_MAX);

#if defined(WIN32)
	printf("sizeof(zjint64)=%d\n", sizeof(zjint64));
#elif defined(MAC64)
	printf("sizeof(zjint64)=%lu\n", sizeof(zjint64));
#else
	printf("sizeof(zjint64)=%d\n", sizeof(zjint64));
#endif 
#if defined(WIN32)
	printf("ZJINT64_MIN=%I64d\n", ZJINT64_MIN);
	printf("ZJINT64_MAX=%I64d\n", ZJINT64_MAX);
	printf("ZJUINT64_MAX=%I64u\n", ZJUINT64_MAX);
#elif defined(MAC64)
	printf("ZJINT64_MIN=%ld\n", ZJINT64_MIN);
	printf("ZJINT64_MAX=%ld\n", ZJINT64_MAX);
	printf("ZJUINT64_MAX=%lu\n", ZJUINT64_MAX);
#else
	printf("ZJINT64_MIN=%ld\n", ZJINT64_MIN);
	printf("ZJINT64_MAX=%ld\n", ZJINT64_MAX);
	printf("ZJUINT64_MAX=%lu\n", ZJUINT64_MAX);
#endif

#if defined(WIN32)
	printf("sizeof(float32)=%d\n", sizeof(zjfloat32));
	printf("sizeof(float64)=%d\n", sizeof(zjfloat64));
#elif defined(MAC64)
	printf("sizeof(float32)=%lu\n", sizeof(zjfloat32));
	printf("sizeof(float64)=%lu\n", sizeof(zjfloat64));
#else
	printf("sizeof(float32)=%d\n", sizeof(zjfloat32));
	printf("sizeof(float64)=%d\n", sizeof(zjfloat64));
#endif 
	return 0;
}
