/**
  @file		list.h	
  @brief	declaration list functions
  @details	
  @author	zuohaitao
  @date		2009-03-02
  @warning	
  @bug		
  */
#ifndef LIST_H
#define LIST_H
#include "public.h"
typedef int Element;			//!< element in list

//! node in list
typedef struct ListItem
{
	Element e;					//!< element
	struct ListItem* next;		//!< point to next
} LISTITEM;

//! list header
typedef struct ListHead
{
	struct ListItem* next;	//!< point to next
} LISTHEAD;

//! list
typedef struct ListHead LIST;

//callback function about travelList
typedef void (*VISITFUN)(LISTITEM* item);
/**
  @name		InitList()
  @brief	initial list 
  @param	LIST* L[ /O] List 
  @return	void
  */
extern void InitList(LISTHEAD* L);

/**
  @name		DestroyList()
  @brief	Destory List 
  @param	LIST* L [I/ ] List 
  @return	void
  */
extern void DestroyList(LISTHEAD* L);

//void ClearList(LISTHEAD* L);

//BOOLEAN IsListEmpty(LISTHEAD* L);

//GetListPriorListElem(L, cur_e, &pre_e);

//GetListNextListElem(L, cur_e, &pre_e);
/**
  @name		NextListElem
  @brief	get next element in list
  @param	LIST* L [I/ ] List
  @param	LISTITEM* current item in list[I/O] 
  @param	LISTHEAD* L [ /O] 
  @return	BOOLEAN TRUE = success or FALSE = fail
  */
//extern BOOLEAN NextListElem(LIST* L, LISTITEM* cur_item, LISTITEM* next_item);

/**
  @name		LocateList	
  @brief	find element in List
  @param	LIST* L [I/ ] List
  @param	const Element e [I/ ] element
  @return	BOOLEAN TRUE = e in list or FALSE = e is NOT in list
  */
extern BOOLEAN LocateList(LISTHEAD* L, const Element e);

/**
  @name		GetListLength()
  @brief	get list length
  @param	LIST* L [I/ ] list 
  @return	int - the length of list
  */
extern int GetListLength(LIST* L);

/**
  @name		TravelList()
  @brief	travel all items in list to run visit() function
  @param	LIST* L [I/ ] list
  @param	VISITFUN visit [I/ ] callback function point
  @return	void
  */
extern void TravelList(LIST* L, VISITFUN visit);

/**
  @name		PutListItem()
  @brief	put item in list
  @param	LIST* L [I/ ] list 
  @param	int pos [I/ ] position
  @param	LISTITEM* [I/ ] a item of list point
  @return	void
  */
extern void PutListItem(LIST* L, int pos, LISTITEM* item);

/**
  @name		GetListElem()
  @brief	get a element in list
  @param	LIST* L [I/ ]
  @param	int pos [I/ ]
  @param	Element* e [ /O]
  @return	BOOLEAN TRUE = Success or FALSE 
  */
extern BOOLEAN GetListElem(LIST* L, int pos, Element* e);

/**
  @name		DelListeItem()
  @brief	delete item in list
  @param	LIST* L [I/ ] list 
  @param	int pos [I/ ] position 
  @return	
  */
extern void DelListItem(LIST* L, int pos);

//InsertListElem()
//DeleteListElem()
#endif
