#ifndef RESTAURANT_H
#define RESTAURANT_H
#include "Eatery.h"
class Restaurant : public Eatery
{
public:
	Restaurant();
	~Restaurant();
	//put restaurant to screen or file
	bool get(istream& get, bool file=false);
	//put restaurant to screen or file
	void put(ostream&put,bool file=false);
	//use it ?
	//put cheapest cart 
	//void putCheapestMeal(ostream&put,bool file=false);
	//put best cart
	//void putBestMal(ostream&put,bool file=false);
private:
	string name;
	string address;
	string telephone;
	//Q1
	int openingHour;
	//int closehour;
};
#endif //RESTAURANT_H
