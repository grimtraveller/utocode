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
#include "digiflip.h"
#include "eventmsg.h"
#include "zjtextedit.h"
#define EVENT_FILE_NAME "zjCalendar.dat"
#define DEFAULT_NOTE_FILE_NAME	"todo.txt"
#define MIN_WIDTH	700
#define MIN_HEIGHT	400
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
	 void hideEvent(QHideEvent *event);
	 void timerEvent(QTimerEvent *event);
	 void changeEvent(QEvent * event); 

 private slots:
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void messageClicked();
	 void mainTabSelected(const QString & tabname);
	 void saveNoteClicked();
	 void loadNoteClicked();
	 void saveCfgClicked();
	 void itemChanged(QStandardItem * item);
	 void editjobClicked();
	 void resetStartWorkTimeClicked();
	 void recordClicked(int s);
	 void menuSelected(QAction* action);

	 void noteEditTextChanged();
	 void about();

 private:
	 void addClosingTimeEvent();
	 void loadEvents();
     void createActions();
     void createTrayIcon();

	 QActionGroup* functionGrp;
     QAction *eventAction;
     QAction *jobAction;
	 QAction *noteAction;
     QAction *restoreAction;
     QAction *quitAction;
	 QAction *aboutAction;

     QSystemTrayIcon *trayIcon;
     QMenu *trayIconMenu;
private:
	int timeid;
	Events events;
	EventMsg* msg;
	QLabel* msgold;
	QTableView* table;
	QStandardItemModel *model;
	QTabWidget* mainTab;
	QWidget* widgetEvent;
	QWidget* widgetJob;
	QWidget* widgetNote;
	QWidget* cfg;
	int eventIdx;
	int jobIdx;
	int noteIdx;
	int cfgIdx;

	DigiFlip* digiflip;
	zjTextEdit* noteEdit;
	QPushButton* saveNote;
	QPushButton* loadNote;
	QPushButton* editjob;
	QPushButton* resetStartTime;
	QTimeEdit* startTime;
	QCheckBox* record;
	QLabel* notepath;
	QPushButton* saveCfg;
	QLineEdit* path;
	QCheckBox* alarm;

	int _id; //current Event ID 
	bool _prompt; //show dialog to prompt or not

	QSound* _sound;
};

#endif // ZJCALENDAR_H
