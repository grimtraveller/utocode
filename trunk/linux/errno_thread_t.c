/*
 * Name:	errno_thread.c
 * Desc:	when compile in gcc no argument, errno is thread safe or not.
 * Usage:	$>gcc  errno_thread.c -o errno_thread
 * Author:	zuohaitao
 * Date:
 */
#include <stdio.h>

int main()
{
# ifndef __ASSEMBLER__
#  if !defined _LIBC || defined _LIBC_REENTRANT
	printf("#   define errno (*__errno_location ())\n");
#  endif
# endif /* !__ASSEMBLER__ */
	return 0;
}
