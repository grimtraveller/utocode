#include "public.h"
int main()
{	
    long max_line;
    max_line = sysconf(_SC_LINE_MAX);
    printf("system max line:%d\n", max_line);
    return 0;
}
