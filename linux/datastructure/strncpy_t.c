int main()
{
	char s[11];
	char t[11];
	s[10] = 'b';
	memset(t, 'a', 11);
	t[10] = '\0';
	strncpy(s, t, 10);
	printf("%s\n", s);
	return 0;
}
