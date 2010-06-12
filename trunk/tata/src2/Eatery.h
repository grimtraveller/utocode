/*
 * Eatery class declaration
 */
#ifndef EATERY_H
#define EATERY_H
#include <string>
#include <list>
using namespace std;
#include "Meal.h"
#define MEAL_SIZE	5
class Eatery
{
public:
	enum TYPE
	{
		DEL = '*',
		RESTAURANT = 'R',
		SNACKCART =  'S',
	};
	Eatery();
	~Eatery();
	TYPE type;
	//order meal by meal index
    float orderMeal(int index,int count);
	//order meal by meal name
	float orderMeal(string mealname,int count);
	//get the information of eatery from file or screen
	virtual bool get(istream&get,bool file=false)/* = 0*/;
	//put the information of eatery to screen or file
	virtual void put(ostream&put,bool file=false)/* = 0*/;
	//put cheapest food list 
	//void putCheapestMeal(ostream&put,bool file=false);
	//put best food list
	//void putBestMal(ostream&put,bool file=false);
protected:
	Meal cheapestMeal();
	Meal bestMeal();

	int mealcount;
	int number;
	list<Meal> meals;
	//Meal meal[MEAL_SIZE];
	string ownername;
private:

	//order meal by list iterator
	float orderMeal(list<Meal>::iterator meal, int count);
};
#endif	//EATERY_H
