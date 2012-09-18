#include "zjtype.h"
#include "zjmem.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//count the parity length
unsigned int zjGetParityLen(int nLen)
{
	zjuint32 nParityLen = 0;
	zjuint32 i = 0;
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

//make hamming code
unsigned int zjHammingCode(unsigned char** ppHamming, unsigned char* pData, unsigned int nDataLen, long flag)
{
	zjuint8* pHamming = NULL;
	//hamming code length
	zjuint32 nHammingLen = 0;
	//parity bit index
	zjuint32 nParityIdx = 1;  
	zjuint32 nHammingIdx = 1;  
	zjuint32 nParityPos = 1;	
	zjuint32 k = 0;	
	zjuint32 nDataIdx = 1;
	zjuint8* pPDtbl = NULL;	//ParityBit DataBit table
	zjuint32 nTblLen = 0;
	zjuint32 tempIdx = 0;
	zjuint32 value = 0;
	zjuint32 nParityLen = zjGetParityLen(nDataLen);
	nHammingLen = nParityLen+nDataLen;
	nTblLen = nParityLen*nDataLen;
	pPDtbl = (zjuint8*)ZJMALLOC(nTblLen*sizeof(zjuint8));
	memset(pPDtbl, 0, nTblLen*sizeof(zjuint8));
	pHamming = (zjuint8*)ZJMALLOC(nHammingLen*sizeof(zjuint8));
	
	while(nHammingIdx <= nHammingLen)
	{
		if (nHammingIdx == nParityPos)
		{
			nHammingIdx++;
			nParityIdx++;
			nParityPos  = (zjuint32)pow(2, nParityIdx-1);
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
			tempIdx += (zjuint32)pow(2, k-1);
			if (tempIdx > nHammingIdx)
			{
				//rollback temp value
				tempIdx -= (zjuint32)pow(2, k-1);
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
			//odd check
			*(pHamming+(zjuint32)pow(2, nParityIdx)-1) = value%2;
			
		}
		else
		{
			//even check 
			*(pHamming+(zjuint32)pow(2, nParityIdx)-1) = (0 == value%2)?1:0;

		}

	}
	free(pPDtbl);
	pPDtbl = NULL;
	*(ppHamming) = pHamming;
	return nHammingLen;
}

