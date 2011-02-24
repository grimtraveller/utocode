#include "jztype.h"
#include "jzmem.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef NULL
#define NULL 0
#endif	// NULL

//计算校验位位数
jzuint32 GetParityLen(jzuint32 nLen)
{
	jzuint32 nParityLen = 0;
	jzuint32 i = 0;
	while(1)
	{
		if (pow(2, nParityLen) >= nParityLen + nLen)
		{
			return nParityLen;
		}
		nParityLen++;
	}
	return 0;
}

//生成海明码
jzuint32 HammingCode(jzuint8** ppHamming, jzuint8* pData, jzuint32 nDataLen, BOOLEAN flag)
{
	jzuint8* pHamming = NULL;
	//hamming code length
	jzuint32 nHammingLen = 0;
	//parity bit index
	jzuint32 nParityIdx = 1;  
	jzuint32 nHammingIdx = 1;  
	jzuint32 nParityPos = 1;	
	jzuint32 k = 0;	
	jzuint32 nDataIdx = 1;
	jzuint8* pPDtbl = NULL;	//ParityBit DataBit table
	jzuint32 nTblLen = 0;
	jzuint32 tempIdx = 0;
	jzuint32 value = 0;
	jzuint32 nParityLen = GetParityLen(nDataLen);
	nHammingLen = nParityLen+nDataLen;
	nTblLen = nParityLen*nDataLen;
	pPDtbl = (jzuint8*)JZMALLOC(nTblLen*sizeof(jzuint8));
	memset(pPDtbl, 0, nTblLen*sizeof(jzuint8));
	pHamming = (jzuint8*)JZMALLOC(nHammingLen*sizeof(jzuint8));
	
	while(nHammingIdx <= nHammingLen)
	{
		if (nHammingIdx == nParityPos)
		{
			nHammingIdx++;
			nParityIdx++;
			nParityPos  = (jzuint32)pow(2, nParityIdx-1);
			continue;
		}

		assert(nHammingIdx < nParityPos);
		k = nParityIdx;
		tempIdx = 0;
		//save Parity Bit index that checks current Data Bit(i) to ParityBit DataBit table
		while(1)
		{
			if (k-1 < 0)
			{
				break;
			}
			tempIdx += (jzuint32)pow(2, k-1);
			if (tempIdx > nHammingIdx)
			{
				//rollback temp value
				tempIdx -= (jzuint32)pow(2, k-1);
				k--;
			}
			else /*if (tempIdx <= nHammingIdx)*/
			{
				*(pPDtbl+(k-1)*nDataLen+nDataIdx-1) = 1;
				k--;
				if (tempIdx == nHammingIdx)
				{
					break;
				}
			}
		}

		*(pHamming+nHammingIdx-1) = *(pData+nDataIdx-1);
		nDataIdx++;
		nHammingIdx++;
	}
	
	for (nParityIdx = 0; nParityIdx < nParityLen; nParityIdx++)
	{
		value = 0;
		for (nDataIdx = 0; nDataIdx < nDataLen; nDataIdx++)
		{
			if (0 == *(pPDtbl+nParityIdx*nDataLen+nDataIdx))
			{
				continue;
			}
			value += *(pData+nDataIdx);
		}
		
		if (0 == flag)
		{
			//偶校验
			*(pHamming+(jzuint32)pow(2, nParityIdx)-1) = value%2;
			
		}
		else
		{
			//奇校验
			*(pHamming+(jzuint32)pow(2, nParityIdx)-1) = (0 == value%2)?1:0;

		}

	}
	free(pPDtbl);
	pPDtbl = NULL;
	*(ppHamming) = pHamming;
	return nHammingLen;
}

int
main(int argc, char* argv[])
{
	jzuint8* pHamming = NULL;
	jzuint8 test[1024] = "01101001";
	jzuint8* p;
	jzuint32 i = 0;
	jzuint32 len = 0;
	p = (jzuint8*)malloc(strlen(test)+1);
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
