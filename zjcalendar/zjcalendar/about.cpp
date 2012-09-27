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
2012.06.11 Play sound when event happens.<br />	\
2012.09.16 Add win+left win+right win+up win+down gloable hot key for move window<br />	\
2012.09.16 minize and message clicked to hide() and no show dialog until next tip <br />\
2012.09.27 HACK: the cursor is lost, when input by Google inputmethod  <br />\
<b>todo</b><br />	\
<font color=\"red\">	\
Add Configure dialog<br />	\
\tSetting DIGFLIP type<br />	\
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