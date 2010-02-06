#include <stdio.h>
#include <setjmp.h>
int
main(int argc, char** argv)
{
	int label1, label2;
	sigjmp_buf jmp1, jmp2;
	label1 = setjmp(jmp1);
	printf("start\n");
	longjmp(jmp1, label1);
	printf("No JMP\n");
	printf("end\n");
	printf("YES JMP\n");
	return 0;
}
