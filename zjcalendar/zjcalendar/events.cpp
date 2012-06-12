/**
 * @file	events.cpp 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#include <events.h>
#include <QFile>
#include <QStringList>
Event::Event()
{
}

Event::~Event()
{
}

Event& Event::operator=(const Event& e)
{
	id = e.id;
	from = e.from;
	to = e.to;
	desc = e.desc;
	return *this;
}

Events::Events()
{
}

Events::~Events()
{
}


bool Events::exist(int id)
{
	return (eventMap.find(id) != eventMap.end());
}
int Events::addEvent()
{
	Event new_event;
	new_event.id = eventMap.size();
	eventMap[new_event.id] = new_event;
	return new_event.id;
}

bool Events::editEvent(int id, DateTime from, DateTime to, QString desc)
{
	if (!exist(id))
	{
		return false;
	}
	eventMap[id].from = from;
	eventMap[id].to = to;
	eventMap[id].desc = desc;
	return true;
}

Event* Events::findEvent(int id, Event& e)
{
	if (!exist(id)) 
	{
		return NULL;
	}
	e = eventMap[id];
	return &e;
}

bool Events::delEvent(int id)
{
	if (!exist(id))
	{
		return false;
	}
	eventMap.erase(eventMap.find(id));
	return true;
}

bool Events::getEventsFromFile(QString filename)
{
	QFile file(filename);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	 {
         return false;
	 }

     while (!file.atEnd()) 
	 {
		 Event e;
         QByteArray line = file.readLine();
         line2event(line, e);
		 eventMap[e.id] = e;
     }
	 return true;
}

void Events::line2event(QString line, Event& e)
{
	QStringList list = line.split('\t');
	e.id = list[0].toInt();
	e.from.time = QTime::fromString(list[1], "hh:mm:ss");
	e.desc = list[2];
}

unsigned int Events::haveEvent(DateTime dt)
{
	std::map<int, Event>::iterator it;
	for (it = eventMap.begin(); it != eventMap.end(); it++)
	{
		if ((*it).second.from.time == dt.time)
		 {
			 return (*it).second.id;
		 }
	}
	return -1;
}

unsigned int Events::size()
{
	return eventMap.size();
}
