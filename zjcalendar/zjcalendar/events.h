/**
 * @file	events.h 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#ifndef EVENTS_H
#define EVENTS_H
#include "datetime.h"
#include <QString>
#include <map>
class Event
{
public:
	Event();
	~Event();
	Event& operator=(const Event& e);
	unsigned int id;
	DateTime from;
	DateTime to;
	QString desc;
};
class Events
{
public:
	typedef enum TYPEE
	{
		FILE = 0,
		SQLITE = 1
	}TypeE;
	Events();
	~Events();
	int addEvent();
	bool editEvent(int id, DateTime from, DateTime to, QString desc);
	bool delEvent(int id);
	Event* findEvent(int id, Event& e);
	bool getEventsFromFile(QString file);
	bool flushEventsToFile();
	unsigned int haveEvent(DateTime dt);
	unsigned int size();
/*private:*/
	bool exist(int id);
	void line2event(QString line, Event& e);
	std::map<int, Event> eventMap;
};
#endif //EVENTS_H
