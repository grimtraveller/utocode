/*
 * Name:	f2c.c
 * Desc:	print the table Fahrenheit and Celsius temperatures with head
 * Usage:	$>gcc f2c.c -o f2c
 *			$>./f2c
 * Author:	zuohaitao
 * Date:	2009年 09月 03日 星期四 16:07:32 CST
 */
#include <stdio.h>
int main()
{
	float fahr, celsius;
	float lower, upper, step;
	lower = 0;
	upper = 300;
	step = 20;

	printf("Fahrenheit Celsius\n");

/*
	fahr = lower;
	while ( fahr <= upper)
	{
		celsius = (5.0/9.0) * (fahr - 32.0);
		printf("%10.0f %7.1f\n", fahr, celsius);
		fahr = fahr + step;
	}
*/
	for (fahr = upper; fahr >= 0; fahr -= step)
	{
		celsius = (5.0/9.0) * (fahr - 32.0);
		printf("%10.0f %7.1f\n", fahr, celsius);
	}
}
