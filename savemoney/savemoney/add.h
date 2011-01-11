#ifndef ADD_H
#define ADD_H

#include <QWidget>
#include "ui_add.h"

class add : public QWidget
{
	Q_OBJECT

public:
	add(QWidget *parent = 0);
	~add();

private:
	Ui::add ui;
};

#endif // ADD_H
