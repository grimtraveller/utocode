#include "zjcalendar.h"

zjCalendar::zjCalendar(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	timeid = startTimer(1000/**60*/);  // 60*1-second timer
	//createIconGroupBox();
	//createMessageGroupBox();

	//iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

	createActions();
	createTrayIcon();

	//connect(showMessageButton, SIGNAL(clicked()), this, SLOT(showMessage()));
	//connect(showIconCheckBox, SIGNAL(toggled(bool)),
	//        trayIcon, SLOT(setVisible(bool)));
	//connect(iconComboBox, SIGNAL(currentIndexChanged(int)),
	//        this, SLOT(setIcon(int)));
	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	//    QVBoxLayout *mainLayout = new QVBoxLayout;
	//    mainLayout->addWidget(iconGroupBox);
	//	 mainLayout->addWidget(messageGroupBox);
	//     setLayout(mainLayout);

	//     iconComboBox->setCurrentIndex(1);
	trayIcon->show();

	setWindowTitle(tr("Systray"));
	QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);

	trayIcon->setIcon(icon);
	setWindowIcon(icon);
	resize(400, 300);
	QString d;
	QDate date = QDate::currentDate();
	d = date.toString("yyyy-MM-dd");
	QTime time =  QTime::currentTime();
	QString t = time.toString("hh:mm:ss");
	msg = new QLabel(d+tr(" ")+t);
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(msg, 0, 0);
	setLayout(layout);
	QString filename = QCoreApplication::applicationDirPath() + tr("\\zjCalendar.dat"); 
	events.getEventsFromFile(filename);
/*
	events["08:30:00"] = tr("工作开始了，注意保持坐姿。");
	events["08:40:00"] = tr("打水泡茶啦。");
	events["09:00:00"] = tr("老婆早安。");
	events["09:30:00"] = tr("已经工作一个小时啦，起来活动活动。");
	events["10:00:00"] = tr("已经工作一个半小时了,打水泡茶。");
	events["11:30:00"] = tr("已经工作三个小时半了，午休时间快到了，提交代码，准备去吃午饭。");
	events["12:10:00"] = tr("午休时间，听听音乐，睡一觉，休息一下。");
	events["13:10:00"] = tr("下午工作开始了。");
	events["13:30:00"] = tr("老婆午安。");
	events["14:00:00"] = tr("工作四个小时了，续上一杯茶。");
	events["15:00:00"] = tr("工作五个个小时了。溜达溜达。");
	events["16:00:00"] = tr("工作六小时了。WC见。");
	events["17:00:00"] = tr("工作七个半小时了，提交代码，写周报。");
	events["17:10:00"] = tr("老婆晚上好。");
	events["17:20:00"] = tr("还有十分钟就下班了，注意提交代码，写周报。");
	events["17:30:00"] = tr("八小时工作已经完成，下班时间到。");
	events["19:40:00"] = tr("喝茶时间到。");
	events["22:00:00"] = tr("哄老婆睡觉时间到。");
	events["23:00:00"] = tr("编码时间到");
	events["23:10:00"] = tr("已经过去十分钟了，加油。");
	events["23:30:00"] = tr("已经过去半小时了");
	events["23:50:00"] = tr("提交代码，准备睡觉");
	events["00:00:00"] = tr("夜深了，注意睡眠");
	events["00:05:00"] = tr("夜深了，注意睡眠");
	events["00:10:00"] = tr("夜深了，注意睡眠");
	events["00:15:00"] = tr("夜深了，注意睡眠");
	events["00:20:00"] = tr("夜深了，注意睡眠");
	events["00:25:00"] = tr("夜深了，注意睡眠");
	events["00:30:00"] = tr("夜深了，注意睡眠");
	events["00:35:00"] = tr("夜深了，注意睡眠");
	events["00:40:00"] = tr("夜深了，注意睡眠");
	events["00:45:00"] = tr("夜深了，注意睡眠");
	events["00:50:00"] = tr("夜深了，注意睡眠");
	events["01:00:00"] = tr("夜深了，注意睡眠");
	events["01:05:00"] = tr("夜深了，注意睡眠");
	events["01:10:00"] = tr("夜深了，注意睡眠");
	events["01:15:00"] = tr("夜深了，注意睡眠");
	events["01:20:00"] = tr("夜深了，注意睡眠");
	events["01:25:00"] = tr("夜深了，注意睡眠");
	events["01:30:00"] = tr("夜深了，注意睡眠");
	events["01:35:00"] = tr("夜深了，注意睡眠");
	events["01:40:00"] = tr("夜深了，注意睡眠");
	events["01:45:00"] = tr("夜深了，注意睡眠");
*/
}

zjCalendar::~zjCalendar()
{
	killTimer(timeid);
}

void zjCalendar::setVisible(bool visible)
{
	minimizeAction->setEnabled(visible);
	maximizeAction->setEnabled(!isMaximized());
	restoreAction->setEnabled(isMaximized() || !visible);
	QDialog::setVisible(visible);
}

void zjCalendar::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) {
		/*
		QMessageBox::information(this, tr("Systray"),
		tr("The program will keep running in the "
		"system tray. To terminate the program, "
		"choose <b>Quit</b> in the context menu "
		"of the system tray entry."));
		*/
		hide();
		event->ignore();
	}
}

void zjCalendar::setIcon(int index)
{
	//QIcon icon = iconComboBox->itemIcon(index);
	QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);

	trayIcon->setIcon(icon);
	setWindowIcon(icon);

	trayIcon->setToolTip(iconComboBox->itemText(index));
}

void zjCalendar::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	 case QSystemTrayIcon::Trigger:
	 case QSystemTrayIcon::DoubleClick:
		 //iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1)
		 //                              % iconComboBox->count());
		 break;
	 case QSystemTrayIcon::MiddleClick:
		 showMessage();
		 break;
	 default:
		 ;
	}
}

void zjCalendar::showMessage()
{
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
		typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
	trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
		durationSpinBox->value() * 1000);
}

void zjCalendar::messageClicked()
{
	/*     QMessageBox::information(0, tr("Systray"),
	tr("Sorry, I already gave what help I could.\n"
	"Maybe you should try asking a human?"));
	*/
}

void zjCalendar::createIconGroupBox()
{
	iconGroupBox = new QGroupBox(tr("Tray Icon"));

	iconLabel = new QLabel("Icon:");

	iconComboBox = new QComboBox;
	iconComboBox->addItem(QIcon(":/images/bad.svg"), tr("Bad"));
	iconComboBox->addItem(QIcon(":/images/heart.svg"), tr("Heart"));
	iconComboBox->addItem(QIcon(":/images/trash.svg"), tr("Trash"));
	iconComboBox->addItem(QIcon(":/images/icon.svg"), tr("Icon"));

	showIconCheckBox = new QCheckBox(tr("Show icon"));
	showIconCheckBox->setChecked(true);

	QHBoxLayout *iconLayout = new QHBoxLayout;
	iconLayout->addWidget(iconLabel);
	iconLayout->addWidget(iconComboBox);
	iconLayout->addStretch();
	iconLayout->addWidget(showIconCheckBox);
	iconGroupBox->setLayout(iconLayout);
}

void zjCalendar::createMessageGroupBox()
{

	messageGroupBox = new QGroupBox(tr("Balloon Message"));

	typeLabel = new QLabel(tr("Type:"));

	typeComboBox = new QComboBox;
	typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
	typeComboBox->addItem(style()->standardIcon(
		QStyle::SP_MessageBoxInformation), tr("Information"),
		QSystemTrayIcon::Information);
	typeComboBox->addItem(style()->standardIcon(
		QStyle::SP_MessageBoxWarning), tr("Warning"),
		QSystemTrayIcon::Warning);
	typeComboBox->addItem(style()->standardIcon(
		QStyle::SP_MessageBoxCritical), tr("Critical"),
		QSystemTrayIcon::Critical);
	typeComboBox->setCurrentIndex(1);

	durationLabel = new QLabel(tr("Duration:"));

	durationSpinBox = new QSpinBox;
	durationSpinBox->setRange(5, 60);
	durationSpinBox->setSuffix(" s");
	durationSpinBox->setValue(15);

	durationWarningLabel = new QLabel(tr("(some systems might ignore this "
		"hint)"));
	durationWarningLabel->setIndent(10);

	titleLabel = new QLabel(tr("Title:"));

	titleEdit = new QLineEdit(tr("Cannot connect to network"));

	bodyLabel = new QLabel(tr("Body:"));

	bodyEdit = new QTextEdit;
	bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
		"clue.\nClick this balloon for details."));

	showMessageButton = new QPushButton(tr("Show Message"));
	showMessageButton->setDefault(true);

	QGridLayout *messageLayout = new QGridLayout;
	messageLayout->addWidget(typeLabel, 0, 0);
	messageLayout->addWidget(typeComboBox, 0, 1, 1, 2);
	messageLayout->addWidget(durationLabel, 1, 0);
	messageLayout->addWidget(durationSpinBox, 1, 1);
	messageLayout->addWidget(durationWarningLabel, 1, 2, 1, 3);
	messageLayout->addWidget(titleLabel, 2, 0);
	messageLayout->addWidget(titleEdit, 2, 1, 1, 4);
	messageLayout->addWidget(bodyLabel, 3, 0);
	messageLayout->addWidget(bodyEdit, 3, 1, 2, 4);
	messageLayout->addWidget(showMessageButton, 5, 4);
	messageLayout->setColumnStretch(3, 1);
	messageLayout->setRowStretch(4, 1);
	messageGroupBox->setLayout(messageLayout);

}

void zjCalendar::createActions()
{
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	maximizeAction = new QAction(tr("Ma&ximize"), this);
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void zjCalendar::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
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
	//if (events.find(t) != events.end() )
	{
	//	QMessageBox::information(0, 
	//								QObject::tr("Systray"),
	//				 =				str);

	//	trayIcon->showMessage(events[t], d+tr(" ")+t, QSystemTrayIcon::Information,
	//		30*1000);

		//msg->setText(d+tr(" ")+t+tr(" <br><b>")+events[t]+tr("</b>"));	 
	//	show();
		trayIcon->showMessage(events.eventMap[id].desc, d+tr(" ")+t, QSystemTrayIcon::Information, 30*1000);
		msg->setText(d+tr(" ")+t+tr(" <br><b>")+events.eventMap[id].desc+tr("</b>"));
		show();

	}
	else
	{
		time =  QTime::currentTime();
		t = time.toString("hh:mm:ss");
		msg->setText(d+tr(" ")+t);
	}
}