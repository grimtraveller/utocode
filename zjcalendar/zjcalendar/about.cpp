/**
 @file		about.cpp
 @brief		
 @details	
 @author	zuohaitao
 @date		2012-06-08
 warning	
 bug		
**/

#include "about.h"
//#include "zjcalendar.h"
//#include "macro.h"
#define VERSION_INFORMATION	"	\
<br />	\
<b>update</b><br />	\
2012.06.08 Add \"Edit With Vim\" button<br />	\
2012.06.08 Add \"About\" dialog For show update information and todo list<br />	\
<b>todo</b><br />	\
<font color=\"red\">	\
Add Configure dialog<br />	\
\tSetting DIGFLIP type<br />	\
\tSetting GVIM PATH<br />	\
Reconstruct class event<br />	\
</font>	\
<br />	\
"
 
About::About(QWidget* parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{	
	//remove [?], add [_].
	flags = Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	//flags |= Qt::SplashScreen;
	//flags |= Qt::Popup;
	setWindowFlags(flags);
	setWindowTitle(tr("About zjcalendar"));

	widget = new QWidget(this);
	QGridLayout* layout = new QGridLayout(widget);
	setLayout(layout);
	info = new QLabel(tr(VERSION_INFORMATION));
	layout->addWidget(info);
}

About::~About()
{
}