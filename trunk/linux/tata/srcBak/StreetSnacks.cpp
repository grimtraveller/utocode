
#include "SnackCart.h"
#include <fstream>
using namespace std;

const int MAX_SIZE = 5; // set small for testing purposes

int getMenuOption(void);
int loadCarts(string filename, SnackCart carts[]);
void saveCarts(string filename, SnackCart carts[],int count);
void displayCarts(SnackCart carts[],int count);
int addCart(SnackCart carts[],int count);

int main(void) {
	
	SnackCart carts[MAX_SIZE];
	int count = 0;
	bool running = true;
	
	// main program loop
	while (running) {
		int option = getMenuOption();
		switch (option) {
		case 1: 
			count = loadCarts("carts.txt",carts);
			break;
		case 2:
			saveCarts("carts.txt",carts,count);
			break;
		case 3:
			displayCarts(carts,count);
			break;
		case 4:
			count = addCart(carts,count);
			break;
		case 5:
			running = false;
			break;
		}
	}
	
	return 0;
}

int getMenuOption(void) 
{
	cout << endl;
	cout << "Menu:" << endl
		<< "1 - load snack cart information" << endl
		<< "2 - save snack cart information" << endl
		<< "3 - display snack cart information" << endl
		<< "4 - add new snack cart" << endl
		<< "5 - exit program" << endl;
	cout << "Enter option (1-5): ";

	int value;
	cin >> value;
	return value;
}

int loadCarts(string filename, SnackCart carts[]) 
{
	int count = 0;
	ifstream dataFile(filename.c_str());
	
	for ( count = 0; count < MAX_SIZE; ++count)
	{
		if (!carts[count].getCart(dataFile,true))break;
		carts[count].number=count+1;
	}

	dataFile.close();
	return count;
}

void saveCarts(string filename,  SnackCart carts[],int count)
{
    ofstream dataFile(filename.c_str());

    for (int i = 0; i < count; ++i) 
		carts[i].putCart(dataFile,true);

    dataFile.close();
}

void displayCarts(SnackCart carts[],int count)
{
	for (int i = 0; i < count; ++i) 
		carts[i].putCart(cout);
}

int addCart(SnackCart carts[],int count)
{
	if (count>=MAX_SIZE)
	{
		cout<<"count is too big!"<<endl;
		return count;
	}

    carts[count].getCart(cin);
	carts[count].number=count+1;

    return count + 1;
}


