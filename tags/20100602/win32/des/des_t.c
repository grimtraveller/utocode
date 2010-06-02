/*
 * Name:	des_t.c
 * Desc:	test descryption with d3des.c
 * Usage:	>cl des_t.c d3des.c
 *			>des_t.exe zuohaitao
 * Author:	zuohaitao
 * Date:	2009 0207
 * Thanks:	http://topic.csdn.net/t/20050111/14/3716370.html
 */

#include   <stdio.h>  
#include   <string.h>  
#include   <stdlib.h>
#include   "d3des.h"  

int  
des_decryption(unsigned   char   *   szIn,   int   nInlen,   unsigned   char   *   key,   int   type)  
{  
	int   i;  
	unsigned   char   in[8];  
	unsigned   char   out[8];  
	//des_key_schedule   ks;  
	int   nNum;  

	if   (nInlen%8==0)   {  
		nNum   =   nInlen/8;  
	}
	else   {  
		nNum   =   nInlen/8+1;  
	}  

	if   (type==DE1)   {  
		deskey(key,   DE1);  
	}
	else   {  
		deskey(key,   EN0);  
	}  

	for   (i=0;   i<nNum;   i++)  
	{  
		memset(in,   0,   sizeof(in));  
		memset(out,   0,   sizeof(out));  

		if   (i==(nNum-1))   {  
			memcpy(in,   szIn+i*8,   nInlen-i*8);  
		}   else   {  
			memcpy(in,   szIn+i*8,   8);  
		}  

		des(in,   out);  

		if   (i==(nNum-1))   {  
			memcpy(szIn+i*8,   out,   nInlen-i*8);  
		}   else   {  
			memcpy(szIn+i*8,   out,   8);  
		}  
	}  

	return   1;  
}  

	int  
main(int   argc,   char   **argv)  
{  
	int   n   =   0;  
	char   *   key   =   "admin123";  
	char* key2 = "12345678";
	unsigned char* buf = 0;
	int insize = 0;
	n   =   strlen(argv[1]);
	if (0 != n % 8)
	{
		insize = n+8-n%8;

	}
	else
	{
		insize = n;
	}
	buf = malloc(sizeof(unsigned char)*insize);
	if (0 == buf)
	{
		perror("malloc");
		return 1;
	}
	if (0 != n%8)
	{
		memset(buf+n, argv[1][n-1], insize - n);
	}

	memcpy(buf, argv[1], n);
		
	printf("%s         %d\n",   argv[1],   n);  


//des
	des_decryption(buf,   insize,   (unsigned   char   *)key,   EN0);  
	des_decryption(buf,   insize,   (unsigned   char   *)key,   DE1);  
	buf[n] = '\0';
	printf("des:%s         %d\n",   buf,   n); 
//3des
	des_decryption(buf, insize, (unsigned char*)key, EN0);
	des_decryption(buf, insize, (unsigned char*)key2, EN0);
	des_decryption(buf, insize, (unsigned char*)key, EN0);

	des_decryption(buf,   insize,(unsigned char*)key,   DE1);  
	des_decryption(buf,   insize,(unsigned char*)key2,   DE1);  
	des_decryption(buf,   insize,(unsigned char*)key,   DE1);  
	buf[n] = '\0';
	printf("3des:%s         %d\n",   buf,   n); 


	return   0;  
}   

