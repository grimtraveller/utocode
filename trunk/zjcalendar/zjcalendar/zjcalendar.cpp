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
zjCalendar::zjCalendar(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
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
	setMinimumSize(700, 300);

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
	
	msg = new QLabel(tr(""));
	msg->resize(100, 300);

	LayoutEvent->addWidget(msg);

	QString filenameEvent = QCoreApplication::applicationDirPath() + tr("/") + tr(EVENT_FILE_NAME); 
	events.getEventsFromFile(filenameEvent);
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
	LayoutEvent->addWidget(splitter, 1, 0, 9, 1);

	std::map<int, Event>::iterator it;
	int row = 0;
	QString id;
	for (it = events.eventMap.begin(); it != events.eventMap.end(); it++)
	{
		model->insertRows(row, 1, QModelIndex());
		id.setNum((*it).second.id);
		model->setData(model->index(row, 0, QModelIndex()), id);
		model->setData(model->index(row, 1, QModelIndex()), (*it).second.from.time.toString());
		QLabel* desclabel = new QLabel(tr("<font size=\"4\">")+(*it).second.desc+tr("</font>"));
		table->setIndexWidget(model->index(row, 2, QModelIndex()), desclabel); 
		row++;
	}

	//note tab
	QGridLayout* LayoutNote = new QGridLayout(widgetNote);
	noteEdit = new QTextEdit;
	LayoutNote->addWidget(noteEdit, 0, 0, 9, 9);
	QString fileNameNote = QCoreApplication::applicationDirPath() + tr("\\") + tr(NOTE_FILE_NAME); 
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


	//tray setting
	createTrayIcon();

	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	trayIcon->setIcon(icon);
	trayIcon->show();

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
		resize(700, 300);
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
		int ret = QMessageBox::Yes;
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
	///todo show next task message
	setWindowFlags((Qt::WindowFlags)(~Qt::WindowStaysOnTopHint));
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
}

void zjCalendar::createTrayIcon()
{
	///to do add event, joblist, note 
	trayIconMenu = new QMenu(this);
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
		mainTab->setCurrentIndex(eventIdx);
		table->setFocus();
		trayIcon->showMessage(events.eventMap[id].desc,
			d+tr(" ")+t, 
			QSystemTrayIcon::Information, 
			30*1000);
		msg->setText(d+tr(" ")+t+tr(" <br><b>")+events.eventMap[id].desc+tr("</b>"));
		for (int idx = 0; idx < model->rowCount(); idx++)
		{
			if (id == model->data(model->index(idx, 0, QModelIndex()), Qt::DisplayRole))
			{
				table->setSelectionBehavior(QAbstractItemView::SelectRows);
				table->setCurrentIndex(model->index(idx, 0));
			}
		}
		showNormal();
	}
	else
	{
		time =  QTime::currentTime();
		t = time.toString("hh:mm:ss");
		msg->setText(d+tr(" ")+t);
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
