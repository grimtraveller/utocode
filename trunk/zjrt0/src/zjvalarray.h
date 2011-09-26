/**
 @file		zjvalarray.h
 @brief		squence list data structure
 @details	
 @author	zuohaitao
 @date		2011-09-26
 warning	
 bug		
**/

#ifndef ZJVALARRAY_H
#define ZJVALARRAY_H
#include "zjtype.h"

typedef struct zjValArray_ST
{
	zjbyte* _data;				//! data point
	unsigned int _element_size;	//! element size
	unsigned int _used_count;	//! used element count
	unsigned int _allocated_count;	//! total emelent count
}zjValArray;

zjValArray* zjValArrayConstruct(unsigned int element_size, unsigned int allocated_count);

void zjValArrayDestruct(zjValArray* array);

void zjValArrayClear(zjValArray* array);

ZJBOOL zjValArrayIsEmpty(zjValArray* array);

ZJBOOL zjValArrayAdd(zjValArray* array, void* data);

ZJBOOL zjValArrayRemove(zjValArray* array, unsigned int idx);

unsigned int zjValArrayGetCount(zjValArray* array);

void* zjValArrayGetData(zjValArray* array);

void* zjValArrayGetAt(zjValArray* array, unsigned int idx);

#endif //ZJVALARRAY_H
