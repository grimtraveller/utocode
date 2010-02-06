/*
 * usage:
 *  $>gcc uid_t.c -o uid_t
 *  [normal]
 *	$>sudo chown usr ./uid_t
 *	$>ls -l ./uid_t
 *	-rwxr-xr-- uid_t
 *	$>./uid_t
 *	[setuid bit]
 *	$>sudo chown 0 uid_t
 *	$>sudo chomod 04755 uid_t
 *	$>ls -l uid_t
 *	-rwsr-xr-x uid_t
 *	$>./uid_t
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
void printuid();
int 
main(int argc, char* argv[])
{
	uid_t zhtid, iid;
	struct passwd* ppwd;
	ppwd = getpwnam("zuohaitao");
	zhtid = ppwd->pw_uid;
	ppwd = getpwnam("i");
	iid = ppwd->pw_uid;
	printf("zuohaitao=%d\n", zhtid);
	printf("i=%d\n", iid);
	printuid();
	if (-1 == setuid(0))
	{
		perror("");
		return 1;
	}
	printuid();
	return 0;
}

void printuid()
{
	printf("real uid = %d\n", getuid());
	printf("effictive uid = %d\n", geteuid());
}

