#include "zjrt0.h"
int
main(int argc, char* argv[])
{
	zjuint8* pHamming = NULL;
	zjuint8 test[1024] = "01101001";
	zjuint8* p;
	zjuint32 i = 0;
	zjuint32 len = 0;
	p = (zjuint8*)malloc(strlen(test)+1);
	memset(p, 0, strlen(test)+1);
	for (i = 0; i < strlen(test); i++)
	{
		*(p+i) = test[strlen(test)-i-1];
	}
	strcpy(test, p);
	i = 0;
	while(test[i] != '\0')
	{
		if (test[i] == '0')
		{
			test[i] = 0;
		}
		else
		{
			test[i] = 1;
		}
		i++;
	}
	len = HammingCode(&pHamming, test, i, FALSE);
	for (i = 0; i < len; i++)
	{
		printf("%d", pHamming[len-i-1]);
	}
	printf("\n");
	free(pHamming);
	
	return 0;
}	
