void single_quote_string
{
	//error C2015: too many characters in constant
	//char c = (char)'this is a char';
}

/* 1-1 */
int allowNestedComments()
{
	return /*/*/0*/**/1;
}

/* 1-4 */
void AAddAddAddAddAddBmean()
{
#if 0
	//error C2105: '++' needs l-value
	a+++++b;
#endif //0
}
int
main(int argv, char* argc[])
{
	
	printf("Does Compiler allow nested comments?%s\n", allowNestedComments()?"yes":"no");
	AAddAddAddAddAddBmean();
	mean();
	single_quote_string();
	return 0;
}
