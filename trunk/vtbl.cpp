/*
 * C++ Class Virtual Table
 *
 *
 *     a +------------+
 *       |  vt        +------>+----------+----------+
 *       +------------+       | AA()::f1  | AA()::f2|
 *       |  i         |       +----------+----------+
 *       +------------+      
 *                           
 * $>g++ vtbl.cpp -Wno-invalid-offsetof -o vtbl
 * $>./vtbl
 *
 */
#include <stdio.h>
class A
{
public:
	A() {
		i = 0;
	};
	virtual void f1() = 0;
	int i;
};

class AA : public A
{
public:
	virtual void f1() {
		printf("%s\n", __FUNCTION__);
	};
	virtual void f2() {
		printf("%s\n", __FUNCTION__);
	};
};

class AAA : public AA
{
public:
	void f1() {
		printf("%s\n", __FUNCTION__);
	};
};

typedef void (*PF)();

void f() {
	printf("%s\n", __FUNCTION__);
}

int
main(int argc, char* argv[])
{
	int a_len = sizeof(A);
	int aa_len = sizeof(AA);
	int ab_len = sizeof(AAA);

	int i_offset_a = ((size_t)(&(((A*)0)->i)));
	A* a = new AA;
	a->f1();
	((AA*)a)->f2();
	printf("sizeof(A)=%u\n", sizeof(A));
	printf("sizeof(AA)=%u\n", sizeof(AA));
	printf("sizeof(AAA)=%u\n", sizeof(AA));
	PF pf = f;
	pf();
	printf("a = %p\n", a);
	//virtual table pointer
    void* vtptr = (void*)a;
	//virtual table
    void* vt = (void*)(*(size_t*)vtptr);
	//virtual function1 pointer
	void* vf1 = (void*)(((size_t*)vt)[0]);
	printf("vf1=%p\n", vf1);
	//virtual function2 pointer
	void* vf2 = (void*)(((size_t*)vt)[1]);
	printf("vf2=%p\n", vf2);
	printf("a->vtptr = %p\n", (void*)(a));
	printf("a->1vf point = %p\n", (PF*)(*((size_t*)(a))));
	printf("a->2vf point = %p\n", (PF*)(*((size_t*)(a)))+1);
	printf("a->1vf = %p\n", *((PF*)(*((size_t*)(a)))));
	printf("a->1vf = %p\n", *((PF*)(((size_t*)(a))[0])));
	printf("a->2vf = %p\n", *(((PF*)(*((size_t*)(a))))+1));
	printf("a->2vf = %p\n", *(((PF*)(*((size_t*)(a))))[1]));
	printf("a->i = %d\n", (int)*(((char*)a) + i_offset_a)); 
	*(((char*)a) + i_offset_a) = 2;
	printf("a->i = %d\n", a->i); 
	pf = *((PF*)(*((size_t*)(a))));
	pf();
	pf = *((PF*)(((size_t*)(a)))[0]);
	pf();
	pf = *((PF*)(*((size_t*)(a)))+1);
	pf();
	pf = *(((PF*)(*((size_t*)(a))))[1]);
	pf();

	return 0;
}
