#include "SnackCart.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    SnackCart cart;
	
	//get cart from screen;
    cart.getCart(cin);
	//put cart to screen
	cart.putCart(cout);

	//order 100 meal,index 0
	int f = cart.orderMeal(0,100);
	cout<<"order 100 meal,index 0 total money:"<<f<<endl;
	return 0;
}
