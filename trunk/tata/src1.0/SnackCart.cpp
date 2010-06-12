#include "SnackCart.h"
SnackCart::SnackCart()
{
	mealcount=0;
    number=0;
}

float SnackCart::orderMeal(int index,int count)
{
	if ( index>=5) return 0;
	meals[index].rate+=count;
	return meals[index].orderMeal(count);
}

float SnackCart::orderMeal(string mealname,int count)
{
	int i;
	for(i=0;i<5;i++) if ( mealname==meals[i].name) break;

	if ( i>=5) return 0;
	meals[i].rate+=count;
	return meals[i].orderMeal(count);
}

bool SnackCart::get(istream&get,bool file)
{
	type = SNACKCART;
	if ( file)
	{
		if ( !(get>>ownername>>openHours>>location>>mealcount) ) return false;
		
		for ( int i = 0;i<mealcount;i++)
		{
			if (!meals[i].getMeal(get,file))
			{
				printf("eee\n");
				return false;
			}
			meals[i].number=i+1;
		}

		return true;
	}

	cout<<"Input the owner name of the snack cart:";
	if(!(get>>ownername)) return false;


	cout<<"Input the open hours of the snack cart:";
	if(!(get>>openHours)) return false;

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


void SnackCart::put(ostream& put,bool file)
{
	if ( file)
	{
		put<<"S"<<" "<<ownername<<" "<<openHours<<" "<<location<<" "<<mealcount<<" ";
		
		for ( int i=0;i<mealcount;i++)
		{
			meals[i].putMeal(put,file);
		}
		
		put<<endl;
		return;
	}
		
	put<<"SnackCart No."<<number<<" Owner name:"<<ownername<<" Open hours:"<<openHours
		<<" Location:"<<location<<" Meal count:"<<mealcount<<endl;
	
	for (int i=0;i<mealcount;i++)
		meals[i].putMeal(put,file);
	
	this->putCheapestMeal(put,file);
	this->putBestMal(put,file);
	
	put<<endl;
	
}
