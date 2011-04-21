#include <QWidget>
#include <QTimeLine>
#include <QPainter>
class Digits: public QWidget
{
    Q_OBJECT
public:

    /*Define three transition modes of the digital clock*/
    enum {
        Slide,
        Flip,
        Rotate
    };

    Digits(QWidget *parent)
        : QWidget(parent)
        , m_number(0)
        , m_transition(Slide)
    {
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        //Widget paints all its pixels when it receives a paint event

        setAttribute(Qt::WA_NoSystemBackground, true);
        //Indicates that the widget has no background, i.e. when the widget receives paint events, the background is not automatically repainted.

        connect(&m_animator, SIGNAL(frameChanged(int)), SLOT(update()));
        //start animation

        m_animator.setFrameRange(0, 100);
        m_animator.setDuration(600);
        //Construct a 0.6-second timeline with a frame range of 0 - 100

        m_animator.setCurveShape(QTimeLine::EaseInOutCurve);
        //starts growing slowly, then runs steadily, then grows slowly again
    }

    /*Set transition when time changed*/
    void setTransition(int tr) {
        m_transition = tr;
    }

    /*Get transition mode*/
    int transition() const {
        return m_transition;
    }

    /*Set hours and minutes*/
    void setNumber(int n) {
        if (m_number != n) {
            m_number = qBound(0, n, 99);
            preparePixmap();
            update();
        }
    }

    /*Flip to next state*/
    void flipTo(int n) {
        if (m_number != n) {
            m_number = qBound(0, n, 99);
            m_lastPixmap = m_pixmap;
            preparePixmap();
            m_animator.stop();
            m_animator.start();
        }
    }

protected:

    /*Draw the main frame of the digits area*/
    void drawFrame(QPainter *p, const QRect &rect) {
        p->setPen(Qt::NoPen);
        QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
        //Set linear gradient area

        gradient.setColorAt(0.00, QColor(245, 245, 245));
        gradient.setColorAt(0.49, QColor(192, 192, 192));
        gradient.setColorAt(0.51, QColor(245, 245, 245));
        gradient.setColorAt(1.00, QColor(192, 192, 192));
        //Creates stop points at the given position with the given color

        p->setBrush(gradient);
        QRect r = rect;
        p->drawRoundedRect(r, 15, 15, Qt::RelativeSize);
        /*
            Draws outer rectangle rect with rounded corners.
            Qt::RelativeSize specifies the size relative to the bounding rectangle,
            typically using percentage measurements.
        */

        r.adjust(1, 4, -1, -4);
        //Adds 1, 4, -1 and -4 respectively to the existing coordinates of the rectangle

        p->setPen(QColor(181, 181, 181));
        p->setBrush(Qt::NoBrush);
        p->drawRoundedRect(r, 15, 15, Qt::RelativeSize);
        //Draws inner rectangle rect with rounded corners.

        p->setPen(QColor(159, 159, 159));
        int y = rect.top() + rect.height() / 2 - 1;
        p->drawLine(rect.left(), y, rect.right(), y);
        //Draws the mid-line from (rect.left(), y) to (rect.right(), y) and sets the current pen position to (rect.right(), y)

    }

    /*Draw the digits*/
    QPixmap drawDigits(int n, const QRect &rect) {

        int scaleFactor = 2;
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
        if (rect.height() > 240)
            scaleFactor = 1;
#endif

        QString str = QString::number(n);
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

        gradient.setColorAt(0.00, QColor(128, 128, 128));
        gradient.setColorAt(0.49, QColor(64, 64, 64));
        gradient.setColorAt(0.51, QColor(128, 128, 128));
        gradient.setColorAt(1.00, QColor(16, 16, 16));
        //Creates stop points at the given position with the given color

        QPainter p;
        p.begin(&pixmap);
        p.setFont(font);
        QPen pen;
        pen.setBrush(QBrush(gradient));
        //Set penbrush with linergrident

        p.setPen(pen);
        p.drawText(pixmap.rect(), Qt::AlignCenter, str);
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
        p.drawPixmap(0, 0, drawDigits(m_number, rect()));
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
        //Fill the widget rec with black color

        int pad = width() / 10;
        drawFrame(&p, rect().adjusted(pad, pad, -pad, -pad));
        p.drawPixmap(0, 0, m_pixmap);
    }

    /*Paint the slide state*/
    void paintSlide() {
        QPainter p(this);
        p.fillRect(rect(), Qt::white);

        int pad = width() / 10;
        QRect fr = rect().adjusted(pad, pad, -pad, -pad);
        drawFrame(&p, fr);
        p.setClipRect(fr);
        //sets the clip region to the given rectangle using the given clip operation

        int y = height() * m_animator.currentFrame() / 100;
        p.drawPixmap(0, y, m_lastPixmap);
        //Draw last-time state pixmap from 0 to height()(Y Coordinate)  in 0.6 second

        p.drawPixmap(0, y - height(), m_pixmap);
        //Draw current-time state pixmap from -height() to 0 (Y Coordinate) in 0.6 second
    }

    /*Paint the flip state*/
    void paintFlip() {
        QPainter p(this);
#if !defined(Q_OS_SYMBIAN) && !defined(Q_OS_WINCE_WM)
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.setRenderHint(QPainter::Antialiasing, true);
#endif
        p.fillRect(rect(), Qt::white);

        int hw = width() / 2;
        int hh = height() / 2;

        // behind is the new pixmap
        int pad = width() / 10;
        QRect fr = rect().adjusted(pad, pad, -pad, -pad);
        drawFrame(&p, fr);
        p.drawPixmap(0, 0, m_pixmap);

        int index = m_animator.currentFrame();

        if (index <= 50) {

            // the top part of the old pixmap is flipping
            int angle = -180 * index / 100;
            QTransform transform;
            transform.translate(hw, hh);
            //Moves the coordinate system to the center of widget

            transform.rotate(angle, Qt::XAxis);
            //Rotates the coordinate system counterclockwise by angle about the X axis

            p.setTransform(transform);
            drawFrame(&p, fr.adjusted(-hw, -hh, -hw, -hh));
            p.drawPixmap(-hw, -hh, m_lastPixmap);

            // the bottom part is still the old pixmap
            p.resetTransform();
            p.setClipRect(0, hh, width(), hh);
            //Enables clipping, and sets the clip region to the rectangle beginning at (0, hh) with the given width and height

            drawFrame(&p, fr);
            p.drawPixmap(0, 0, m_lastPixmap);
        } else {

            p.setClipRect(0, hh, width(), hh);

            // the bottom part is still the old pixmap
            drawFrame(&p, fr);
            p.drawPixmap(0, 0, m_lastPixmap);

            // the bottom part of the new pixmap is flipping
            int angle = 180 - 180 * m_animator.currentFrame() / 100;
            QTransform transform;
            transform.translate(hw, hh);
            transform.rotate(angle, Qt::XAxis);
            p.setTransform(transform);
            drawFrame(&p, fr.adjusted(-hw, -hh, -hw, -hh));
            p.drawPixmap(-hw, -hh, m_pixmap);

        }

    }

    /*Paint the rotate state*/
    void paintRotate() {
        QPainter p(this);

        int pad = width() / 10;
        QRect fr = rect().adjusted(pad, pad, -pad, -pad);
        drawFrame(&p, fr);
        p.setClipRect(fr);

        int angle1 = -180 * m_animator.currentFrame() / 100;
        int angle2 = 180 - 180 * m_animator.currentFrame() / 100;
        int angle = (m_animator.currentFrame() <= 50) ? angle1 : angle2;
        QPixmap pix = (m_animator.currentFrame() <= 50) ? m_lastPixmap : m_pixmap;

        QTransform transform;
        transform.translate(width() / 2, height() / 2);
        transform.rotate(angle, Qt::XAxis);

        p.setTransform(transform);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.drawPixmap(-width() / 2, -height() / 2, pix);
    }

    void paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        if (m_animator.state() == QTimeLine::Running) {
            if (m_transition == Slide)
                paintSlide();
            if (m_transition == Flip)
                paintFlip();
            if (m_transition == Rotate)
                paintRotate();
        } else {
            paintStatic();
        }
    }

private:
    int m_number;//number to set to digits

    int m_transition;//transition mode(change effect)

    QPixmap m_pixmap;//current time pixmap

    QPixmap m_lastPixmap;//next state time pixmap

    QTimeLine m_animator;
    //used to animate a GUI control by calling a slot periodically
    //The timeline's duration describes for how long the animation will run
    //connect the frameChanged() signal to a suitable slot in the widget you wish to animate
};

