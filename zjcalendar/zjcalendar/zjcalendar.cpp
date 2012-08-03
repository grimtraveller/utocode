/**
 * @file	zjcalendar.cpp 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#include "zjcalendar.h"
#include "macro.h"
#include "jobmgr.h"
#include <QProcess>
#include "about.h"

zjCalendar::zjCalendar(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	_sound = new QSound("sound.wav");
	_id = -1;
	_prompt = true;
	timeid = startTimer(1000);  // 1-second timer

	QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);

	//main dialog gui setting
	
	//remove [?], add [_].
	flags = Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	//flags |= Qt::SplashScreen;
	//flags |= Qt::Popup;
	setWindowFlags(flags);

	setWindowTitle(tr(APP_NAME));
	setWindowIcon(icon);
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

	QShortcut* m_CTRL_S_Accel = new QShortcut(QKeySequence(tr("Ctrl+S")), this);

	connect(m_CTRL_S_Accel, SIGNAL(activated()), this, SLOT(saveNoteClicked()));

	//menu
	createActions();
	
	//main dialog
	mainTab = new QTabWidget(this);
	connect(mainTab, SIGNAL(selected(QString)), this, SLOT(mainTabSelected(QString)));
	QGridLayout* layoutMain = new QGridLayout();
	layoutMain->addWidget(mainTab, 0, 0, 0, 0);
	setLayout(layoutMain);

	widgetEvent = new QWidget();
	eventIdx = mainTab->addTab(widgetEvent, QString("Event"));
	widgetJob = new QWidget();
	jobIdx = mainTab->addTab(widgetJob, QString("Job"));
	widgetNote = new QWidget();
	noteIdx = mainTab->addTab(widgetNote, QString("Note"));

	
	//event tab

	QGridLayout* LayoutEvent = new QGridLayout(widgetEvent);

	msg = new EventMsg(widgetEvent);
	LayoutEvent->addWidget(msg, 0, 0);
	digiflip = new DigiFlip();
	digiflip->resize(100, 300);
	LayoutEvent->addWidget(digiflip, 1, 0, 8, 0);

	//job tab
	QGridLayout* LayoutJob = new QGridLayout(widgetJob);

	msgold = new QLabel(tr(""));
	msgold->resize(100, 10);
	LayoutJob->addWidget(msgold, 0, 0, 1, 6);
	QTime t = QTime::currentTime();
	startTime = new QTimeEdit(t);
	LayoutJob->addWidget(startTime, 0, 7, 1, 1);

	resetStartTime = new QPushButton(tr("&I Am Here"));
	connect(resetStartTime, SIGNAL(clicked()), this, SLOT(resetStartWorkTimeClicked()));
	LayoutJob->addWidget(resetStartTime, 0, 8, 1, 1);

	editjob = new QPushButton(tr("&Edit Job"));
	connect(editjob, SIGNAL(clicked()), this, SLOT(editjobClicked()));
	LayoutJob->addWidget(editjob, 0, 9, 1, 1);

	model = new QStandardItemModel(0, 3, this);
	model->setHeaderData(0, Qt::Horizontal, tr("Id"));
	model->setHeaderData(1, Qt::Horizontal, tr("Time"));
	model->setHeaderData(2, Qt::Horizontal, tr("Event"));
	
	QSplitter *splitter = new QSplitter;
	table = new QTableView;
	splitter->addWidget(table);
	splitter->setStretchFactor(0, 0);

	table->setModel(model);

	QHeaderView *headerView = table->horizontalHeader();
	headerView->setStretchLastSection(true);
	LayoutJob->addWidget(splitter, 1, 0, 9, 10);

	loadEvents();

	connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

	//note tab
	QGridLayout* LayoutNote = new QGridLayout(widgetNote);
	//noteEdit = new QTextEdit(this);
	//connect(noteEdit, SIGNAL(cursorPositionChanged()), this, SLOT(noteEditTextChanged()));

	//LayoutNote->addWidget(noteEdit, 0, 0, 9, 9);
	//QString fileNameNote = QCoreApplication::applicationDirPath() + tr("\\") + tr(NOTE_FILE_NAME); 
	//QFile file(fileNameNote);
	//if (file.open(QFile::ReadOnly | QFile::Text))
	//{
	//	noteEdit->setPlainText(file.readAll());
	//}
	//file.close();
	//saveNote = new QPushButton;
	//saveNote->setText(tr("&save"));
	//connect(saveNote, SIGNAL(clicked()), this, SLOT(saveNoteClicked()));
	//LayoutNote->addWidget(saveNote, 9, 8);


	editNote = new QPushButton;
	editNote->setText(tr("&edit with gVim"));
	connect(editNote, SIGNAL(clicked()), this, SLOT(editNoteClicked()));
	LayoutNote->addWidget(editNote, 9, 7);

	//tray setting
	createTrayIcon();

	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	trayIcon->setIcon(icon);
	trayIcon->show();

}

void zjCalendar::noteEditTextChanged()
{
	QTextCursor tc = noteEdit->textCursor();
	tc.setVisualNavigation(true);
	noteEdit->setTextCursor(noteEdit->textCursor());	
}
zjCalendar::~zjCalendar()
{
	killTimer(timeid);
}

void zjCalendar::setVisible(bool visible)
{
	restoreAction->setEnabled(/*isMaximized() || */!visible);
	QDialog::setVisible(visible);
	if (!isMinimized())
	{

		QSize s = QApplication::desktop()->size();
		resize(MIN_WIDTH, MIN_HEIGHT);
		move(s.width()/2-350, s.height()/2-150);
		
	}
}

void zjCalendar::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) 
	{
		/*
		int ret = QMessageBox::question(this, 
			tr(APP_NAME), 
			tr("Are you sure close?"), 
			QMessageBox::Yes|QMessageBox::No);
		*/
		int ret = QMessageBox::No;
		if (QMessageBox::No == ret)
		{
			hide();
			event->ignore();
		}
	}
}

void zjCalendar::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) 
	{
	 case QSystemTrayIcon::Trigger:
		 break;
	 case QSystemTrayIcon::DoubleClick:
		 hide();
		 setWindowFlags(Qt::WindowStaysOnTopHint);
		 showNormal();
		 break;
	 case QSystemTrayIcon::MiddleClick:
		 show();
		 break;
	 default:
		 break;
	}
}

void zjCalendar::messageClicked()
{
	// show next task message
	setWindowFlags((Qt::WindowFlags)(~Qt::WindowStaysOnTopHint));
	_prompt = false;
}

void zjCalendar::createActions()
{
	///to do add event, joblist, note
	eventAction = new QAction(tr("&Event"), this);
	eventAction->setCheckable(true); 
	jobAction = new QAction(tr("&Job"), this);
	jobAction->setCheckable(true); 
	noteAction = new QAction(tr("&Note"), this);
	noteAction->setCheckable(true);
	//create a group for mutex each in croup
	functionGrp = new QActionGroup(this);
	//add menu to group
    functionGrp->addAction(eventAction);         
    functionGrp->addAction(jobAction);
    functionGrp->addAction(noteAction);
	//set default selected menu
    eventAction->setChecked(true);
	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	connect(functionGrp, SIGNAL(triggered(QAction*)), this, SLOT(menuSelected(QAction*)));
}

void zjCalendar::menuSelected(QAction* action)
{
	if (eventAction == action)
	{
		mainTab->setCurrentIndex(eventIdx);
	}
	else if (jobAction == action)
	{
		mainTab->setCurrentIndex(jobIdx);
	}
	else if (noteAction == action)
	{
		mainTab->setCurrentIndex(noteIdx);
	}
	else
	{
		//NEVER DO THIS!
	}
	
}

void zjCalendar::createTrayIcon()
{
	///to do add event, joblist, note 
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(aboutAction);
	trayIconMenu->addAction(eventAction);
	trayIconMenu->addAction(jobAction);
	trayIconMenu->addAction(noteAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}

void zjCalendar::timerEvent(QTimerEvent *event)
{
	QString d;
	QDate date = QDate::currentDate();
	d = date.toString("yyyy-MM-dd");

	QString t;
	QTime time =  QTime::currentTime();
	t = time.toString("hh:mm:00");

	DateTime dt;
	dt.time = time;
	int id;
	dt.time = QTime::fromString(t);
	if (-1 != (id = events.haveEvent(dt)))
	{
		if (_sound->isFinished())
		{
			_sound->play();
		}
		mainTab->setCurrentIndex(eventIdx);
		table->setFocus();
		trayIcon->showMessage(events.eventMap[id].desc,
			d+tr(" ")+t, 
			QSystemTrayIcon::Information, 
			30*1000);
		msgold->setText(d+tr(" ")+t+tr(" <br><b>")+events.eventMap[id].desc+tr("</b>"));
		msg->setEventMsg(events.eventMap[id].desc);
		for (int idx = 0; idx < model->rowCount(); idx++)
		{
			if (id == model->data(model->index(idx, 0, QModelIndex()), Qt::DisplayRole))
			{
				table->setSelectionBehavior(QAbstractItemView::SelectRows);
				table->setCurrentIndex(model->index(idx, 0));
			}
		}
		if ((_id != id)) ///event changed
		{
			//save event id and reset prompt flag
			_id = id; 
			_prompt = true;
			
		}
		if (_prompt)
		{
			showNormal();
		}
	}
	else
	{
		time =  QTime::currentTime();
		t = time.toString("hh:mm:ss");
		msgold->setText(d+tr(" ")+t);
		//msg->setEventMsg(d+tr(" ")+t);
		table->setSelectionBehavior(QAbstractItemView::SelectItems);
	}
}

void zjCalendar::changeEvent(QEvent* event)
{
	if(event->type()==QEvent::WindowStateChange)
	{
		int stat = windowState();
		if(stat & Qt::WindowMinimized)
		{
			QTimer::singleShot(0, this, SLOT(hide()));
		}
	}

}

void zjCalendar::mainTabSelected(const QString & tabname)
{

}

void zjCalendar::saveNoteClicked()
{
	QString note = noteEdit->toPlainText();
	QString fileNameNote = QCoreApplication::applicationDirPath() + tr("\\") + tr(NOTE_FILE_NAME); 
	QFile file(fileNameNote);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		
	}
	QTextStream out(&file);
	out << note;
	file.close();
}

void zjCalendar::editNoteClicked()
{
	QStringList args;
	args << QDir::currentPath() + tr("\\todo.txt");
	QProcess::startDetached(tr("C:\\Program Files\\Vim\\vim73\\gvim.exe"),args);
}

void zjCalendar::itemChanged(QStandardItem * item)
{
	//QString id;
	//id.setNum(events.eventMap[item->row()].id);
	//model->setData(model->index(item->row(), 0, QModelIndex()), id);
	//model->setData(model->index(item->row(), 1, QModelIndex()), events.eventMap[item->row()].from.time.toString());

}

void zjCalendar::editjobClicked()
{
	JobMgr job_mgr;
	job_mgr.resize(730, 300);
	job_mgr.exec();
	loadEvents();
}

void zjCalendar::addClosingTimeEvent()
{
	int new_event_id = events.addEvent();
	QString d;
	QDate date = QDate::currentDate();
	d = date.toString("yyyy-MM-dd");

	QTime time = startTime->time();
	int h = time.hour()+1+8;
	if (h > 24)
	{
		h -= 24;
	}
	time.setHMS(h, time.minute(), 0);
	QString t = time.toString("hh:mm:00");
	DateTime date_time;
	date_time.time = QTime::fromString(t, "hh:mm:ss");
	events.editEvent(new_event_id, date_time, date_time, tr("下班时间到！！"));
}

void zjCalendar::resetStartWorkTimeClicked()
{
	events.delEvent(events.size()-1);
	loadEvents();
}

void zjCalendar::loadEvents()
{
	model->removeRows(0, model->rowCount());
	QString filenameEvent = QCoreApplication::applicationDirPath() + tr("/") + tr(EVENT_FILE_NAME); 
	events.getEventsFromFile(filenameEvent);

	addClosingTimeEvent();
	
	DateTime dt;
	//dt.time = time;
	//int id;
	//dt.time = QTime::fromString(t);
	std::map<int, Event>::iterator it;
	int row = 0;
	QString id;
	for (it = events.eventMap.begin(); it != events.eventMap.end(); it++)
	{
		model->insertRows(row, 1, QModelIndex());
		id.setNum((*it).second.id);
		model->setData(model->index(row, 0, QModelIndex()), id);
		model->setData(model->index(row, 1, QModelIndex()), (*it).second.from.time.toString());
		model->item(row, 0)->setEditable(false);
		model->item(row, 1)->setEditable(false);
		QLabel* desclabel = new QLabel(tr("<font size=\"4\">")+(*it).second.desc+tr("</font>"));
		table->setIndexWidget(model->index(row, 2, QModelIndex()), desclabel); 
		row++;
	}
}

void zjCalendar::about()
{
	About about;
	//about.resize(730, 300);
	about.exec();
}