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
//������	�Ƿ�ΪDBF�ļ�
//������	const char* FileName �ļ���
//���أ�	<0ʧ�ܣ�����ɹ�
extern int IsDBF(const char* FileName);
//��һ��DBF�ļ�
//const char* DBName �ļ���
//int mode �򿪷�ʽ DB_RD ֻ�� DB_WR ��д
//���� �����ʧ�ܷ���0 ���򷵻سɹ�
extern struct DBFFILE* OpenDBF(const char* DBName, int mode);
//�ر�DBF�ļ�
extern void CloseDBF(struct DBFFILE* pdbf);
//�õ�ÿ�е����ĳ���
//DBFFILE* pdbf DBF�ļ�ָ��
//���� ����
extern int GetMaxLenOfCol(struct DBFFILE* pdbf);
//�õ�һ����¼
//RecIndex ��¼���
//char* rec ��¼����ָ��
//DBFFILE* pdbf DBF�ļ�
//<0����1�ɹ�
extern int GetRecord(int RecIndex, char* rec, struct DBFFILE* pdbf);
//�õ�RecFrom������м�¼
//char* rec ��¼����ָ��
//DBFFILE* pdbf DBF�ļ�
//<0�������򷵻ؼ�¼����
extern int GetRecords(int RecFrom, char* rec, int RecNum, struct DBFFILE* pdbf);
//����һ����¼
//char* rec ��¼����
//DBFFILE* pdbf DBF�ļ�
//<0����1�ɹ�
extern int AppendRecord(char* rec, struct DBFFILE* pdbf);
//����RecNum����¼
//char* rec ��¼����
//int RecNum ���ӵļ�¼��
//DBFFILE* pdbf DBF�ļ�
//���ؼ�¼���Ӽ�¼�������С��0�����
extern int AppendRecords(char* rec, int RecNum, struct DBFFILE* pdbf);
//����RecFrom ��ʼ��RecNum����¼
//int RecFrom ��¼��ʼ��ʼ
//char* rec	��¼����
//int RecNum��¼��Ŀ
//DBFFILE* pdbf DBF�ļ�
//���� ����ļ�¼�� ���С��0�����
extern int SaveRecords(int RecFrom, char* rec, int RecNum, struct DBFFILE* pdbf);
//�õ������
//const char* szFileName ����
//struct DBFFILE* pdbf DBF�ļ�ָ��
//���� ������� ���С��0�����
extern int GetFieldIndex(const char* szFileName, struct DBFFILE* pdbf);
//�õ��������
//int FieldIndex ������
//char* field[out]������ֵ
//char* rec ��¼ָ��
//DBFFILE* pdbf DBF�ļ�
//������ֵ�ĳ��� ���С��0��ʧ��
extern int GetFieldData(int FieldIndex, char* field, char* rec, struct DBFFILE* pdbf);
//�õ���ĳ���
//int FieldIndex ������
//struct DBFFILE* pdbf �õ��򳤶�
//���� ��ĳ��� ���С��0��ʧ��
extern int GetFieldWidth(int FieldIndex, struct DBFFILE* pdbf);
//���ļ�
extern int LockRecord(int nRecord, struct DBFFILE* pdbf);
//�����ļ�
extern int UnLockRecord(int nRecord, struct DBFFILE* pdbf);
//�Ƚ�DBF�ļ��Ľṹ
//����0��һ�� ���򷵻�����Ĳ�һ�µļ���
int DBFStrucCompare(char *file1, char* file2);
//�õ�ĳ��ֵ
//long row��¼���
//long col�����
//char* value[out]ֵ
//int* len [in][out]����
//struct DBFFILE* dbfp DBF�ļ�
//���� ���ʧ����С��0 
int GetValue(long row, long col, char* value, int* len, struct DBFFILE* dbfp);
//�õ������Ϣ
//int nIndex ������
//DBFFILE* dbfp DBF�ļ�
//���� ���Ϊ����ʧ�� ����Ϊ����Ϣָ��
struct FieldsList* GetFieldInfo(int nIndex, struct DBFFILE* dbfp);
//����ĳ��ֵ
//long row��¼���
//long col�����
//char* value[out]ֵ
//struct DBFFILE* dbfp DBF�ļ�
//���� ���ʧ����С��0 
int SetValue(long row, long col, char* newVal, struct DBFFILE* dbfp);
//DEL***************************
//�õ�ĳ��ֵ
//long RecIndex��¼���
//const char* FieldName����
//char* value[out]ֵ
//int* len [in][out]����
//struct DBFFILE* dbfp DBF�ļ�
//���� ���ʧ����С��0 
//int GetValue(long RecIndex, const char* FieldName, char* value, int len, struct DBFFILE* dbfp);
//long row��¼���
//const char* FieldName����
//char* value[out]ֵ
//struct DBFFILE* dbfp DBF�ļ�
//���� ���ʧ����С��0 
//int SetValue(long RecIndex, const char* FieldName, char* newvalue, struct DBFFILE* dbfp);
//DEL END***********************
//δʵ��
//ɾ����¼:������ʱ�ļ����ѷ�ɾ���ļ�¼д����ʱ�ļ��У��޸���ʱ�ļ���HeadInfo ɾ��ԭ�ļ� �޸���ʱ�ļ�Ϊ�ļ�
//int DelRecord(long RecIndex, struct DBFFILE* dbfp);
//�õ���¼ɾ����־λ��־ΪΪ"*"���ʾɾ��
int GetRecFlag(long RecIndex, char* Flag, struct DBFFILE* dbfp);
int SetRecFlag(long RecIndex, char DelFlag, struct DBFFILE* dbfp);
#endif
