/*
 * Name:	vectorerase.cpp
 * Desc:	Study how to delete item in vector
 * Usage:	In Win32,
 *			cl vectorerase.cpp vectorerase.exe
 *			In Linux,
 *			g++ vectroerase.cpp -o vectorerase
 * Author:	zuohaitao
 * Date:	2009/02/19
 *
 */
#pragma warning( disable : 521)
#pragma warning( disable : 525)
#include <vector>
using namespace std;
int main()
{
	vector<int> vec;
	for (int i = 0; i < 100; i++)
	{
		vec.push_back(i);
	}
	vector <int>::iterator item;
	for (item = vec.begin();item != vec.end();)
	{
		//printf("%d ", *item);
		if (*item%3 == 0)
		{
			item = vec.erase(item);
		}
		else
		{
			item++;
		}
	}
	for (int j = 0; j < vec.size(); j++)
	{
		printf("%d ", vec[j]);
	}
	return 0;
}

