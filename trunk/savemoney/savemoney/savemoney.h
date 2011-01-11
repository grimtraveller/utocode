#ifndef SAVEMONEY_H
#define SAVEMONEY_H

#include <QtGui/QMainWindow>
#include "ui_savemoney.h"

class savemoney : public QMainWindow
{
	Q_OBJECT

public:
	savemoney(QWidget *parent = 0, Qt::WFlags flags = 0);
	~savemoney();

private:
	Ui::savemoneyClass ui;
private slots:
	void addRecord();
private:
	void createMenus();
	virtual void resizeEvent(QResizeEvent * event);
};

#endif // SAVEMONEY_H
