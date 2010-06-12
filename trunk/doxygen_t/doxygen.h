/**
 * @file	doxygen.h 
 * @brief	test doxygen comment
 * @details	
 * @author	zuohaitao
 * @date	2010-06-12 
 * @warning	
 * @bug	
 */
#ifdef DOXYGEN_H
//! myint <-> int
typedef int myint
//! _stu_st struct  
typedef struct _my_st
{
	//! length
	int a;
	//! point
	char* b;
	//! float var
	float c;
} my_st,*pmy_st;

/**
 * @name	func
 * @brief	function comment for doxygen
 * @param	a [I/ ] int - length
 * @param	b [I/ ] char* - point
 * @param	c [ /O] float - float numeric
 * @return	int - 0 if success; other if fail
 */
int func(int a, char* b, float c);
#endif //DOXYGEN_H
