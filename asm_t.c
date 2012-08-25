/*
 * Name:	asm_t.c
 * Purpose:	the guide for asm in c language
 * Usage:	$>gcc src/asm_t.c -ggdb -Wall -o bin/asm_t
 *			$>gdb bin/asm_t
 *			(gdb)dissas main
 *			>cl asm_t.c -D "WIN32"
 *			>asm_t.exe
 * Author:	zuohaitao
 * Date:	2009-07-02
 */

#include <stdio.h>
int main(int argc, char* argv[])
{
#if defined(WIN32)
	__asm
	{
		int 3
		nop
		nop
	}
#elif defined(LINUX)
	__asm__
	(
	"int 3\n\t"
	"nop\n\t"
	"nop\n\t"
	);
#elif defined(MACOSX)
	__asm__
	(
	"NOP\n\t"
	"NOP\n\t"
	);

#else 

#endif
	return 1;
}
