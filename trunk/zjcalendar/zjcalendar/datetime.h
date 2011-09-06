/**
 * @file	datetime.h 
 * @brief	
 * @details	
 * @author	zuohaitao
 * @date	2011-03-19 
 * @warning	
 * @bug	
 */
#ifndef DATETIME_H
#define DATETIME_H
#include <QDate>
#include <QTime>
class DateTime
{
public:
	DateTime();
	DateTime(QDate d, QTime t);
	~DateTime();
	void getDateTime(QDate& d, QTime& t);
	DateTime& operator=(const DateTime& dt);

	QDate date;
	QTime time;
private:
};
#endif //DATETIME_H
