#include "zjvalarray.h"
#include "zjmem.h"

zjValArray* zjValArrayConstruct(unsigned int element_size, unsigned int allocated_count)
{
	zjValArray* array = (zjValArray*)ZJMALLOC(sizeof(zjValArray));
	if (NULL == array)
	{
		return NULL;
	}
	array->_data = NULL;
	array->_element_size = element_size;
	array->_used_count = 0;
	array->_allocated_count = allocated_count;
	array->_data = ZJMALLOC(array->_element_size * array->_allocated_count);
	if (NULL == array->_data)
	{
		ZJFREE(array);
		return NULL;
	}
	return array;
}

void zjValArrayDestruct(zjValArray* array)
{
	//assert(array != NULL);
	if (NULL != array->_data)
	{
		ZJFREE(array->_data);
	}
	ZJFREE(array);
}

void zjValArrayClear(zjValArray* array)
{
	array->_used_count = 0;
}

ZJBOOL zjValArrayIsEmpty(zjValArray* array)
{
	return (ZJBOOL)(array->_used_count == 0);
}

ZJBOOL zjValArrayAdd(zjValArray* array, void* data)
{
	if( ++(array->_used_count) >= array->_allocated_count)
	{
		array->_allocated_count *= 2;
        if (0 == array->_allocated_count)
		{
			array->_allocated_count = 11;
		}
        array->_data = ZJREALLOC(array->_data, array->_allocated_count * array->_element_size);
		if (NULL == array->_data)
		{
			//
		}
	}
    if (NULL == array->_data)
	{
		return FALSE;
	}
	memcpy(array->_data+((array->_used_count - 1)*array->_element_size),
			data, 
			array->_element_size);
	//printf("%s\n", array->_data);
    return TRUE;
}

ZJBOOL zjValArrayRemove(zjValArray* array, unsigned int idx)
{
	if (idx >= array->_used_count)
	{
		return FALSE;
	}
	array->_used_count -= 1;
	memcpy(array->_data+(idx*array->_element_size), 
			array->_data+((idx+1)*array->_element_size),
			(array->_used_count-idx)*array->_element_size);
	return TRUE;
}

unsigned int zjValArrayGetCount(zjValArray* array)
{
	return array->_used_count;
}

void* zjValArrayGetData(zjValArray* array)
{
	return array->_data;
}

void* zjValArrayGetAt(zjValArray* array, unsigned int idx)
{
	if (idx >= array->_used_count)
	{
		return NULL;
	}
    return (array->_data+(idx*array->_element_size));
}
