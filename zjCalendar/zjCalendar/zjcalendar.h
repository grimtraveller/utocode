#ifndef ZJCALENDAR_H
#define ZJCALENDAR_H

#include <QtGui>
#include <map>
#include <events.h>
class QAbstractItemModel;
//class QItemSelectionModel;
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

 private slots:
     void setIcon(int index);
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void showMessage();
     void messageClicked();

 private:
     void createIconGroupBox();
     void createMessageGroupBox();
     void createActions();
     void createTrayIcon();

     QGroupBox *iconGroupBox;
     QLabel *iconLabel;
     QComboBox *iconComboBox;
     QCheckBox *showIconCheckBox;

     QGroupBox *messageGroupBox;
     QLabel *typeLabel;
     QLabel *durationLabel;
     QLabel *durationWarningLabel;
     QLabel *titleLabel;
     QLabel *bodyLabel;
     QComboBox *typeComboBox;
     QSpinBox *durationSpinBox;
     QLineEdit *titleEdit;
     QTextEdit *bodyEdit;
     QPushButton *showMessageButton;

     QAction *minimizeAction;
     QAction *maximizeAction;
     QAction *restoreAction;
     QAction *quitAction;

     QSystemTrayIcon *trayIcon;
     QMenu *trayIconMenu;
private:
	int timeid;
	Events events;
	//std::map<QString, QString> events;
	QLabel* msg;
	QTableView* table;
	QAbstractItemModel *model;
	//QItemSelectionModel *selectionModel;



};

#endif // ZJCALENDAR_H
