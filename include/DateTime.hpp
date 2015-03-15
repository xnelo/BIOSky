/**
* @file DateTime.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines a class that handles both date and time.
*/
/*
* The zlib/libpng License
*
* Copyright (c) 2014 Spencer Hoffa
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from the
* use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
*		1. The origin of this software must not be misrepresented; you must not
*		claim that you wrote the original software. If you use this software in
*		a product, an acknowledgment in the product documentation would be
*		appreciated but is not required.
*
*		2. Altered source versions must be plainly marked as such, and must not
*		be misrepresented as being the original software.
*
*		3. This notice may not be removed or altered from any source
*		distribution.
*
* This liscense can also be found at: http://opensource.org/licenses/Zlib
*/

#ifndef ___BIOENGINE_DATETIME_H__2014___
#define ___BIOENGINE_DATETIME_H__2014___

#include "CompileConfig.h"

#include "Date.hpp"

namespace BIO
{
	class DateTime
	{
	private:
		Date _date;/**The date component of the Date and Time.*/
		float _time;/**The Time component of the Date and Time.*/
		float _UTCOffset;/**The offest from greenwich mean time(UTC 0).*/

		/**
		* Creates a Deep copy of the passed in class.
		*
		* @param other The class to copy.
		*/
		BIOSKY_API void _DeepCopy(const DateTime & other);
	public:
		/**
		* Default Constructor
		*/
		BIOSKY_API DateTime();

		/**
		* Copy Constructor
		*/
		BIOSKY_API DateTime(const DateTime & other);

		/**
		* Destructor
		*/
		BIOSKY_API ~DateTime();

		/**
		* Assignment operator.
		*/
		BIOSKY_API DateTime & operator = (const DateTime & other);

		/**
		* Equality operator.
		*/
		BIOSKY_API bool operator == (const DateTime & other) const;

		/**
		* Inequality operator
		*/
		BIOSKY_API bool operator != (const DateTime & other) const;

		/**
		* Add Time to the date in this class.
		*
		* @param timeToAdd A float number representing the amount of time to 
		*			add in seconds.
		*/
		BIOSKY_API void AddTime(float timeToAdd);

		/**
		* Get the date stored in this class.
		*
		* @return Returns a @see BIO::Date class object with the stored date.
		*/
		BIOSKY_API Date GetDate();

		/**
		* Get the day of the month.
		*
		* @return Returns an unsigned int representing the day of the month.
		*/
		BIOSKY_API unsigned int GetDay();

		/**
		* Get the day of the year. This value is within the range 1 to 366 and 
		* starts with Jan 1 == 1, Jan 2 == 2 and December 31 == 365(or 366 on a 
		* leap year).
		*
		* @return Returns an unsigned int with the day of the year.
		*/
		BIOSKY_API unsigned int GetDayOfYear();

		/**
		* Get the Month.
		*
		* @return Returns the month of the year.
		*/
		BIOSKY_API DATE_MONTH GetMonth();

		/**
		* Get the TIme. The returned value will be in seconds.
		*
		* @return Returns a float value with the time in seconds.
		*/
		BIOSKY_API float GetTime();

		/**
		* Get the Time. The returned value will be in decimal 24hr format. This
		* means that 3:24 AM will be 3.4 and 3:24 PM will be 15.4.
		*
		* @return Returns a float value with the time in decimal 24hr format.
		*/
		BIOSKY_API float GetTimeHours();

		/**
		* Get the amount of time difference from UTC 0 (Greenwich Mean Time).
		* This should be the standard timezones UTC offset.
		*
		* @return Returns the difference from UTC 0.
		*/
		BIOSKY_API float GetUTCOffset();

		/**
		* Get the Year.
		*
		* @return Returns an unsigned int representing the year of this date.
		*/
		BIOSKY_API unsigned int GetYear();

		/**
		* Set the Date.
		*
		* @param newDate This is a @see BIO::Date object that contains the new 
		*			date to be set.
		*/
		BIOSKY_API void SetDate(Date newDate);

		/**
		* Set the Date.
		*
		* @param month The month of the year to set the date to.
		*
		* @param day The day of the month to set the date to.
		*
		* @param year The year to set the date to.
		*/
		BIOSKY_API void SetDate(DATE_MONTH month, unsigned int day, unsigned int year);
		
		/**
		* Set the day of the mont.
		*
		* @param newDay The day of the month to set the date to.
		*/
		BIOSKY_API void SetDay(unsigned int newDay);

		/**
		* Set the month of the year.
		*
		* @param newMonth The month of the year to set the date to.
		*/
		BIOSKY_API void SetMonth(DATE_MONTH newMonth);

		/**
		* Set the amount of time difference from UTC 0 (Greenwich Mean Time).
		* This should be the standard timezones UTC offset. The passed in value
		* should be between [-12,+12], if it is not then this function will cap
		* it at the nearest value -12, or +12.
		*
		* @pram offset The amount of offset from UTC 0
		*/
		BIOSKY_API void SetUTCOffset(float offset);

		/**
		* Set the Year.
		*
		* @param newYear The year to set the date to.
		*/
		BIOSKY_API void SetYear(unsigned int newYear);

		/**
		* Set the time. The passed in value must be between the values of 
		* [0.0, 86400.0). This function will check to make sure the input 
		* values are between the specified interval. If the new value is not 
		* within the interval then the time will be set to 0.0f(Midnight).
		*
		* @note The value 86400.0f is the number of seconds in the day. This 
		*		value is obtained by the equation 24 hrs * 60 min * 60 sec.
		*
		* @param newTime The new time to set in seconds.
		*/
		BIOSKY_API void SetTime(float newTime);

		/**
		* Set the time. The passed in value must be between the values of
		* [0.0, 24.0). This function will check to make sure the input values 
		* in the class are between the specified interval. If the new value is 
		* not within the interval then the time will be set to 0.0f(Midnight).
		*
		* @param newTime The new time to set.
		*/
		BIOSKY_API void SetTimeHours(float newTime);

#if BIOSKY_TESTING == 1
		/**
		* Test this class.
		*
		* @param test A pointer to a Test class that holds all the function
		*				for testing and will hold all the results of the
		*				testing.
		*
		* @return Returns true iff all the tests pass.
		*/
		static bool Test(XNELO::TESTING::Test * test);
#endif

	};

}//end namespace BIO

inline BIO::DateTime::DateTime() : _date(), _time(0.0f), _UTCOffset(0.0f)
{}

inline BIO::DateTime::DateTime(const DateTime & other) : _date(), _time(), _UTCOffset(0.0f)
{
	_DeepCopy(other);
}

inline BIO::DateTime::~DateTime()
{}

inline BIO::DateTime & BIO::DateTime::operator = (const DateTime & other)
{
	if (this != &other)
		_DeepCopy(other);

	return *this;
}

inline bool BIO::DateTime::operator == (const DateTime & other) const
{
	return ((_date == other._date) && (_time == other._time) && (_UTCOffset == other._UTCOffset));
}

inline bool BIO::DateTime::operator != (const DateTime & other) const
{
	return !(*this == other);
}

inline BIO::Date BIO::DateTime::GetDate()
{
	return _date;
}

inline unsigned int BIO::DateTime::GetDay()
{
	return _date.GetDay();
}

inline unsigned int BIO::DateTime::GetDayOfYear()
{
	return _date.GetDayOfTheYear();
}

inline BIO::DATE_MONTH BIO::DateTime::GetMonth()
{
	return _date.GetMonth();
}

inline float BIO::DateTime::GetTime()
{
	return _time;
}

inline float BIO::DateTime::GetTimeHours()
{
	return _time / 3600.0f;
}

inline float BIO::DateTime::GetUTCOffset()
{
	return _UTCOffset;
}

inline unsigned int BIO::DateTime::GetYear()
{
	return _date.GetYear();
}

inline void BIO::DateTime::SetDate(Date newDate)
{
	_date = newDate;
}

inline void BIO::DateTime::SetDate(DATE_MONTH month, unsigned int day, unsigned int year)
{
	_date.SetDate(month, day, year);
}

inline void BIO::DateTime::SetDay(unsigned int newDay)
{
	_date.SetDay(newDay);
}

inline void BIO::DateTime::SetMonth(DATE_MONTH newMonth)
{
	_date.SetMonth(newMonth);
}

inline void BIO::DateTime::SetUTCOffset(float offset)
{
	if (offset > 12)
		offset = 12;

	if (offset < -12)
		offset = -12;

	_UTCOffset = offset;
}

inline void BIO::DateTime::SetYear(unsigned int newYear)
{
	_date.SetYear(newYear);
}

inline void BIO::DateTime::SetTimeHours(float newTime)
{
	SetTime(newTime * 3600.0f);
}

#endif //___BIOENGINE_DATETIME_H__2014___