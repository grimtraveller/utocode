#ifndef APP_H
#define APP_H
#include <QtGui>
#include <windows.h>
class App : public QApplication
{
	Q_OBJECT
public:
	App(int& argc, char** argv);
	~App();
	void registerHotKey(WId wid);
	virtual bool winEventFilter(MSG *msg, long *result);
private:
	ATOM hKeyWin[4];
	WId _wid;
};
#endif //APP_H