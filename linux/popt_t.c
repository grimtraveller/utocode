/*
 * Name:	popt_t.c
 * Desc:	this is a example about opot usage in `man popt`
 * Usage:	$ wget http://rpm5.org/files/popt/popt-1.14.tar.gz
 *			$ tar -vxxf popt-1.14.tar.gz
 *			$ ./configure
 *			$ make 
 *			$ make install
 *			$ gcc popt_t.c -I/usr/local/include -L/usr/local/lib -lpopt -o popt_t
 * Author:	zuohaitao
 * Date:	2010-03-16
 */

#include <popt.h>
#include <stdio.h>
#include <stdlib.h>
void usage(poptContext optCon, int exitcode, char *error, char *addl) {
    poptPrintUsage(optCon, stderr, 0);
    if (error) fprintf(stderr, "%s: %s\0", error, addl);
    exit(exitcode);
}

int main(int argc, char *argv[]) {
   char    c;            /* used for argument parsing */
   int     i = 0;        /* used for tracking options */
   char    *portname;
   int     speed = 0;    /* used in argument parsing to set speed */
   int     raw = 0;      /* raw mode? */
   int     j;
   char    buf[BUFSIZ+1];
   poptContext optCon;   /* context for parsing command-line options */

   struct poptOption optionsTable[] = {
      { "bps", 'b', POPT_ARG_INT, &speed, 0, "signaling rate in bits-per-second", "BPS" },
      { "crnl", 'c', 0, 0, 'c', "expand cr characters to cr/lf sequences", NULL },
      { "hwflow", 'h', 0, 0, 'h', "use hardware (RTS/CTS) flow control", NULL },
      { "noflow", 'n', 0, 0, 'n', "use no flow control", NULL },
      { "raw", 'r', 0, &raw, 0, "don't perform any character conversions", NULL },
      { "swflow", 's', 0, 0, 's', "use software (XON/XOF) flow control", NULL } ,
      POPT_AUTOHELP
      { NULL, 0, 0, NULL, 0 }
    };

   optCon = poptGetContext(NULL, argc, (const char**)argv, optionsTable, 0);
   poptSetOtherOptionHelp(optCon, "[OPTIONS]* <port>");

   if (argc < 2) {
	   poptPrintUsage(optCon, stderr, 0);
		exit(1);
   }


   /* Now do options processing, get portname */
   while ((c = poptGetNextOpt(optCon)) >= 0) {
      switch (c) {
       case 'c':
          buf[i++] = 'c';
          break;
       case 'h':
          buf[i++] = 'h';
          break;
       case 's':
          buf[i++] = 's';
          break;
       case 'n':
          buf[i++] = 'n';
          break;
      }
   }
   portname = (char*)poptGetArg(optCon);
   if((portname == NULL) || !(poptPeekArg(optCon) == NULL))
      usage(optCon, 1, "Specify a single port", ".e.g., /dev/cua0");

   if (c < -1) {
      /* an error occurred during option processing */
      fprintf(stderr, "%s: %s\n",
       poptBadOption(optCon, POPT_BADOPTION_NOALIAS),poptStrerror(c));
      return 1;
   }

   /* Print out options, portname chosen */
   printf("Options  chosen: ");
   for(j = 0; j < i ; j++)
      printf("-%c ", buf[j]);
   if(raw) printf("-r ");
   if(speed) printf("-b %d ", speed);
   printf("\nPortname chosen: %s\n", portname);

   poptFreeContext(optCon);
   exit(0);
}
