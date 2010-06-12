#include "Restaurant.h"
Restaurant::Restaurant()
{
	
}

Restaurant::~Restaurant()
{
}

bool Restaurant::get(istream&get,bool file)
{
	type = RESTAURANT;
	if ( file)
	{
		if ( !(get>>ownername>>openHours>>name>>address>>telephone>>mealcount) ) return false;
		
		for ( int i = 0;i<mealcount;i++)
		{
			if (!meals[i].getMeal(get,file)) return false;
			meals[i].number=i+1;
		}

		return true;
	}

	cout<<"Input the owner name of the snack cart:";
	if(!(get>>ownername)) return false;


	cout<<"Input the open hours of the snack cart:";
	if(!(get>>openHours)) return false;

	cout<<"Input the name of the snack cart:";
	if(!(get>>name)) return false;

	cout<<"Input the address of the snack cart:";
	if(!(get>>address)) return false;
 

	cout<<"Input the telephone of the snack cart:";
	if(!(get>>telephone)) return false;

	cout<<"Input the meal count of the snack cart:";
	if(!(get>>mealcount)) return false;

	for ( int i = 0 ; i<mealcount;i++)
	{
		if( !meals[i].getMeal(get,file)) return false;
	}


	return true;
}

void Restaurant::put(ostream&put,bool file)
{
	if (file)
	{
		put<<"R"<<" "<<ownername<<" "<<openHours<<" "<<name<<" "<<address<<" "<<telephone<<" "<<mealcount<<" ";
		
		for ( int i=0;i<mealcount;i++)
		{
			meals[i].putMeal(put,file);
		}
		
		put<<endl;
		return;
	}
		
	put<<"Restaurant No."<<number<<" Owner name:"<<ownername<<" Name:"<<name<<" Open Hours:"<<openHours
		<<" address:"<<address<<" telephone"<<telephone<<" Meal count:"<<mealcount<<endl;
	
	for (int i=0;i<mealcount;i++)
		meals[i].putMeal(put,file);
	
	this->putCheapestMeal(put,file);
	this->putBestMal(put,file);
	
	put<<endl;

	return;
}
