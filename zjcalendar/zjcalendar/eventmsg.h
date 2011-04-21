#ifndef EVENTMSG_H
#define EVENTMSG_H

#include <QWidget>
#include <QPainter>
class EventMsg : public QWidget
{
	Q_OBJECT

public:
	EventMsg(QWidget *parent)
        : QWidget(parent)
	{
		 setAttribute(Qt::WA_OpaquePaintEvent, true);
        //Widget paints all its pixels when it receives a paint event

        setAttribute(Qt::WA_NoSystemBackground, true);
        //Indicates that the widget has no background, i.e. when the widget receives paint events, the background is not automatically repainted.
	}
	/*Set hours and minutes*/
	void setEventMsg(QString msg) 
	{
        if (m_msg != msg) 
		{
			m_msg = msg;
			preparePixmap();
            update();
        }
    }
protected:
   /*Draw the main frame of the digits area*/
    void drawFrame(QPainter *p, const QRect &rect) 
	{
  //      p->setPen(Qt::NoPen);

		//QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
  //      //Set linear gradient area

  //      gradient.setColorAt(0.00, QColor(245, 245, 245));
  //      gradient.setColorAt(0.49, QColor(192, 192, 192));
  //      gradient.setColorAt(0.51, QColor(245, 245, 245));
  //      gradient.setColorAt(1.00, QColor(192, 192, 192));
  //      //Creates stop points at the given position with the given color

  //      p->setBrush(gradient);
		  QRect r = rect;
  //      p->drawRoundedRect(r, 0, 0, Qt::RelativeSize);
  //      /*
  //          Draws outer rectangle rect with rounded corners.
  //          Qt::RelativeSize specifies the size relative to the bounding rectangle,
  //          typically using percentage measurements.
  //      */

        r.adjust(1, 4, -1, -4);
        //Adds 1, 4, -1 and -4 respectively to the existing coordinates of the rectangle

        p->setPen(QColor(181, 181, 181));
        p->setBrush(Qt::NoBrush);
        p->drawRoundedRect(r, 0, 0, Qt::RelativeSize);
        //Draws inner rectangle rect with rounded corners.

  //      p->setPen(QColor(159, 159, 159));
  //      int y = rect.top() + rect.height() / 2 - 1;
  //      p->drawLine(rect.left(), y, rect.right(), y);
  //     //Draws the mid-line from (rect.left(), y) to (rect.right(), y) and sets the current pen position to (rect.right(), y)

    }

   /*Draw the digits*/
    QPixmap drawMsg(QString msg, const QRect &rect) 
	{

        int scaleFactor = 2;
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
        if (rect.height() > 240)
            scaleFactor = 1;
#endif

        QString str = msg;
        if (str.length() == 1)
            str.prepend("0");
        //Ensure it is double-digit

        QFont font;
        font.setFamily("Helvetica");
        int fontHeight = scaleFactor * 0.55 * rect.height();
        font.setPixelSize(fontHeight);
        //Sets the font size to pixelSize pixels

        font.setBold(true);

        QPixmap pixmap(rect.size() * scaleFactor);
        pixmap.fill(Qt::transparent);
  
		QLinearGradient gradient(QPoint(0, 0), QPoint(0, pixmap.height()));
        //Constructs a linear gradient with interpolation area between (0,0) and (0,pixmap.height())

        gradient.setColorAt(0.00, QColor(212, 72, 66));
		gradient.setColorAt(1.00, QColor(230, 148, 148));
        //Creates stop points at the given position with the given color

  
        QPainter p;
        p.begin(&pixmap);
        p.setFont(font);
        QPen pen;
        pen.setBrush(QBrush(gradient));
        //Set penbrush with linergrident

        p.setPen(pen);
        p.drawText(pixmap.rect(), Qt::AlignHCenter, str);
        //Draws the digit number(str here) within the provided rectangle

        p.end();

        return pixmap.scaledToWidth(width(), Qt::SmoothTransformation);
        //Returns a scaled copy of the image which is transformed using bilinear filtering
    }
   /*prepare the pixmap  */
    void preparePixmap() {
        m_pixmap = QPixmap(size());
        m_pixmap.fill(Qt::transparent);
        //Fills the pixmap with the given transparent black value (i.e., QColor(0, 0, 0, 0))

        QPainter p;
        p.begin(&m_pixmap);
        p.drawPixmap(0, 0, drawMsg(m_msg, rect()));
        //Draws the given digits-pixmap at position (0, 0)

        p.end();
    }

    /*define a resize event*/
    void resizeEvent(QResizeEvent*) {
        preparePixmap();
        update();//Causes a paintEvent() call

    }

    /*Paint the static state*/
    void paintStatic() {
        QPainter p(this);
        p.fillRect(rect(), Qt::white);
        p.drawPixmap(0, 0, m_pixmap);
    }

	void paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
		paintStatic();
    }
private:
	QString m_msg;

	QPixmap m_pixmap;//current time pixmap


private:
	
};

#endif // EVENTMSG_H
