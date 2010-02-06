#include "Eatery.h"
class Restaurant : public Eatery
{
public:
	Restaurant();
	~Restaurant();
	//get restaurants from file or screen
	bool get(istream&get,bool file=false);
	//put restaurants to screen or file
	void put(ostream&put,bool file=false);
private:
	string name;
	string address;
	string telephone;
	int openningtime;
};
