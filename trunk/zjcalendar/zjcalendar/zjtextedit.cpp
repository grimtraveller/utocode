#include "zjtextedit.h"

zjTextEdit::zjTextEdit(QWidget* parent)
:QTextEdit(parent)
{

};

void zjTextEdit::inputMethodEvent(QInputMethodEvent* e)
{
	//HACK: the cursor is lost, when input by Google inputmethod 
	for (int i = 0; i < e->attributes().size(); ++i) {
		const QInputMethodEvent::Attribute &a = e->attributes().at(i);
		if (a.type == QInputMethodEvent::Cursor) {
			(*(int*)(&(a.start))) = 0;
		}
	}
	QTextEdit::inputMethodEvent(e);
};