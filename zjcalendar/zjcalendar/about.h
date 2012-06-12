/**
 @file		about.h
 @brief		
 @details	
 @author	zuohaitao
 @date		2012-06-08
 warning	
 bug		
**/
#ifndef ABOUT_H
#define ABOUT_H
#include <QtGui>
class About : public QDialog
{
	Q_OBJECT
public:
	About(QWidget *parent = 0, Qt::WFlags flags = 0);
	~About();
	QWidget* widget;
	QLabel* info;

};
#endif //JOBMGR_H
