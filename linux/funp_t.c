/*
 * Name:		funp_t.c
 * Purpose:		1 function point
 *				2 a function that returns a function point
 *				3 force convert from a pointer to function point
 * Usage:		
 *				gcc src/funp_t.c -o bin/funp_t;bin/funp_t;echo $?
 * Author:		zuohaitao
 * Date:		2009-06-28
 */
//FUNC is as a function style
typedef int FUNC(int);
//PFUNC is as a function point style
typedef int (*PFUNC)(int);
//PF1 is a function point style and the function returns a function point
typedef void (*(*PF1)());
//declare functions
int funci(int i);
void (*f1());
char* f2();
int main()
{
	char* pc;
	FUNC* func;
	PFUNC pfunc = funci;
	PF1 pf = f1;
	/* 1 */
	//func = pf();
	//return func(5);
	/* 2 */
	//return ((FUNC*)(pf()))(5);
	/* 3 */
	//return ((int(*)(int))pf())(5);
	/* 4 */
	pc = (char*)f2;
	//pf = (void(*(*)()))pc;
	//func = pf();
	//return func(5);
	/* 6 */
	pc = (char*)f2;
	return ((int(*)(int))(((void(*(*)()))pc)()))(5);
	//func = (int(*)(int))(PF1)pc;
	//return func();//(int(*)(int)pc())(5);
}
//return a function point
void (*f1())
{
	FUNC* pi;
	pi = funci;
	return pi;
}
char* f2()
{
	FUNC* pi;
	pi = funci;
	return (char*)pi;
}
// a common function
int funci(int i)
{
	return i;
}

