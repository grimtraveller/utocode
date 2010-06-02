#ifndef PFXDATETIME_H
#ifdef __cplusplus
extern "C"
{
#endif
/*
    bufsize > 11
	flag=0 YYYYMMDD
	flag=1 YYYY-MM-DD
 */
extern void pfxdate(char* buf, int flag);
#ifdef __cplusplus
}
#endif
#endif	//PFXDATETIME_H
