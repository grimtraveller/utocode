#ifndef MEAL
#define MEAL

#include <string>
#include <iostream>
using namespace std;

class Meal  
{
public:
	Meal();
	~Meal();

public:
	//get meal from screen or file
	bool getMeal(istream&get,bool file=false);
	//put meal to screen or file
	void putMeal(ostream&put,bool file=false);
    //order the meal
	float orderMeal(int count);
	//Modify for restruct in 2
	//move this operator reload function into class Meal 
	bool operator < (const Meal& that);
	Meal& operator = (const Meal& that);
public:
	int number;
	string name;
	float  cost;
	//Add for restruct in 2 
	int rate;
};


#endif 
