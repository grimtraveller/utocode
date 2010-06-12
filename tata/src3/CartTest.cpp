#include "SnackCart.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    SnackCart cart;
	
	//get cart from screen;
    cart.get(cin);
	//put cart to screen
	cart.put(cout);

	return 0;
}
