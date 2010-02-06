
#include "Eatery.h"
#include <fstream>
#include <vector>
using namespace std;

const int MAX_SIZE = 5; // set small for testing purposes

int getMenuOption(void);
int loadEatery(string filename, vector<Eatery*>& eaterys);
//void saveCarts(string filename, SnackCart carts[],int count);
//void displayCarts(SnackCart carts[],int count);
//int addCart(SnackCart carts[],int count);

int main(void)
{	
	char szFile[] = "";
	vector<Eatery*> eaterys;
	int count = 0;
	bool running = true;
	
	// main program loop
	while (running) {
		int option = getMenuOption();
		switch (option) {
		case 1: 
			count = loadEatery("eateries.txt",eaterys);
			break;
/*
		case 2:
			saveEatery("carts.txt",carts,count);
			break;
		case 3:
			displayEatery(carts,count);
			break;
		case 4:
			displayCart(carts, count);
		case 5:
			displayRestaurant(carts, count);
		case 6:
			count = addEatery(eaterys,count);
			break;
		case 7:
			count = delEatery(eaterys, num);
		case 8:
			running = false;
			break;
*/
		default:
			break;
		}
	}
	for (int i = 0; i < eaterys.size(); i++)
	{
		delete eaterys[i];
	}
	return 0;
}

int getMenuOption(void) 
{
	cout << endl;
	cout << "Menu:" << endl
		<< "1 - load eatery information" << endl
		<< "2 - save eatery information" << endl
		<< "3 - display eatery information" << endl
		<< "4 - display snack cart information" << endl
		<< "5 - display restaurant information" << endl
		<< "6 - add new eatery" << endl
		<< "7 - delete eatery" << endl
		<< "8 - exit program" << endl;
	cout << "Enter option (1-8): ";

	int value;
	cin >> value;
	return value;
}

int loadEatery(string filename, vector<Eatery*>& eaterys) 
{
	int count = 0;
	char type;
	Eatery* eatery;
	ifstream dataFile(filename.c_str());

	while(!(dataFile>>type))
	{
		if (type == Eatery::DEL)
		{
			continue;
		}
		else if (type == Eatery::RESTAURANT)
		{
			printf("restaurant\n");
		}
		else if (type == Eatery::SNACKCART)
		{
			printf("snack cart\n");
		}
		else
		{
			//never can be run
			//cout>>"error type!">>endl;
		}
	}
/*
	Eatery eatery;
	
	for ( count = 0; count < MAX_SIZE; ++count)
	{
		if (!carts[count].getCart(dataFile,true))break;
		carts[count].number=count+1;
	}

	dataFile.close();
	return count;
*/
}
/*
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
*/
/* add for expand service to restaurant sector */

/*
int loadRestaurant(string filename, Restaurant restaurants[]) 
{
	int count = 0;
	ifstream dataFile(filename.c_str());
	
	for ( count = 0; count < MAX_SIZE; ++count)
	{
		if (!restaurants[count].get(dataFile,true))break;
		restaurants[count].number=count+1;
	}

	dataFile.close();
	return count;
}
*/
