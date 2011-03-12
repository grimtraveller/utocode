#include "datetime.h"

DateTime::DateTime()
{
	date = QDate::currentDate();
	time = QTime::currentTime();
	QString t = time.toString("hh:mm:00");
	time.fromString(t);
}

DateTime::DateTime(QDate d, QTime t)
{
	date = d;
	time = t;
}

DateTime::~DateTime()
{

}

void DateTime::getDateTime(QDate& d, QTime& t)
{
	d = date;
	t = time;
}

DateTime& DateTime::operator=(const DateTime& dt)
{
	date = dt.date;
	time = dt.time;
	return *this;
}