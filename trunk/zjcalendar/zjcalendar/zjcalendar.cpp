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
#include <windows.h>

zjCalendar::zjCalendar(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	_sound = new QSound("sound.wav");
	_id = -1;
	_prompt = true;
	timeid = startTimer(1000);  // 1-second timer

	//QIcon ico = style()->standardIcon(QStyle::SP_MessageBoxInformation);
	HICON icon = (HICON)LoadImageW(GetModuleHandle(NULL),L"IDI_ICON",IMAGE_ICON,0,0,0);

	ICONINFO info;
	GetIconInfo(icon, &info);
	QPixmap pixmap = QPixmap::fromWinHBITMAP( info.hbmColor, QPixmap::Alpha );
	DeleteObject(info.hbmColor);
	DeleteObject(info.hbmMask);
	DestroyIcon(icon);
	QIcon ico(pixmap);





	//main dialog gui setting

	//remove [?], add [_].
	flags = Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	//flags |= Qt::SplashScreen;
	//flags |= Qt::Popup;
	setWindowFlags(flags);

	setWindowTitle(tr(APP_NAME));
	setWindowIcon(ico);
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

	QShortcut* m_CTRL_S_Accel = new QShortcut(QKeySequence(tr("Ctrl+S")), this);

	connect(m_CTRL_S_Accel, SIGNAL(activated()), this, SLOT(saveNoteClicked()));


	//read config
	QSettings settings(tr("zjcalendar.ini"), QSettings::IniFormat);
	path = new QLineEdit;
	path->setText(settings.value(tr("note/path")).toString());

	if (path->text() == tr(""))
	{
		path->setText(QCoreApplication::applicationDirPath() + tr("/") + DEFAULT_NOTE_FILE_NAME);
		settings.setValue(tr("note/path"), path->text());
	}

	alarm = new QCheckBox(tr("Alarm"));
	alarm->setCheckState((Qt::CheckState)settings.value(tr("job/alarm")).toInt());
	settings.setValue(tr("job/alarm"), alarm->checkState());
	

	record = new QCheckBox(tr("&Record Start Time"));
	record->setCheckState((Qt::CheckState)settings.value(tr("job/record")).toInt());
	settings.setValue(tr("job/record"), record->checkState());

	digiflip = new DigiFlip();
	digiflip->setTransition(settings.value(tr("event/transition")).toInt());
	settings.setValue(tr("event/transition"), digiflip->getTransition());


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
	cfg = new QWidget();
	cfgIdx = mainTab->addTab(cfg, QString("Config"));



	//event tab

	QGridLayout* LayoutEvent = new QGridLayout(widgetEvent);

	msg = new EventMsg(widgetEvent);
	LayoutEvent->addWidget(msg, 0, 0);

	digiflip->resize(100, 300);
	LayoutEvent->addWidget(digiflip, 1, 0, 8, 0);

	//job tab
	QGridLayout* LayoutJob = new QGridLayout(widgetJob);

	msgold = new QLabel(tr(""));
	msgold->resize(100, 10);
	LayoutJob->addWidget(msgold, 0, 0, 1, 5);

	connect(record, SIGNAL(stateChanged(int)), this, SLOT(recordClicked(int)));
	LayoutJob->addWidget(record, 0, 6, 1, 1);

	QTime t = QTime::currentTime();
	startTime = new QTimeEdit(t);
	LayoutJob->addWidget(startTime, 0, 7, 1, 1);
	startTime->setEnabled(false);


	resetStartTime = new QPushButton(tr("&I Am Here"));
	connect(resetStartTime, SIGNAL(clicked()), this, SLOT(resetStartWorkTimeClicked()));
	LayoutJob->addWidget(resetStartTime, 0, 8, 1, 1);
	resetStartTime->setEnabled(false);

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
	noteEdit = new zjTextEdit(this);
	connect(noteEdit, SIGNAL(cursorPositionChanged()), this, SLOT(noteEditTextChanged()));
	LayoutNote->addWidget(noteEdit, 0, 0, 9, 9);
	QString fileNameNote = path->text(); 
	QFile file(fileNameNote);
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		noteEdit->setPlainText(file.readAll());
	}
	file.close();

	saveNote = new QPushButton;
	saveNote->setText(tr("&save"));
	connect(saveNote, SIGNAL(clicked()), this, SLOT(saveNoteClicked()));
	LayoutNote->addWidget(saveNote, 9, 8);



	//config tab
	QGridLayout* LayoutCfg = new QGridLayout(cfg);
	notepath = new QLabel(tr("Note Path:"));
	LayoutCfg->addWidget(notepath, 0, 0, 1, 1);
	LayoutCfg->addWidget(path, 0, 1, 1, 8);

	LayoutCfg->addWidget(alarm, 1, 0, 1, 9);

	saveCfg = new QPushButton;
	saveCfg->setText(tr("&save"));
	connect(saveCfg, SIGNAL(clicked()), this, SLOT(saveCfgClicked()));
	LayoutCfg->addWidget(saveCfg, 2, 8, 1, 1);


	//tray setting
	createTrayIcon();

	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	trayIcon->setIcon(ico);
	trayIcon->show();

}

void zjCalendar::noteEditTextChanged()
{
	QTextCursor tc = noteEdit->textCursor();
	tc.setPosition(-1);
	//tc.setVisualNavigation(true);
	//noteEdit->setTextCursor(noteEdit->textCursor());
	//tc.selectionEnd();
}


zjCalendar::~zjCalendar()
{
	QSettings settings(tr("zjcalendar.ini"), QSettings::IniFormat);
	settings.setValue(tr("event/transition"), digiflip->getTransition());
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

void zjCalendar::hideEvent(QHideEvent *event)
{
	_prompt = false;
	event->accept();
}

void zjCalendar::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}

void zjCalendar::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) 
	{
	case QSystemTrayIcon::Context:
		break;
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::DoubleClick:
		_prompt = true;
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
	hide();
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
		if (alarm->checkState() == Qt::Checked)
		{
			if (_sound->isFinished())
			{
				_sound->play();
			}
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
			SetWindowPos(winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
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
	QString fileNameNote = path->text(); 
	QFile file(fileNameNote);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{

	}
	QTextStream out(&file);
	out << note;
	file.close();
}

void zjCalendar::saveCfgClicked()
{
		QSettings settings(tr("zjcalendar.ini"), QSettings::IniFormat);
		settings.setValue(tr("note/path"), path->text());
		settings.setValue(tr("job/alarm"), alarm->checkState());
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
	events.editEvent(new_event_id, date_time, date_time, tr("下班时间到！！！"));
#if 0 ///Z:test 
	time = startTime->time();
	new_event_id = events.addEvent();
	time.setHMS(time.hour(), time.minute(), 0);
	t = time.toString("hh:mm:00");
	date_time.time = QTime::fromString(t, "hh:mm:ss");
	events.editEvent(new_event_id, date_time, date_time, tr("test 1"));

	new_event_id = events.addEvent();
	time.setHMS(time.hour(), time.minute()+1, 0);
	t = time.toString("hh:mm:00");
	date_time.time = QTime::fromString(t, "hh:mm:ss");
	events.editEvent(new_event_id, date_time, date_time, tr("test 2"));
#endif ///Z:end
}

void zjCalendar::resetStartWorkTimeClicked()
{
	events.delEvent(events.size()-1);
	loadEvents();
}

void zjCalendar::recordClicked(int s)
{
	Qt::CheckState state = record->checkState();

	if (state == Qt::Checked)
	{
		startTime->setEnabled(true);
		resetStartTime->setEnabled(true);
	}
	else if (state == Qt::Unchecked)
	{
		startTime->setEnabled(false);
		resetStartTime->setEnabled(false);
	}
	else
	{
		return;
	}
	QSettings settings(tr("zjcalendar.ini"), QSettings::IniFormat);
	settings.setValue(tr("job/record"), record->checkState());
	events.delEvent(events.size()-1);
	loadEvents();

}
void zjCalendar::loadEvents()
{
	model->removeRows(0, model->rowCount());
	QString filenameEvent = QCoreApplication::applicationDirPath() + tr("/") + tr(EVENT_FILE_NAME); 
	events.getEventsFromFile(filenameEvent);

	Qt::CheckState state = record->checkState();
	if (state == Qt::Checked)
	{
		addClosingTimeEvent();
	}
	else
	{

	}

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