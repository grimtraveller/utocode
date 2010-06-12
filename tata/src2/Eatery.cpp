#include "Eatery.h"
#include "Meal.h"
#include <list>
using namespace std;
Eatery::Eatery()
{
	mealcount = 0;
	number = 0;
}

Eatery::~Eatery()
{
	
}

bool Eatery::get(istream&get,bool file)
{
}
void Eatery::put(ostream&put,bool file)
{
	;
}
inline float Eatery::orderMeal(list<Meal>::iterator mealIter, int count)
{
	((Meal)(*mealIter)).rate += count;
	return (*mealIter).orderMeal(count);
}

float Eatery::orderMeal(int index, int count)
{
	list<Meal>::iterator mealIter = meals.begin();
	for (int i = 0; i < index; i++)
	{
		mealIter++;
	}
	return orderMeal(mealIter, count);
}

float Eatery::orderMeal(string mealname, int count)
{
	list <Meal>::iterator mealIter;

	for (mealIter = meals.begin(); mealIter != meals.end(); mealIter++)
	{
		if (mealname == (*mealIter).name)
		{
			//Success!find it!
			return orderMeal(mealIter, count);	
		}
	}
	//Fail!
	return 0;
}

Meal Eatery::bestMeal()
{
	Meal bestMeal;
	list<Meal>::iterator mealIter;

	for (mealIter = meals.begin(); mealIter != meals.end(); mealIter++)
	{
		if (bestMeal.rate < (*mealIter).rate)
		{
			bestMeal = (*mealIter);
		}
	}
	return bestMeal;
}

Meal Eatery::cheapestMeal() 
{
	Meal cheapestMeal;
	list<Meal>::iterator mealIter;

	for (mealIter = meals.begin(); mealIter != meals.end(); mealIter++)
	{
		if (cheapestMeal < (*mealIter))
		{
			cheapestMeal = (*mealIter);
		}
	}
	return cheapestMeal;
}

