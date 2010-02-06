#ifndef SNACKCART
#define SNACKCART

#include "Meal.h"      

class SnackCart  
{
public:
	SnackCart();
	//order meal by meal index
    float orderMeal(int index,int count);
	//order meal by meal name
	float orderMeal(string mealname,int count);
	//get cart from file or screen
	bool getCart(istream&get,bool file=false);
	//put cart to screen or file
	void putCart(ostream&put,bool file=false);
	//put cheapest cart 
	void putCheapestMeal(ostream&put,bool file=false);
	//put best cart
	void putBestMal(ostream&put,bool file=false);
private:
	Meal cheapestMeal();
	Meal bestMeal();

public:
	int  mealcount;
	int  number;
	Meal meals[5];
	int  mealrates[5];
	string location;
	string ownername;
};


#endif 
