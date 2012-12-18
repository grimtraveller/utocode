/**
 @file		tmapfind.cpp
 @brief		map<>.find函数如果值为map.<>.end()则表示未找到
 @details	
 @author	zuohaitao
 @date		2012-12-14
 warning	
 bug		
**/

#include <map>
using namespace std;
int
main(int argc, char* argv[])
{

	map<long, long> long_map;
	map<long, long>::iterator it;
	printf("add 1 in map\n");
	long_map[1] = 1;
	//it->first = 1, it->second = 1;
	//long_map.insert(it);
	printf("1 is ");
	it = long_map.find(1);
	if (it != long_map.end())
	{
		printf("found!");
	}
	else
	{
		printf("not found!");
	}
	printf("\n");
	it = long_map.find(2);
	printf("2 is ");
	it = long_map.find(2);
	if (it != long_map.end())
	{
		printf("found!");
	}
	else
	{
		printf("not found!");
	}
	printf("\n");
	return 0;
}
