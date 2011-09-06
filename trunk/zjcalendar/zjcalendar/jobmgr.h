/**
 @file		jobmgr.h
 @brief		
 @details	
 @author	zuohaitao
 @date		2011-09-06
 warning	
 bug		
**/
#ifndef JOBMGR_H
#define JOBMGR_H
#include <QtGui>
class JobMgr : public QDialog
{
	Q_OBJECT
public:
	JobMgr(QWidget *parent = 0, Qt::WFlags flags = 0);
	~JobMgr();
protected:
	void closeEvent(QCloseEvent* event);
private slots:
	void saveClicked();
	void textChanged();
private:
	bool isTextChanged;
	QWidget* widget;
	QTextEdit* text;
	QPushButton* save;

};
#endif //JOBMGR_H
