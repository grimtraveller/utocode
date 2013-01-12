#include <QWidget>
#include <QResizeEvent>
#include <QBasicTimer>
#include <QString>
#include <QTime>
#include <QAction>
#include "digits.h"

class DigiFlip : public QWidget
{
    Q_OBJECT

public:
    DigiFlip(QWidget *parent = 0)
        : QWidget(parent)
    {
        m_hour = new Digits(this);
        m_hour->show();
        m_minute = new Digits(this);
        m_minute->show();
		m_sec = new Digits(this);
		m_sec->show();

        QPalette pal = palette();
        //pal.setColor(QPalette::Window, Qt::white);
        //Sets the color used for the given color role, in all color groups, to the specified solid color.

        setPalette(pal);

        m_ticker.start(1000, this);
        //Send a timer event every second

        QTime t = QTime::currentTime();
        m_hour->setNumber(t.hour());
        m_minute->setNumber(t.minute());
		m_sec->setNumber(t.second());
        updateTime();

        slideAction = new QAction("&Slide", this);
        flipAction = new QAction("&Flip", this);
        rotateAction = new QAction("&Rotate", this);
		slideAction->setCheckable(true);
		flipAction->setCheckable(true);
		rotateAction->setCheckable(true);
        connect(slideAction, SIGNAL(triggered()), SLOT(chooseSlide()));
        connect(flipAction, SIGNAL(triggered()), SLOT(chooseFlip()));
        connect(rotateAction, SIGNAL(triggered()), SLOT(chooseRotate()));
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
        menuBar()->addAction(slideAction);
        menuBar()->addAction(flipAction);
        menuBar()->addAction(rotateAction);
#else
        addAction(slideAction);
        addAction(flipAction);
        addAction(rotateAction);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        //Shows a context menu(right click)
#endif
    }

    /*Real-time updates*/
    void updateTime() {
        QTime t = QTime::currentTime();
        m_hour->flipTo(t.hour());
        m_minute->flipTo(t.minute());
		m_sec->flipTo(t.second());
        QString str = t.toString("hh:mm:ss");
        str.prepend(": ");
        if (m_hour->transition() == Digits::Slide)
            str.prepend("Slide");
        if (m_hour->transition() == Digits::Flip)
            str.prepend("Flip");
        if (m_hour->transition() == Digits::Rotate)
            str.prepend("Rotate");
        setWindowTitle(str);
    }

    /*Switch transition mode*/
    void switchTransition(int delta) {
        int i = (m_hour->transition() + delta + 3) % 3;
        m_hour->setTransition(i);
        m_minute->setTransition(i);
		m_sec->setTransition(i);
        updateTime();
    }

	void setTransition(int transition)
	{
		if (transition == 0 || transition == 1 || transition == 2)
		{
			_transition = transition;
			m_hour->setTransition(transition);
			m_minute->setTransition(transition);
			m_sec->setTransition(transition);

		}


	}
	int getTransition()
	{
		return _transition;
	}
protected:
    void resizeEvent(QResizeEvent*) {
        int digitsWidth = width() / 3;
        int digitsHeight = digitsWidth * 1.2;

        int y = (height() - digitsHeight) / 2;

        m_hour->resize(digitsWidth, digitsHeight);
        m_hour->move(0, y);

        m_minute->resize(digitsWidth, digitsHeight);
        m_minute->move(width() / 3, y);

		m_sec->resize(digitsWidth, digitsHeight);
        m_sec->move(width() * 2 / 3 - 1 , y);
    }

    /*Timer event,receive timer events */
    void timerEvent(QTimerEvent*) {
        updateTime();
    }

    /* Get key press event */
    void keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Right) {
            switchTransition(1);
            event->accept();
        }
        if (event->key() == Qt::Key_Left) {
            switchTransition(-1);
            event->accept();
        }
    }

private slots:
    void chooseSlide() {
		slideAction->setChecked(true);
		flipAction->setChecked(false);
		rotateAction->setChecked(false);

		_transition = 0;
        m_hour->setTransition(0);
        m_minute->setTransition(0);
		m_sec->setTransition(0);
        updateTime();
    }

    void chooseFlip() {
		slideAction->setChecked(false);
		rotateAction->setChecked(false);
		flipAction->setChecked(true);

		_transition = 1;
		m_hour->setTransition(1);
        m_minute->setTransition(1);
		m_sec->setTransition(1);
        updateTime();
    }

    void chooseRotate() {
		slideAction->setChecked(false);
		rotateAction->setChecked(true);
		flipAction->setChecked(false);

		_transition = 2;
        m_hour->setTransition(2);
        m_minute->setTransition(2);
		m_sec->setTransition(2);
        updateTime();
    }

private:
    QBasicTimer m_ticker;
    Digits *m_hour;
    Digits *m_minute;
	Digits *m_sec;
	int _transition;
	QAction * slideAction; 
	QAction * flipAction;
	QAction * rotateAction;

};