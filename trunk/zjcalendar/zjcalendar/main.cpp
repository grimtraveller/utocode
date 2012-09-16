/**
 * @file	main.cpp 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#include "zjcalendar.h"
#include <QtGui/QApplication>
#include <QtGui>
#include "macro.h"
#include "app.h"

int main(int argc, char *argv[])
{
	App a(argc, argv);
	//a.setWindowIcon(a.style()->standardIcon(QStyle::SP_MessageBoxInformation));
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
	a.registerHotKey(w.winId());
	w.show();
	return a.exec();
}
