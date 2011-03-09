#include "zjtype.h"
int
main(int argc, char* argv[])
{
	ZJBOOL bool_value;
	status status_value;
	zjint8 int8_value;
	printf("sizeof(ZJBOOL)=%d\n", sizeof(ZJBOOL));
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
	
	printf("sizeof(status)=%d\n", sizeof(status));
	
	printf("ZJ_MAX_PATH=%d\n", ZJ_MAX_PATH);
	
	printf("sizeof(zjint8)=%d\n", sizeof(zjint8));
	printf("ZJINT8_MIN=%d\n", ZJINT8_MIN);
	printf("ZJINT8_MAX=%d\n", ZJINT8_MAX);
	printf("ZJUINT8_MAX=%u\n", ZJUINT8_MAX);

	printf("sizeof(zjint16)=%d\n", sizeof(zjint16));
	printf("ZJINT16_MIN=%d\n", ZJINT16_MIN);
	printf("ZJINT16_MAX=%d\n", ZJINT16_MAX);
	printf("ZJUINT16_MAX=%u\n", ZJUINT16_MAX);

	printf("sizeof(zjint32)=%d\n", sizeof(zjint32));
	printf("ZJINT32_MIN=%d\n", ZJINT32_MIN);
	printf("ZJINT32_MAX=%d\n", ZJINT32_MAX);
	printf("ZJUINT32_MAX=%u\n", ZJUINT32_MAX);

	printf("sizeof(zjint64)=%d\n", sizeof(zjint64));
	printf("ZJINT64_MIN=%I64d\n", ZJINT64_MIN);
	printf("ZJINT64_MAX=%I64d\n", ZJINT64_MAX);
	printf("ZJUINT64_MAX=%I64u\n", ZJUINT64_MAX);

	printf("sizeof(float32)=%d\n", sizeof(zjfloat32));
	printf("sizeof(float64)=%d\n", sizeof(zjfloat64));
	return 0;
}
