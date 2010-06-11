#include "dbf.h"
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif //WIN32
#include <string.h>
#include <stdlib.h>
void ShowDBFData(struct DBFFILE* dbfp);
void ShowDBFHeadInfo(struct DBFFILE* dbfp);
void ShowDBFHeadInfo(struct DBFFILE* dbfp);
void FormatFields(char *buf, int FieldsIndex, int length, struct DBFFILE* dbfp);
void ShowDBFRecords(struct DBFFILE* dbfp);
void ShowDBFRecord(long RecIndex, struct DBFFILE* dbfp);
void ShowFields(struct DBFFILE* dbfp);
void FormatFields(char *buf, int FieldsIndex, int length, struct DBFFILE* dbfp);
void FormatRecords(char *buf, char *rec, int FieldsPtr, int length, struct DBFFILE* dbfp);
void AppandADBFRecord(struct DBFFILE* dbfp);
void AppandDBFRecords(struct DBFFILE* dbfp);
void SaveDBFRecords(int RecIndex, struct DBFFILE* dbfp);
int main()
{
	char filename[260];
	struct DBFFILE* dbfp;
	char answer[8];
	printf("file name:\n");
	scanf("%s", filename);
	printf("%s:\n", filename);
	if (IsDBF(filename) < 0)
	{
		printf("NO DBF\n");
        return 0;
	}
	else
	{
		printf("DBF\n");
	}
	dbfp = OpenDBF(filename, DB_WR);
	if (0 == dbfp)
	{
		printf("OpenDBF FAIL\n");
		exit(1);
	}
	printf("OpenDBF Success\n");
	while (1)
	{
		printf("++++++++++++++++++++++++\n");
		printf("+1. show struct        +\n");
		printf("+2. show data          +\n");
		printf("+3. appand a Record    +\n");
		printf("+4. appand N Record    +\n");
		printf("+5. save records       +\n");
		printf("+6. compare            +\n");
		printf("+7. exit               +\n");
		printf("++++++++++++++++++++++++\n");
		memset(answer, 0, 8);
		printf("choice(1-7)\n");
		printf("answer:\n");
		scanf("%s", answer); 
		switch( answer[0] )
		{
		case '1':
			ShowDBFHeadInfo(dbfp);
			break;
		case '2':
			ShowDBFRecords(dbfp);
			//ShowDBFData(dbfp);
			break;
		case '3':
			AppandADBFRecord(dbfp);
			break;
		case '4':
			//AppandDBFRecords(dbfp);
			printf("Nothing to do.\n");
			break;
		case '5':
			SaveDBFRecords(0, dbfp);
			break;
		case '6':
			break;
		case '7':
			CloseDBF(dbfp);	
			printf("CloseDBF Success\n");
			return;
		}
	}	
	return 0;
}
	
void ShowDBFHeadInfo(struct DBFFILE* dbfp)
{
	int i = 0;
	printf("LastUpdate: \n");
	printf("%d/%d/%d", 
			(int)(dbfp->hi.LastUpdate[0]), 
			(int)(dbfp->hi.LastUpdate[1]), 
			(int)(dbfp->hi.LastUpdate[2]));
	printf("	totle %d records\n", dbfp->hi.RecNum);
	printf("%d fields: [record length = %d]\n", dbfp->hi.FieldsNum, dbfp->hi.RecLength);
	printf("+----------+----+----------+---+----------+\n");
	printf("+%-10s+%s+%-10s+%s+%-10s+\n", "name", "type", "width", "dec", "offset");
	printf("+----------+----+----------+---+----------+\n");
	for (i=0; i<dbfp->hi.FieldsNum; i++)
	{
		printf("+%-10s+%c   +%-10d+%c  +%-10d+\n", 
				dbfp->hi.fl[i].name,
				dbfp->hi.fl[i].type,
				dbfp->hi.fl[i].width,
				dbfp->hi.fl[i].dec,
				dbfp->hi.fl[i].offset);	//cout << setiosflags(cout.left);
		printf("+----------+----+----------+---+----------+\n");
	}
	return;

}

void ShowDBFData(struct DBFFILE* dbfp)
{
	char answer[8];
	long RecsTotal = dbfp->hi.RecNum;
	long RecIndex = 0L;
	while (RecIndex < RecsTotal)
	{
		long num = RecsTotal - RecIndex > 24 ? 24 : RecsTotal - RecIndex;
		long i;
		for(i=0; i<num; i++ )
		{
			ShowDBFRecord(RecIndex, dbfp);
			RecIndex++;
		}
		printf("continue(y/n)\n");
		scanf("%s", answer);
		if( answer[0]!='y' && answer[0]!='Y' )
		{
			break;
		}
	}
}

void ShowDBFRecords(struct DBFFILE* dbfp)
{
	int LinesPerRec;
	int RecordsPerScr;
	int ShowWidth = 0;
	int j;
	int i;
	int i1;
	int i2;
	char answer[8];

	if (dbfp->hi.RecNum <= 0)
	{
		return;
	}

	for (j=0; j<dbfp->hi.FieldsNum; j++)
	{
		int tmp = strlen(dbfp->hi.fl[j].name);
		ShowWidth += (dbfp->hi.fl[j].width > tmp ? dbfp->hi.fl[j].width : tmp) + 1;
	}
	LinesPerRec = (ShowWidth-1)/80+1;
	RecordsPerScr = 23/LinesPerRec-1;
	for(i=0; i< dbfp->hi.RecNum/RecordsPerScr; i++ )
	{
		ShowFields(dbfp);
		for(i1=0; i1<RecordsPerScr; i1++ )
		{
			ShowDBFRecord(i*RecordsPerScr+i1, dbfp);
		}
		printf("continue(y/n)\n");
		memset(answer, 0, 8);
		scanf("%s", answer);
		if( answer[0]!='y' && answer[0]!='Y' )
		{
			return;
		}
	}

	//SHOW LEFT RECORDS
	ShowFields(dbfp);
	for(i2=0; i2<dbfp->hi.RecNum%RecordsPerScr; i2++ )
	{
		ShowDBFRecord(i*RecordsPerScr + i2, dbfp);
	}
}

void ShowDBFRecord(long RecIndex, struct DBFFILE* dbfp)
{
	char *buf, *dbuf;
	int nMaxLen = GetMaxLenOfCol(dbfp);

	buf = (char*)malloc(sizeof(char)*dbfp->hi.RecLength);
	dbuf = (char*)malloc(sizeof(char)*nMaxLen+1);

	GetRecord(RecIndex, buf, dbfp);
	FormatRecords(dbuf, buf, 0, nMaxLen, dbfp);
	printf("%s\n", dbuf);
	free(buf);
	free(dbuf);
}

void FormatFields(char *buf, int FieldsIndex, int length, struct DBFFILE* dbfp)
{
	int i;
	int	ptr = 0;

	if( length <= 0 )
	{
		return;
	}
	memset(buf, 0x20, length);

	for( i=FieldsIndex; i<dbfp->hi.FieldsNum; i++ )
	{
		int j = 0;
		int tmp = strlen(dbfp->hi.fl[i].name);
		int ptrnew = ptr + 1 +
					(dbfp->hi.fl[i].width > tmp ? dbfp->hi.fl[i].width : tmp);
		if( ptrnew > length )
		{
			buf[length] = 0;
			return;
		}
		while ((buf[ptr + j] = dbfp->hi.fl[i].name[j] ) != 0)
		{
			j++;
		}
		buf[ptr+j] = 0x20;
		ptr =  ptrnew;
		buf[ptr-1] = 0x20;
	}
	buf[ptr-1]= 0;
}

void ShowFields(struct DBFFILE* dbfp)
{
	char *buf;
	int nMaxLen = GetMaxLenOfCol(dbfp);
	buf = (char*)malloc(sizeof(char)*(nMaxLen+1));
	FormatFields(buf, 0, nMaxLen, dbfp);
	printf("%s\n", buf);
	free(buf);
}

void FormatRecords(char *buf, char *rec, int FieldsPtr, int length, struct DBFFILE* dbfp)
{
	int i;
	unsigned int ptr = 0;

	memset( buf, ' ', length );

	for( i=FieldsPtr; i<dbfp->hi.FieldsNum; i++ )
	{
		int tmp = strlen( dbfp->hi.fl[i].name );
		int ptrnew = ptr + 1 + ( dbfp->hi.fl[i].width > tmp ? dbfp->hi.fl[i].width : tmp );
		if( ptrnew > length )
		{
			buf[length] = '\0';
			return;
		}

		if( rec )
		{
			memcpy(buf + ptr, rec + dbfp->hi.fl[i].offset, dbfp->hi.fl[i].width );
		}
		else
		{
			int j;
			for(j=ptr; j<ptrnew - 1; j++ )
			{
				buf[ j ] = '-';
			}
		}

		ptr = ptrnew;
		buf[ptr-1] = ' ';
	}
	buf[ptr-1] = '\0';
}

void AppandADBFRecord(struct DBFFILE* dbfp)
{
	char* buf;
	int len;
	len = dbfp->hi.RecLength;
	buf = (char*)malloc(sizeof(char)*len);
	memset(buf, 0x20, len);
	buf[1] = '9';
	buf[3] = 'z';
	buf[4] = 'u';
	buf[5] = 'o';
	if (AppendRecord(buf, dbfp)<0)
	{
		printf("FAIL\n");
	}
	else
	{
		printf("Success\n");
	}
	free(buf);
	return;
}
void SaveDBFRecords(int RecIndex, struct DBFFILE* dbfp)
{
	char* buf;
	int len;
	len = dbfp->hi.RecLength;
	buf = (char*)malloc(sizeof(char)*len);
	memset(buf, 0x20, len);
	buf[1] = '9';

	buf[3] = 'z';
	buf[4] = 'u';
	buf[5] = 'o';
	if (SaveRecords(0, buf, 1, dbfp)<0)
	{
		printf("FAIL\n");
	}
	else
	{
		printf("Success\n");
	}
	free(buf);
	return;
}
	




