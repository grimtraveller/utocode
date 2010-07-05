/*
 * Name:	asm_t.c
 * Purpose:	the guide for asm in c language
 * Usage:	$>gcc src/asm_t.c -ggdb -Wall -o bin/asm_t
 *			$>gdb bin/asm_t
 *			(gdb)dissas main
 * Author:	zuohaitao
 * Date:	2009-07-02
 */

int main()
{
	__asm__
	(
	"nop\n\t"
	"nop\n\t"
	);
	return 1;
}
