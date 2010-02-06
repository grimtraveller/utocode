#ifndef DBF_H
#define DBF_H
#include <stdio.h>
#define	FLAGCHAR	0x03
#define ENDCHAR		0x1a
#define	OFDATE	 	1
#define	OFRECNUM	4
#define	OFRECNUM	4
#define	OFRECLENGTH	10 
#define	OFRECOFFSET	8
#define	OFFLDNAME	0
#define	OFFLDTYPE	11
#define	OFFLDWIDTH	16
#define	OFFLDDEC	17
#define	OFFLDOFFSET	12

#define DB_RD		0x0001 	// read only
#define DB_WR		0x0002	// read & write

struct FieldsList
{
	char name[11];
	char type;
	unsigned char width;
	char dec;
	unsigned int offset;		// the offset in the record
};

struct HeadInfo
{
	char LastUpdate[3]; 		// [0] --- year, [1] --- month, [2] --- day
	int FieldsNum;
	long RecNum;			// totle record number
	unsigned int RecOffset;
	unsigned int RecLength;
	struct FieldsList *fl;
};

struct DBFFILE
{
	FILE* fp;
	int mode;
	struct HeadInfo hi;
};
extern int CreateDBF(const char* DBName, struct HeadInfo* hi);
//描述：	是否为DBF文件
//参数：	const char* FileName 文件名
//返回：	<0失败，否则成功
extern int IsDBF(const char* FileName);
//打开一个DBF文件
//const char* DBName 文件名
//int mode 打开方式 DB_RD 只读 DB_WR 读写
//返回 如果打开失败返回0 否则返回成功
extern struct DBFFILE* OpenDBF(const char* DBName, int mode);
//关闭DBF文件
extern void CloseDBF(struct DBFFILE* pdbf);
//得到每列的最长项的长度
//DBFFILE* pdbf DBF文件指针
//返回 长度
extern int GetMaxLenOfCol(struct DBFFILE* pdbf);
//得到一条记录
//RecIndex 记录序号
//char* rec 记录数据指针
//DBFFILE* pdbf DBF文件
//<0出错，1成功
extern int GetRecord(int RecIndex, char* rec, struct DBFFILE* pdbf);
//得到RecFrom后的所有记录
//char* rec 记录数据指针
//DBFFILE* pdbf DBF文件
//<0出错，否则返回记录条数
extern int GetRecords(int RecFrom, char* rec, int RecNum, struct DBFFILE* pdbf);
//增加一条记录
//char* rec 记录数据
//DBFFILE* pdbf DBF文件
//<0出错，1成功
extern int AppendRecord(char* rec, struct DBFFILE* pdbf);
//增加RecNum条记录
//char* rec 记录数据
//int RecNum 增加的记录数
//DBFFILE* pdbf DBF文件
//返回记录增加记录数，如果小于0则出错
extern int AppendRecords(char* rec, int RecNum, struct DBFFILE* pdbf);
//保存RecFrom 开始的RecNum条记录
//int RecFrom 记录起始开始
//char* rec	记录数据
//int RecNum记录数目
//DBFFILE* pdbf DBF文件
//返回 保存的记录数 如果小于0则出错
extern int SaveRecords(int RecFrom, char* rec, int RecNum, struct DBFFILE* pdbf);
//得到键序号
//const char* szFileName 键名
//struct DBFFILE* pdbf DBF文件指针
//返回 键的序号 如果小于0则出错
extern int GetFieldIndex(const char* szFileName, struct DBFFILE* pdbf);
//得到域的数据
//int FieldIndex 域的序号
//char* field[out]返回域值
//char* rec 记录指针
//DBFFILE* pdbf DBF文件
//返回域值的长度 如果小于0则失败
extern int GetFieldData(int FieldIndex, char* field, char* rec, struct DBFFILE* pdbf);
//得到域的长度
//int FieldIndex 域的序号
//struct DBFFILE* pdbf 得到域长度
//返回 域的长度 如果小于0则失败
extern int GetFieldWidth(int FieldIndex, struct DBFFILE* pdbf);
//锁文件
extern int LockRecord(int nRecord, struct DBFFILE* pdbf);
//解锁文件
extern int UnLockRecord(int nRecord, struct DBFFILE* pdbf);
//比较DBF文件的结构
//返回0则一致 否则返回最初的不一致的键号
int DBFStrucCompare(char *file1, char* file2);
//得到某项值
//long row记录序号
//long col域序号
//char* value[out]值
//int* len [in][out]长度
//struct DBFFILE* dbfp DBF文件
//返回 如果失败则小于0 
int GetValue(long row, long col, char* value, int* len, struct DBFFILE* dbfp);
//得到域的信息
//int nIndex 域的序号
//DBFFILE* dbfp DBF文件
//返回 如果为空则失败 否则为域信息指针
struct FieldsList* GetFieldInfo(int nIndex, struct DBFFILE* dbfp);
//设置某项值
//long row记录序号
//long col域序号
//char* value[out]值
//struct DBFFILE* dbfp DBF文件
//返回 如果失败则小于0 
int SetValue(long row, long col, char* newVal, struct DBFFILE* dbfp);
//DEL***************************
//得到某项值
//long RecIndex记录序号
//const char* FieldName域名
//char* value[out]值
//int* len [in][out]长度
//struct DBFFILE* dbfp DBF文件
//返回 如果失败则小于0 
//int GetValue(long RecIndex, const char* FieldName, char* value, int len, struct DBFFILE* dbfp);
//long row记录序号
//const char* FieldName域名
//char* value[out]值
//struct DBFFILE* dbfp DBF文件
//返回 如果失败则小于0 
//int SetValue(long RecIndex, const char* FieldName, char* newvalue, struct DBFFILE* dbfp);
//DEL END***********************
//未实现
//删除记录:建立临时文件，把非删除的记录写入临时文件中，修改临时文件的HeadInfo 删除原文件 修改临时文件为文件
//int DelRecord(long RecIndex, struct DBFFILE* dbfp);
//得到记录删除标志位标志为为"*"则表示删除
int GetRecFlag(long RecIndex, char* Flag, struct DBFFILE* dbfp);
int SetRecFlag(long RecIndex, char DelFlag, struct DBFFILE* dbfp);
#endif
