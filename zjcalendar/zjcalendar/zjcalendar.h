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

#define EVENT_FILE_NAME "zjCalendar.dat"
#define NOTE_FILE_NAME	"todo.txt"
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
	 void mainTabSelected(const QString & tabname);
	 void saveNoteClicked();

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
	QTabWidget* mainTab;
	QWidget* widgetEvent;
	QWidget* widgetJob;
	QWidget* widgetNote;
	int eventIdx;
	int jobIdx;
	int noteIdx;
	QTextEdit* noteEdit;
	QPushButton* saveNote;

};

#endif // ZJCALENDAR_H
