#include "zjtype.h"
#include <stdio.h>
void printCDataTypeSize()
{
	printf("====C Data Type Size====\n");
	printf("\tsizeof(char)=%lu\n", sizeof(char));
	printf("\tsizeof(short)=%lu\n", sizeof(short));
	printf("\tsizeof(long)=%lu\n", sizeof(long));
	printf("\tsizeof(double)=%lu\n", sizeof(double));
	printf("\tsizeof(float)=%lu\n", sizeof(float));
	printf("\tsizeof(int)=%lu\n", sizeof(int));
	printf("\tsizeof(long long)=%lu\n", sizeof(long long));
	printf("\tsizeof(size_t)=%lu\n", sizeof(size_t));
}
int
main(int argc, char* argv[])
{
	printf("NULL=%d\n", NULL);
	printf("sizeof(ZJBOOL)=%lu\n", sizeof(ZJBOOL));
	printf("TRUE=%d\n", TRUE);
	printf("FALSE=%d\n", FALSE);
	printf("ZJOK=%d\n", ZJOK);
	printf("ZJERROR=%d\n", ZJERROR);
	printf("ZJINFEASIBLE=%d\n", ZJINFEASIBLE);
	printf("ZJOVERFLOW=%d\n", ZJOVERFLOW);
	
	printf("sizeof(zjint8)=%lu\n", sizeof(zjint8));
	printf("ZJINT8_MIN=%d\n", ZJINT8_MIN);
	printf("ZJINT8_MAX=%d\n", ZJINT8_MAX);
	
	printf("sizeof(zjuint8)=%lu\n", sizeof(zjuint8));
	printf("ZJUINT8_MAX=%u\n", ZJUINT8_MAX);
	
	printf("sizeof(zjint16)=%lu\n", sizeof(zjint16));
	printf("ZJINT16_MIN=%d", ZJINT16_MIN);
	printf("ZJINT16_MAX=%d", ZJINT16_MAX);
	
	printf("sizeof(zjuint16)=%lu", sizeof(zjuint16));
	printf("ZJUINT16_MAX=%u", ZJUINT16_MAX);
	
	printf("sizeof(zjint32)=%lu", sizeof(zjint32));
	printf("ZJINT32_MIN=%d", ZJINT32_MIN);
	printf("ZJINT32_MAX=%d", ZJINT32_MAX);
	
	printf("sizeof(zjuint32)=%lu", sizeof(zjuint32));
	printf("ZJUINT32_MAX=%u", ZJUINT32_MAX);
	
	printf("sizeof(zjint64)=%lu", sizeof(zjint64));
	printf("ZJINT64_MIN=%I64d", ZJINT64_MIN);
	printf("ZJINT64_MAX=%I64d", ZJINT64_MAX);
	
	printf("sizeof(zjuint64)=%lu", sizeof(zjuint64));
	printf("ZJUINT64_MAX=%I64u", ZJUINT64_MAX);
	
	printf("sizeof(zjfloat32)=%lu", sizeof(zjfloat32));

	printf("sizeof(zjfloat64)=%lu", sizeof(zjfloat64));
	
	if (1 AND 1)
	{
		printf("&& == AND\n");
	}
	if (1 OR 0)
	{
		printf("|| == OR\n");
	}
	
	printf("ZJ_MAX_PATH=%d\n", ZJ_MAX_PATH);
	
	return 0;
}
