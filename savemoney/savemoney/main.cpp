#include "savemoney.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	savemoney w;
	w.show();
	return a.exec();
}
