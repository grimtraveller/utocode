#ifndef MEAL
#define MEAL

#include <string>
#include <iostream>
using namespace std;

class Meal  
{
public:
	Meal();

public:
	//get meal from screen or file
	bool getMeal(istream&get,bool file=false);
	//put meal to screen or file
	void putMeal(ostream&put,bool file=false);
    //order the meal
	float orderMeal(int count);

public:
	int number;
	string name;
	float  cost;
	int rate;
};

bool operator < (Meal& meal,Meal& meal1);

#endif 
