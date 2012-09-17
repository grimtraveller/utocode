/*
 * Name:                tmqopen.c
 * Desc:                study posix message queue - mq_open()
 * Usage:
                                $>gcc -lrt tmqopen.c -o tmqopen
                                $>mount -t mqueue none /mnt
                                $>./tmqopen_t
                                $>ls /mnt/mqflag
                                $>rm /mnt/mqflag
 * Date:                2009/01/06
 * Author:              zuohaitao
 */
#include <mqueue.h>
#include <string.h>
int main(int argc, char* argv[])
{
        char mqName[100];
        strncpy(mqName, "/mqflag", 100);
        mqd_t mqd;
        mqd = mq_open(mqName, O_CREAT|O_RDWR, 0777,NULL);
        if (-1 == mqd)
        {
                perror("mq_open");
                return 1;
        }
        mq_close(mqd);
        //mq_unlink(mqName);
        return 0;
}

