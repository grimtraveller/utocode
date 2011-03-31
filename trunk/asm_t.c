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

int main()
{
#ifdef WIN32
	__asm
	{
		int 3
		nop
		nop
	}
#else /* linux */
	__asm__
	(
	"int 3\n\t"
	"nop\n\t"
	"nop\n\t"
	);
#endif //WIN32
	return 1;
}
