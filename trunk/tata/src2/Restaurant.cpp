#include "Restaurant.h"
#include "Meal.h"
Restaurant::Restaurant()
{
	name = "";
	address = "";
	telephone = "";
	openingHour = 0;
}
Restaurant::~Restaurant()
{
}
bool Restaurant::get(istream&get,bool file)
{
	if (file)
	{
		if (!(get>>ownername>>name>>address>>telephone>>openingHour>>mealcount))
		{
			return false;
		}
	
		for ( int i = 0; i < mealcount;i++)
		{
			Meal meal;
			if (!meal.getMeal(get,file))
			{
				return false;
			}
			meal.number=i+1;
			meals.push_back(meal);
		}
		return true;
	}

	cout<<"Input the owner name of the restaurant:";
	if(!(get>>ownername))
	{
		return false;
	}

	cout<<"Input the name of the restaurant:";
	if(!(get>>ownername))
	{
		return false;
	}

	cout<<"Input the address of the restaurant:";
	if(!(get>>address))
	{
		return false;
	}

	cout<<"Input the telephone of the restaurant:";
	if(!(get>>telephone))
	{
		return false;
	}

	cout<<"Input the open time of the restaurant:";
	if(!(get>>telephone))
	{
		return false;
	}

	cout<<"Input the meal count of the snack cart:";
	if(!(get>>mealcount))
	{
		return false;
	}

	for ( int i = 0 ; i<mealcount;i++)
	{
		Meal meal;
		if (!meal.getMeal(get,file))
		{
			return false;
		}
		meal.number=i+1;
		meals.push_back(meal);
	}

	return true;
}

void Restaurant::put(ostream& put,bool file)
{
	if (file)
	{
		put<<ownername<<" "<<name<<" "<<address<<" "<<telephone<<" "<<openingHour<<" "<<mealcount<<" ";
		
		for ( int i=0;i<mealcount;i++)
		{
			Meal meal;
			meal.putMeal(put,file);
			meals.push_back(meal);
		}
		
		put<<endl;
		return;
	}
	put<<"Restaurant No."<<number<<" Owner name:"<<ownername
		<<" Name:"<<name<<" address:"<<address
		<<" Telephone:"<<telephone<<"Open time"
		<<openingHour<<" Meal count:"<<mealcount<<endl;
	
	for (int i=0; i < mealcount; i++)
	{
		Meal meal;
		meal.putMeal(put,file);
		meals.push_back(meal);
	}
	//putCheapestMeal(put,file);
	//putBestMal(put,file);
	put<<endl;
}
