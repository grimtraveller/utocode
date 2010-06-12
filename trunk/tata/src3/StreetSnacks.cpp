
#include <vector>
#include "Eatery.h"
#include "Restaurant.h"
#include "SnackCart.h"
#include <fstream>
using namespace std;

int getMenuOption(void);
int loadEateries(string filename, vector<Eatery*>& eaterys); 
void saveEateries(string filename, vector<Eatery*>& eaterys);
void displayEateries(vector<Eatery*>& eaterys);
int addEatery(vector<Eatery*>& eaterys);
int delEatery(vector<Eatery*>& eaterys, const char* ownname);
int main(void) {
	
	const char file[] = "Eateries.txt";
	vector<Eatery*> eaterys;
	bool running = true;
	string strOwnname;	//only be used by delete
	// main program loop
	while (running) {
		int option = getMenuOption();
		switch (option) {
		case 1: 
			loadEateries(file,eaterys);
			break;
		case 2:
			saveEateries(file,eaterys);
			break;
		case 3:
			displayEateries(eaterys);
			break;
		case 4:
			addEatery(eaterys);
			break;
		case 5:
			cout<<" Input the ownname:";
			cin>>strOwnname;
			delEatery(eaterys, strOwnname.c_str());
			break;
		case 6:
			running = false;
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
		<< "1 - load eateries information" << endl
		<< "2 - save eateries information" << endl
		<< "3 - display eateries information" << endl
		<< "4 - add new snack cart" << endl
		<< "5 - delete a eatery" << endl
		<< "6 - exit program" << endl;
	cout << "Enter option (1-6): ";

	int value;
	cin >> value;
	return value;
}

int loadEateries(string filename, vector<Eatery*>& eaterys) 
{

	char type;
	Eatery* pEatery;
	eaterys.clear();
	ifstream dataFile(filename.c_str());

	while(dataFile>>type)
	{
		pEatery = NULL;
		if (SNACKCART == type)
		{
			pEatery = new SnackCart;
		}
		else if (RESTAURANT == type)
		{
			pEatery = new Restaurant;
		}
		else if (DEL == type)
		{
			string str;
			getline(dataFile, str);
			continue;
		}
		else
		{
			//never run this step
			//only when the format of Eateries.txt is ERROR! 
			cout<< "the format of Eateries.txt is ERROR!" <<endl;
			return -1;
		}
		if (!pEatery->get(dataFile, true))
		{
			break;
		}
		pEatery->number = eaterys.size();
		eaterys.push_back(pEatery);
	}
	dataFile.close();
	return eaterys.size();
}

void saveEateries(string filename,  vector<Eatery*>& eaterys)
{
    ofstream dataFile(filename.c_str());

    for (int i = 0; i < eaterys.size(); i++)
	{
		if (SNACKCART == eaterys[i]->type)
		{
			SnackCart* pS = (SnackCart*)eaterys[i];
			pS->put(dataFile, true);
		}
		else if (RESTAURANT == eaterys[i]->type)
		{
			Restaurant* pR = (Restaurant*)eaterys[i];
			pR->put(dataFile, true);
		}
		else
		{
			//should be type = '*'
			continue;
		}
	}

    dataFile.close();
}

void displayEateries(vector<Eatery*>& eaterys)
{
	for (int i = 0; i < eaterys.size(); i++)
	{
		if (SNACKCART == eaterys[i]->type)
		{

			SnackCart* pS = (SnackCart*)eaterys[i];
			pS->put(cout);
		}
		else if (RESTAURANT == eaterys[i]->type)
		{
			Restaurant* pR = (Restaurant*)eaterys[i];
			pR->put(cout);
		}
		else
		{
			continue;
		}
	}
}

int addEatery(vector<Eatery*>& eaterys)
{
	char type;
	Eatery* pEatery = NULL;
	cout<<"Input the type of the Eatery('S' means snack cart and 'R' means restaurant):";
	cin>>type;
	if (SNACKCART == type)
	{
		pEatery = new SnackCart;
	}
	else if (RESTAURANT == type)
	{
		pEatery = new Restaurant;
	}
	else
	{
		return -1;
	}
	pEatery->get(cin);
	pEatery->number = eaterys.size();
	eaterys.push_back(pEatery);
	return eaterys.size();
}

int delEatery(vector<Eatery*>& eaterys, const char* ownname)
{
	for (int i = 0; i < eaterys.size(); i++)
	{
		if (eaterys[i]->ownername == ownname)
		{
			eaterys[i]->type = DEL;
			return 0;
		}
			
	}
	cout<<"Not find the Eatery!"<<endl;
	return -1;
}


