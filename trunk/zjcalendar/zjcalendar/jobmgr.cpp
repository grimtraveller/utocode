/**
 @file		jobmgr.cpp
 @brief		
 @details	
 @author	zuohaitao
 @date		2011-09-06
 warning	
 bug		
**/

#include "jobmgr.h"
#include "zjcalendar.h"
#include "macro.h"

JobMgr::JobMgr(QWidget* parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{

	isTextChanged = false;
	
		//remove [?], add [_].
	flags = Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	//flags |= Qt::SplashScreen;
	//flags |= Qt::Popup;
	setWindowFlags(flags);

	widget = new QWidget(this);
	QGridLayout* layout = new QGridLayout(widget);
	setLayout(layout);
	text = new QTextEdit;
	layout->addWidget(text, 0, 0, 9, 9);
	QString fileName = QCoreApplication::applicationDirPath() + tr("\\") + tr(EVENT_FILE_NAME); 
	QFile file(fileName);
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		text->setPlainText(file.readAll());
	}
	file.close();
	connect(text, SIGNAL(textChanged()), this, SLOT(textChanged()));
	save = new QPushButton;
	save->setText(tr("&save"));
	QShortcut* m_CTRL_S_Accel = new QShortcut(QKeySequence(tr("Ctrl+S")), this);

	connect(m_CTRL_S_Accel, SIGNAL(activated()), save, SLOT(clicked()));
	connect(save, SIGNAL(clicked()), this, SLOT(saveClicked()));
	layout->addWidget(save, 9, 8);
}

JobMgr::~JobMgr()
{
}

void JobMgr::closeEvent(QCloseEvent *event)
{
	if (isTextChanged)
	{
		int ret = QMessageBox::question(this, 
			tr(APP_NAME), 
			tr("Save changes?"), 
			QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
		if (QMessageBox::Yes == ret)
		{
			saveClicked();
		}
		else if (QMessageBox::Cancel == ret)
		{
			event->ignore();
		}
	}
}

void JobMgr::saveClicked()
{
	QString str = text->toPlainText();
	QString fileName = QCoreApplication::applicationDirPath() + tr("\\") + tr(EVENT_FILE_NAME); 
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		
	}
	QTextStream out(&file);
	out << str;
	file.close();
	isTextChanged = false;
}

void JobMgr::textChanged()
{
	isTextChanged = true;
}