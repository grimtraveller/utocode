/**
 * @file	zjcalendar.h 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#ifndef ZJCALENDAR_H
#define ZJCALENDAR_H

#include <QtGui>
#include <map>
#include <events.h>
class QAbstractItemModel;
class zjCalendar : public QDialog
{
	Q_OBJECT
public:
	zjCalendar(QWidget *parent = 0, Qt::WFlags flags = 0);
	~zjCalendar();

     void setVisible(bool visible);

 protected:
     void closeEvent(QCloseEvent *event);
	 void timerEvent(QTimerEvent *event);
	 void changeEvent(QEvent * event); 

 private slots:
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void messageClicked();

 private:
     void createActions();
     void createTrayIcon();

	 QActionGroup* functionGrp;
     QAction *eventAction;
     QAction *jobAction;
	 QAction *noteAction;
     QAction *restoreAction;
     QAction *quitAction;

     QSystemTrayIcon *trayIcon;
     QMenu *trayIconMenu;
private:
	int timeid;
	Events events;
	QLabel* msg;
	QTableView* table;
	QAbstractItemModel *model;



};

#endif // ZJCALENDAR_H
