/*Name:		fts_t.c
 *Desc:		test fts_open() fts_read() fts_close()
			file tree walk in BSD system, there is no ftw() in BSD.
 *Usage:	$gcc fts_t.c -o fts_t
 *Author:	zuohaitao
 *Date:		2008 12 31
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	char filename[260] = {0};
	char* end = 0;
	char* file[2];
	int bit_flags = 0;
	FTS* fts;
	FTSENT* ent;
	int ok = 1;
	file[0] = filename;
	file[1] = end;
	strcpy(filename, "/home/zuohaitao/src/fts_t.c");
	bit_flags = FTS_PHYSICAL/* | FTS_TIGHT_CYCLE_CHECK*/;
	fts = fts_open(file, bit_flags, 0);
	if (0 == fts)
	{
		perror("fts_open");
	}
	while(1)
	{
		ent =  fts_read(fts);
		printf("%s\t\%d\n", file[0], ent->fts_statp->st_size);
		break;
	}
	fts_close(fts);
	return 0;
}

