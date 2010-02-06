#ifndef EATERY_H
#define EATERY_H
#include <string>
#include "Meal.h"
#define		SNACKCART	'S'
#define		RESTAURANT	'R'
#define		DEL			'*'
using namespace std;
class Eatery
{
public:
	Eatery();
	~Eatery();
	char type;
	int  mealcount;
	int  number;
	Meal meals[5];
	//get eateries from file or screen
	virtual bool get(istream&get,bool file=false) = 0;
	//put eateries to screen or file
	virtual void put(ostream&put,bool file=false) = 0;
	//put cheapest cart 
	void putCheapestMeal(ostream&put,bool file=false);
	//put best cart
	void putBestMal(ostream&put,bool file=false);
	string ownername;
	int openHours;
	ostream& operator << (ostream &output);
	istream& operator >> (istream &input);

protected:
	Meal cheapestMeal();
	Meal bestMeal();
	
};
#endif	//EATERY
