#include "zjrt0.h"
int
main(int argc, char* argv[])
{
	zjInitLog("log.txt", 0);
	zjPrintLog(0, "ÖÐÎÄEnglish!@#$%^&*()_+x=%d", 1);
	zjCloseLogFile();
	return 0;
}
