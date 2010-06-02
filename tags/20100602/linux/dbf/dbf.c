#include "dbf.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
//////FILE API/////////////////////
int GetFileSize(const char* Name)
{
	struct stat buf;
	if (stat(Name, &buf) < 0)
	{
		return -1;
	}
	else
	{
		//?unsiged int
		return buf.st_size;
	}
}
///////////////////////////////////
//?
/*
int CreateDBF(const char *DBName, struct HeadInfo *phi)
{
	char buf[32];
	int i;
	FILE* pfile;
	pfile = fopen(DBName, "w");
	if (0 == pfile)
	{
		return -1;
	}

	//  create head
	memset(buf, 0, 32);
	buf[0] = FLAGCHAR;
	dbfp->hi.RecOffset = (phi->FieldsNum+1) * 32;
	memcpy(buf+OFDATE, dbfp->hi.LastUpdate, 3);
	memcpy(buf+OFRECNUM, &(dbfp->hi.RecNum), 4);
	short tmp;
	tmp = (short)(dbfp->hi.RecLength);
	memcpy(buf+OFRECLENGTH, &tmp, 2);
	tmp = (short)(dbfp->hi.RecOffset);
	memcpy(buf+OFRECOFFSET, &tmp, 2);
	if (32 != fwrite(buf, sizeof(char), 32, pfile))
	{
		goto FAIL;
	}

	//	create fields list
	for(i=0; i<dbfp->hi.FieldsNum; i++ )
	{
		memset(buf, 0, 32);
		memcpy(buf+OFFLDNAME, dbfp->hi.fl[i].name, 11);
		buf[OFFLDTYPE] = dbfp->hi.fl[i].type;
		buf[OFFLDWIDTH] = dbfp->hi.fl[i].width;
		buf[OFFLDDEC] = dbfp->hi.fl[i].dec;
		memcpy(buf+OFFLDOFFSET, &(dbfp->hi.fl[i].offset), 2);
		if (32 != fwrite(buf, sizeof(char), 32, pfile))
		{
			goto FAIL;
		}
	}
	fclose(pfile);
	return 0;
FAIL:
	fclose(pfile);
	return -2;
}
*/
int	IsDBF( const char *FileName )
{
	int isdbf = 0;
	char buf[32];
	int j;

	/*CFile file;*/
	FILE* pf;
	pf = fopen(FileName, "r");
	if (0 == pf)
	{
		return -1;
	}
	fread(buf, sizeof(char), 32, pf);
	if ((0x03 != buf[0]) && (0x30 != buf[0]))
	{
		isdbf = -3;
		goto RET;
	}
	isdbf = 1;
	for (j = 12; j < 32; j++)
	{
		if ((0 != buf[j]) 
			&& (28 != j) && (29!=j) && (30!=j) && (31!=j))
		{
			isdbf = -3;
			break;
		}
	}
RET:
	fclose(pf);
	return isdbf;
}

struct DBFFILE* OpenDBF(const char *DBName, int mode)
{
	struct DBFFILE* dbfp;
	char buf[32];
	short tmp;
	dbfp = 0;
	//isValid = 1;
	if (!IsDBF(DBName))
	{
		//dbase_errno = de_NOT_DBASE_FILE;
		return 0;
	}
	int err;
	char szMode[3] = {0};
	switch(mode)
	{
	case DB_RD:	
		strcpy(szMode, "r");
		break;
	case DB_WR:
		strcpy(szMode, "r+");
		break;
	default:
		//dbase_errno = de_UNKNOWN_MODE;
		return 0;
	}
	dbfp = (struct DBFFILE*)malloc(sizeof(struct DBFFILE));
	if (0 == dbfp)
	{
		return 0;
	}
	memset(dbfp, 0, sizeof(struct DBFFILE));
	dbfp->fp = fopen(DBName, szMode);
	if (0 == dbfp->fp)
	{
		//err = errorno;
		//dbase_errno = de_ERROR_OPEN_FILE;
		goto FAIL1;
	}

	//CurRecNum = 0;
	if (32 != fread(buf, sizeof(char), 32, dbfp->fp))
	{
		goto FAIL0;
	}
	memcpy(dbfp->hi.LastUpdate, buf+OFDATE, 3);
	memcpy(&(dbfp->hi.RecNum), buf+OFRECNUM, 4 );
	memmove(&tmp, buf+OFRECLENGTH, 2);
	dbfp->hi.RecLength = (unsigned int)tmp;
	memcpy(&tmp, buf+OFRECOFFSET, 2);
	dbfp->hi.RecOffset = (unsigned int)tmp;
	dbfp->hi.FieldsNum = dbfp->hi.RecOffset/32 - 1;
	int nFileSize = GetFileSize(DBName);
////
	dbfp->hi.RecNum=(nFileSize-dbfp->hi.RecOffset)/dbfp->hi.RecLength;
////
	if (dbfp->hi.RecNum<0 || dbfp->hi.RecNum>100000000)
	{
		//dbase_errno = de_NOT_DBASE_FILE;;
		goto FAIL0;
	}

	if (dbfp->hi.RecOffset<=0 || dbfp->hi.RecOffset>5000)
	{
		//dbase_errno = de_NOT_DBASE_FILE;;
		//isValid = 0;
		goto FAIL0;
	}

	if (dbfp->hi.FieldsNum<0 || dbfp->hi.FieldsNum>100 )
	{
		//dbase_errno = de_NOT_DBASE_FILE;;
		//isValid = 0;
		goto FAIL0;
	}

	// get memory for fields list
	dbfp->hi.fl = (struct FieldsList*)malloc(sizeof(struct FieldsList)*dbfp->hi.FieldsNum);
	if (0 == dbfp->hi.fl)
	{
		//dbase_errno = de_MEMORY_NOT_ENOUGH;
		//isValid = 0;
		goto FAIL0;
	}
	int i = 0;
	int j=1;
	for(i=0; i<dbfp->hi.FieldsNum; i++ )
	{
		if (32 != fread(buf, sizeof(char), 32, dbfp->fp))
		{
			goto FAIL0;
		}
		/* add for compatable with VFP3.0 */
		if( buf[0]==0x0d )
		{
			dbfp->hi.FieldsNum = i;
			break;
		}
		
		memcpy(dbfp->hi.fl[i].name, buf + OFFLDNAME, 11 );
		dbfp->hi.fl[i].name[11] = 0;
		dbfp->hi.fl[i].type = buf[OFFLDTYPE];
		dbfp->hi.fl[i].width = buf[OFFLDWIDTH];
		dbfp->hi.fl[i].dec = buf[OFFLDDEC];
		memcpy( &tmp, buf + OFFLDOFFSET, 2 );
		dbfp->hi.fl[i].offset = j;//( unsigned int )tmp;
		j=j+dbfp->hi.fl[i].width;
	}
	fseek(dbfp->fp, dbfp->hi.RecOffset, SEEK_SET);
	dbfp->mode = mode;
	return dbfp;
FAIL0:
	close(dbfp->fp);
FAIL1:
	free(dbfp);
	return 0;
}

void CloseDBF(struct DBFFILE* dbfp)
{
	char buf[32];
	if (0 == dbfp)
	{
		return;
	}
	if(DB_WR == dbfp->mode)
	{
		goto RET;
	}
	/*	Update date	*/
	//TODO
	//CTime time = CTime::GetCurrentTime();
	//hi.LastUpdate[0] = ( char )time.GetYear()%100;
	//hi.LastUpdate[1] = ( char )time.GetMonth();
	//hi.LastUpdate[2] = ( char )time.GetDay();
	memset(buf, 0, 32);
	buf[0] = FLAGCHAR;
	memcpy(buf+OFDATE, dbfp->hi.LastUpdate, 3);
	memcpy(buf+OFRECNUM, &(dbfp->hi.RecNum), 4);
	memcpy(buf+OFRECLENGTH, &(dbfp->hi.RecLength), 2);
	memcpy(buf+OFRECOFFSET, &(dbfp->hi.RecOffset), 2);
	if (0 != fseek(dbfp->fp, 0, SEEK_SET))
	{
		goto RET;
	}
	fwrite(buf, sizeof(char), 32, dbfp->fp);

	if (0 != fseek(dbfp->fp, dbfp->hi.RecOffset+dbfp->hi.RecNum*dbfp->hi.RecLength, SEEK_SET))
	{
		goto RET;
	}
	buf[0] = ENDCHAR;
	fwrite(buf, sizeof(char), 1, dbfp->fp);
RET:
	fclose(dbfp->fp);
	free(dbfp);
}

int GetMaxLenOfCol(struct DBFFILE* dbfp)
{
	int len = 0;
	int i = 0;
	for (i = 0; i < dbfp->hi.FieldsNum; i++)
	{
		int tmp = strlen(dbfp->hi.fl[i].name);
		len += (dbfp->hi.fl[i].width > tmp ? dbfp->hi.fl[i].width : tmp)+1;
	}
	return len;
}

int GetRecord(int RecIndex, char *rec, struct DBFFILE* dbfp)
{
	if (RecIndex > dbfp->hi.RecNum )
	{
		//dbase_errno = de_ERROR_RECPTR;
		return -1;
	}
	if (fseek(dbfp->fp, dbfp->hi.RecOffset + RecIndex*dbfp->hi.RecLength, SEEK_SET)<0)
	{
		//dbase_errno = de_ERROR_LSEEK_FILE;
		return -1;
	}
	int nbytes;
	nbytes = fread(rec, sizeof(char), dbfp->hi.RecLength, dbfp->fp);
	if (nbytes != (int)dbfp->hi.RecLength)
	{
		//dbase_errno = de_ERROR_READ_FILE;
		return -1;
	}
	return 1;
}

int GetRecords(int RecFrom, char *rec, int RecNum, struct DBFFILE* dbfp)
{
	int nbytes, NumOfRecToRead;

	if(RecFrom > dbfp->hi.RecNum)
	{
		//dbase_errno = de_ERROR_RECPTR;
		return -1;
	}
	if (fseek(dbfp->fp, dbfp->hi.RecOffset+RecFrom*dbfp->hi.RecLength, SEEK_SET)<0)
	{
		//dbase_errno = de_ERROR_LSEEK_FILE;
		return -1;
	}
	NumOfRecToRead = dbfp->hi.RecNum-RecFrom>=RecNum ? RecNum : dbfp->hi.RecNum - RecFrom;
	nbytes = fread(rec, sizeof(char), NumOfRecToRead*dbfp->hi.RecLength, dbfp->fp);
	return((int)nbytes/dbfp->hi.RecLength);
}

int	AppendRecord(char *rec, struct DBFFILE* dbfp)
{
	if (dbfp->mode != DB_WR)
	{
		//dbase_errno = de_UNKNOWN_MODE;
		return -1;
	}
	if (fseek(dbfp->fp, dbfp->hi.RecOffset + dbfp->hi.RecNum*dbfp->hi.RecLength, SEEK_SET)<0)
	{
		//dbase_errno = de_ERROR_LSEEK_FILE;
		return -1;
	} 
	dbfp->hi.RecNum++;
	fwrite(rec, sizeof(char), dbfp->hi.RecLength, dbfp->fp);

	// update record number
	fseek(dbfp->fp, OFRECNUM, SEEK_SET);
	fwrite(&(dbfp->hi.RecNum), sizeof(char), 4, dbfp->fp);

	return 1;
};

int	AppendRecords(char *rec, int RecNum, struct DBFFILE* dbfp)
{
	if (dbfp->mode != DB_WR)
	{
		//dbase_errno = de_UNKNOWN_MODE;
		return -1;
	}

	if (fseek(dbfp->fp, dbfp->hi.RecOffset+dbfp->hi.RecNum*dbfp->hi.RecLength, SEEK_SET)<0)
	{
		//dbase_errno = de_ERROR_LSEEK_FILE;
		return -1;
	}

	dbfp->hi.RecNum +=RecNum;
	fwrite(rec, sizeof(char), dbfp->hi.RecLength*RecNum, dbfp->fp);

	fseek(dbfp->fp, OFRECNUM, SEEK_SET);
	fwrite(&(dbfp->hi.RecNum), sizeof(char), 4, dbfp->fp);
	return RecNum;
};

int	SaveRecords(int RecFrom, char *rec, int RecNum, struct DBFFILE* dbfp)
{
	if (dbfp->mode != DB_WR)
	{
		//dbase_errno = de_UNKNOWN_MODE;
		return -1;
	}


	if( fseek(dbfp->fp, dbfp->hi.RecOffset+RecFrom*dbfp->hi.RecLength, SEEK_SET)<0)
	{
		//dbase_errno = de_ERROR_LSEEK_FILE;
		return -1;
	}

	fwrite(rec, sizeof(char), dbfp->hi.RecLength*RecNum, dbfp->fp);

	if (dbfp->hi.RecNum<RecFrom+RecNum)
	{
		dbfp->hi.RecNum = RecFrom + RecNum;
		fseek(dbfp->fp, OFRECNUM, SEEK_SET);
		fwrite(&(dbfp->hi.RecNum), sizeof(char),4, dbfp->fp);
	}

	return RecNum;
};

int GetFieldIndex(const char *szFieldName, struct DBFFILE* dbfp)
{
	char name[16];
	int i;
	strcpy(name, szFieldName);
/*
win32
	if (0 == strupr(name))
	{
		return -1;
	}
*/

	for( i=0; i<dbfp->hi.FieldsNum; i++ )
	{
		if(strcmp(name, dbfp->hi.fl[i].name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int GetFieldData(int FieldIndex, char *field, char *rec, struct DBFFILE* dbfp)
{
	if (FieldIndex<0 || FieldIndex>=dbfp->hi.FieldsNum)
	{
		return -1;
	}

	int width = GetFieldWidth(FieldIndex, dbfp);
	memcpy(field, rec+dbfp->hi.fl[FieldIndex].offset, width);
	field[width] = 0;
	return width;
}

int GetFieldWidth(int FieldIndex, struct DBFFILE* dbfp)
{
	if (FieldIndex<0||FieldIndex>=dbfp->hi.FieldsNum)
	{
		return -1;
	}

	return (int)(dbfp->hi.fl[FieldIndex].width);
}

int DBFStrucCompare(char *file1, char *file2 )
{
	struct DBFFILE *dbfp1, *dbfp2;
	dbfp1 = OpenDBF(file1, DB_RD);
	dbfp2 = OpenDBF(file2, DB_RD);
	if ((0 == dbfp1)||(0 == dbfp2))
	{
		return -1;
	}

	if(dbfp1->hi.FieldsNum != dbfp2->hi.FieldsNum)
	{
		return 0;
	}

	int ret;
	struct FieldsList *fl1 = dbfp1->hi.fl;
	struct FieldsList *fl2 = dbfp2->hi.fl;
	int i;
	for (i=0; i<dbfp1->hi.FieldsNum; i++)
	{
		ret = i;
		if (0 != strcmp(fl1[i].name, fl2[i].name))
		{
			break;
		}

		if( fl1[i].type!=fl2[i].type )
		{
			break;
		}
		if( fl1[i].width!=fl2[i].width )
		{
			break;
		}
		if( fl1[i].dec!=fl2[i].dec )
		{
			break;
		}
	}
	CloseDBF(dbfp1);
	CloseDBF(dbfp2);
	return ret;
}

int GetValue(long row, long col, char* value, int* len, struct DBFFILE* dbfp)
{
	int width;
	char *RecBuf;
	RecBuf=(char *)malloc(dbfp->hi.RecLength+1);
	if (0 == RecBuf)
	{
		return -2;
	}
	if (GetRecord(row, RecBuf, dbfp) < 0)
	{
		free(RecBuf);
		return -3;
	}
	width = GetFieldData(col, value, RecBuf, dbfp);
	if (width < 0)  
	{
		free(RecBuf);
		return -4;
	}
	*(value+width) = 0;
	free(RecBuf);
	return 0;
}

struct FieldsList* GetFieldInfo(int nIndex, struct DBFFILE* dbfp)
{
	if (nIndex >= dbfp->hi.FieldsNum)
	{
		return 0;
	}
	return dbfp->hi.fl+nIndex;
}

int SetValue(long row, long col, char* newVal, struct DBFFILE* dbfp)
{
	struct FieldsList* colinfo;
	if (col > dbfp->hi.FieldsNum)
	{
		return -1;
	}
	colinfo = (struct FieldsList*)(dbfp->hi.fl+col);
	char *RecBuf;
	RecBuf=(char *)malloc(dbfp->hi.RecLength+1);
	if (0 == RecBuf)
	{
		return -3;
	}
	if (GetRecord((int)(int)(int)(int)(int)(int)(int)(int)(int)row, RecBuf, dbfp) < 0)
	{
		free(RecBuf);
		return -4;
	}
	memcpy(RecBuf+colinfo->offset, newVal, colinfo->width);
	if (SaveRecords(row, colinfo->name, 1, dbfp) < 0)
	{
		free(RecBuf);
		return -3;
	}
	free(RecBuf);
	return 0;
}

/*
int GetValue(long RecIndex, const char* FieldName, char* value, int len, struct DBFFILE* dbfp)
{
	int col = GetFieldIndex(FieldName, dbfp);
	if (col < 0)
	{
		return -6;
	}
	return GetValue(RecIndex, col, value, len, dbfp);
}

int SetValue(long RecIndex, const char* FieldName, char* newvalue)
{
	int col = GetFieldIndex(FieldName, dbfp);
	if (col < 0)
	{
		return -6;
	}
	return SetValue(RecIndex, col, value, len, dbfp);
}
*/
int LockRecord(int nRecord, struct DBFFILE* dbfp)
{
	int fd = fileno(dbfp->fp);
#ifdef WIN32
	HANDLE h = _get_osfhandle(fd);
	if (INVALID_HANDLE_VALUE == h)
	{
		return -1;
	}
	return (::LockFile(h, dbfp->hi.RecOffset+nRecord*dbfp->hi.RecLength, 0, dbfp->hi.RecLength, 0)?0:-1);
#endif
}
int UnlockRecord(int nRecord, struct DBFFILE* dbfp)
{
	int fd = fileno(dbfp->fp);
#ifdef WIN32
	HANDLE h = _get_osfhandle(fd);
	if (INVALID_HANDLE_VALUE == h)
	{
		return -1;
	}
	return (::UnlockFile(h, dbfp->hi.RecOffset+nRecord*dbfp->hi.RecLength, 0, dbfp->hi.RecLength, 0)?0:-1);
#endif
}
