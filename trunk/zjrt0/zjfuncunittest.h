#ifndef ZJFUNCUNITTEST_H
#define ZJFUNCUNITTEST_H
/*
 *	
　	MS VC++ 9.0 _MSC_VER = 1500 VS2008
　　MS VC++ 8.0 _MSC_VER = 1400 VS2005
　　MS VC++ 7.1 _MSC_VER = 1310 
　　MS VC++ 7.0 _MSC_VER = 1300 
　　MS VC++ 6.0 _MSC_VER = 1200 VC6.0
　　MS VC++ 5.0 _MSC_VER = 1100
 */ 
#if defined(WIN32)
#	if (_MSC_VER >= 1400)
#		define FUNC_UNIT_TEST(rightreturnvalue, funcname, ...)	\
			if (rightreturnvalue == funcname(##__VA_ARGS__))	{	\
				printf("succeeds\n");	\
			}	\
			else {	\
				printf("fails\n");	\
			}
#	else
#	error("Do not support ##__VA_ARGS__ macro!")
#	endif //_MSC_VER
#elif defined(LINUX)
#	define FUNC_UNIT_TEST(rightreturnvalue, funcname, arg...)	\
		if (rightreturnvalue == funcname(##arg))	{	\
			printf("succeeds\n");	\
		}	\
		else {	\
			printf("fails\n");	\
		}
#endif //OS


#endif //ZJFUNCUNITTEST_H
