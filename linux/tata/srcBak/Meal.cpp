#include "Meal.h"

Meal::Meal()
{
	cost=0;
	number=0;
}

float Meal::orderMeal(int count)
{
	return count*cost;
}

void Meal::putMeal(ostream&put,bool file)
{
	if ( file)
	{
		put<<name<<" "<<cost<<" ";
	}
	else put<<"Meal No."<<number<<" Name:"<<name<<" Cost:$"<<cost<<endl;
}


bool Meal::getMeal(istream&get,bool file)
{
	if ( file )
	{
		if(!(get>>name)) return false;
        if(!(get>>cost)) return false;

		return true;
	}

	cout<<"Input the name of meal :";
	if(!(get>>name)) return false;
	cout<<"Input the cost of meal:";
    if(!(get>>cost)) return false;

	return true;
}

bool operator < (Meal& meal, Meal& meal1)
{
	return meal.cost < meal1.cost;
}

