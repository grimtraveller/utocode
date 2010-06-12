#include "SnackCart.h"
SnackCart::SnackCart()
{
	mealcount=0;
        number=0;
	for(int i=0; i < 5;i++) mealrates[i]=0;
}

float SnackCart::orderMeal(int index,int count)
{
	if ( index>=5) return 0;
	mealrates[index]+=count;
	return meals[index].orderMeal(count);
}

float SnackCart::orderMeal(string mealname,int count)
{
	int i;
	for(i=0;i<5;i++) if ( mealname==meals[i].name) break;

	if ( i>=5) return 0;
	mealrates[i]+=count;
	return meals[i].orderMeal(count);
}

bool SnackCart::getCart(istream&get,bool file)
{
	if ( file)
	{
		if ( !(get>>ownername>>location>>mealcount) ) return false;
		
		for ( int i = 0;i<mealcount;i++)
		{
			if (!meals[i].getMeal(get,file)) return false;
			meals[i].number=i+1;
		}

		return true;
	}

	cout<<"Input the owner name of the snack cart:";
	if(!(get>>ownername)) return false;

	cout<<"Input the location of the snack cart:";
	if(!(get>>location)) return false;

	cout<<"Input the meal count of the snack cart:";
	if(!(get>>mealcount)) return false;

	for ( int i = 0 ; i<mealcount;i++)
	{
		if( !meals[i].getMeal(get,file)) return false;
	}

	return true;
}


void SnackCart::putCart(ostream& put,bool file)
{
	if ( file)
	{
		put<<ownername<<" "<<location<<" "<<mealcount<<" ";
		
		for ( int i=0;i<mealcount;i++)
		{
			meals[i].putMeal(put,file);
		}
		
		put<<endl;
		return;
	}
		
	put<<"SnackCart No."<<number<<" Owner name:"<<ownername
		<<" Location:"<<location<<" Meal count:"<<mealcount<<endl;
	
	for (int i=0;i<mealcount;i++)
		meals[i].putMeal(put,file);
	
	this->putCheapestMeal(put,file);
	this->putBestMal(put,file);
	
	put<<endl;
	
}

void SnackCart::putCheapestMeal(ostream& put,bool file)
{
	if ( !file ) 
	{
		put<<"The Cheapest Meal:";
		cheapestMeal().putMeal(put,file);
	}
}

void SnackCart::putBestMal(ostream& put,bool file)
{
	if ( !file ) 
	{
		put<<"The Best Meal:";
		bestMeal().putMeal(put,file);
	}
}

Meal SnackCart::cheapestMeal() 
{
	Meal meal = meals[0];

	for ( int i = 1; i < mealcount; ++i)
		if (meals[i]<meal) meal = meals[i];
	return meal;
}

Meal SnackCart::bestMeal()
{
	Meal meal = meals[0];
	int meali=0;

	for ( int i = 1; i < mealcount; ++i)
	{
		if (meals[i]<meal)
		{
			meal = meals[i];
			meali=i;
			continue;
		}

		if ( meal<meals[i]) continue;
		if ( mealrates[i]>mealrates[meali])
		{
			meal = meals[i];
			meali=i;
		}
	}

	return meal;
}

