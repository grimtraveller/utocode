#include "jztype.h"
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
	return 0;
}
