#include "Eatery.h"
Eatery::Eatery()
{
}

Eatery::~Eatery()
{
}

void Eatery::putCheapestMeal(ostream& put,bool file)
{
	if ( !file ) 
	{
		put<<"The Cheapest Meal:";
		cheapestMeal().putMeal(put,file);
	}
}

void Eatery::putBestMal(ostream& put,bool file)
{
	if ( !file ) 
	{
		put<<"The Best Meal:";
		bestMeal().putMeal(put,file);
	}
}

Meal Eatery::cheapestMeal() 
{
	Meal meal = meals[0];

	for ( int i = 1; i < mealcount; ++i)
		if (meals[i]<meal) meal = meals[i];
	return meal;
}
/*
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
*/
Meal Eatery::bestMeal()
{
	Meal meal = meals[0];
	int meali=0;

	for ( int i = 1; i < mealcount; ++i)
	{
		if (meals[i].rate<meal.rate)
		{
			meal = meals[i];
		}

	}

	return meal;
}
ostream& Eatery::operator << (ostream &output)
{
	output<<ownername<<" "<<openHours;
	for ( int i=0;i<mealcount;i++)
	{
		meals[i].putMeal(output, false);
	}
	output<<endl;
}
istream& Eatery::operator >> (istream &input)
{
	input>>ownername>>openHours;
		
	for ( int i = 0;i<mealcount;i++)
	{
		meals[i].getMeal(input, false);
		meals[i].number=i+1;
	}
}
