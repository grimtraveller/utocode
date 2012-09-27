#ifndef ZJTEXTEDIT_H
#define ZJTEXTEDIT_H

#include <QtGui>

class zjTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	zjTextEdit( QWidget* parent = 0);

	virtual void inputMethodEvent(QInputMethodEvent* e);
};
#endif //ZJTEXTEDIT_H