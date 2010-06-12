
#include "Meal.h"
#include <iostream>
using namespace std;

int main()
{
   Meal meal;
   //get meal from screen
   meal.getMeal(cin);

   //put meal to srceen
   meal.putMeal(cout);

   //order 100 meals
   float f=meal.orderMeal(100);
   cout<<"money:"<<f;

   return 0;
}
