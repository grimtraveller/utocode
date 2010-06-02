#ifndef PUBLIC_H
#define PUBLIC_H
#define	COMMONLEN	128
#define	MAXPATH		260
#ifndef NULL
#define	NULL		0
#endif

/*
#ifndef FALSE＆TRUE
#define	TRUE		(~0)
#define	FALSE		(0)
#endif
*/

enum pfxerrstyle_t
{
	noerr = 0x0000,
	syserr = 0x0001,
	pfxerr = 0x0002
};
#define LOGSUFFIX	".log"
#define PRSNTLIST_KEY	1202
#endif
