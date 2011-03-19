#include "zjcalendar.h"
#include <QtGui/QApplication>
#include <QtGui>
#include "macro.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//get system  character set codec
	QTextCodec *codec = QTextCodec::codecForName("System");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	if (!QSystemTrayIcon::isSystemTrayAvailable()) 
	{
		QMessageBox::critical(0, 
			QObject::tr(APP_NAME),
			QObject::tr("Any system tray is detected on this system."));
		return 1;
	}
	zjCalendar w;
	w.show();
	return a.exec();
}
