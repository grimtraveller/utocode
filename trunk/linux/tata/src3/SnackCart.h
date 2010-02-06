#ifndef SNACKCART_H
#define SNACKCART_H

#include "Meal.h"      
#include "Eatery.h"
class SnackCart : public Eatery  
{
public:
	SnackCart();
	//order meal by meal index
    float orderMeal(int index,int count);
	//order meal by meal name
	float orderMeal(string mealname,int count);
	//get cart from file or screen
	bool get(istream&get,bool file=false);
	//put cart to screen or file
	void put(ostream&put,bool file=false);
public:
	string location;
};


#endif 
