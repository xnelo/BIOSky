/**
* @file Date.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Creates a Date class that will handle all date and date related operations.
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

#ifndef ___BIOENGINE_DATE_H__2014___
#define ___BIOENGINE_DATE_H__2014___

#include "CompileConfig.h"

namespace BIO
{
	enum DATE_MONTH
	{
		JANUARY = 0,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};

	class Date
	{
	private:
		DATE_MONTH _month;/**The month for this date.*/
		unsigned int _day;/**The day of the month.*/
		unsigned int _year;/**The year for this date.*/
		unsigned int _dayOfTheYear;/**The day of the year. Between 1-366.*/

		/**
		* Make A deep copy of passed in class.
		*/
		BIOSKY_API void _DeepCopy(const Date & other);

		/**
		* Private function to calculate the number of days
		* there are before the first of the month passed in.
		*/
		BIOSKY_API static unsigned int _NumDaysBefore(DATE_MONTH month, unsigned int year);
	public:
		/**
		* Default constructor
		*/
		BIOSKY_API Date();

		/**
		* Constructor
		*
		* @param month The month of the year for the new date.
		*
		* @param day the day of the month for the new date.
		*
		* @param year The Year of the new date.
		*/
		BIOSKY_API Date(DATE_MONTH month, unsigned int day, unsigned int year);

		/**
		* Copy Constructor
		*/
		BIOSKY_API Date(const Date & other);

		/**
		* Destructor
		*/
		BIOSKY_API ~Date();

		/**
		* Assignment operator
		*/
		BIOSKY_API Date & operator = (const Date & other);

		/**
		* Equality operator. Returns true if both dates are equal.
		*/
		BIOSKY_API bool operator == (const Date & other) const;

		/**
		* Inequality operator. Returns true if dates are not equal.
		*/
		BIOSKY_API bool operator != (const Date & other) const;

		/**
		* Pre Increments the date by one day.
		*/
		BIOSKY_API Date & operator ++ ();

		/**
		* Post Increments the date by one day.
		*/
		BIOSKY_API Date operator ++ (int);

		/**
		* Calculate the day of the year. The day of the year starts with Jan 1
		* being day 1 and December 31 being 365 (366 on leap year).
		*
		* @param month The month of the year.
		* @param day The day of the month.
		* @param year The year. This is used to determine if it is a leap year.
		*
		* @return Returns the day of the year.
		*/
		BIOSKY_API static unsigned int CalculateDayOfYear(DATE_MONTH month, unsigned int day, unsigned int year);

		/**
		* Return the number of days in a month.
		*
		* @param month The month of the year.
		*
		* @param year The year.
		*
		* @return Returns the number of days in a month.
		*/
		BIOSKY_API static unsigned int DaysInMonth(DATE_MONTH month, unsigned int year);

		/**
		* Get the Day of the month.
		*
		* @return Returns an unsigned int with the day of the month.
		*/
		BIOSKY_API unsigned int GetDay();

		/**
		* Get what day of the year it is. The returned value will be in the
		* range of 1-366 where Jan 1 == 1, Jan 2 == 2 ect.
		*
		* @return Returns and unsigned int with the day of the year.
		*/
		BIOSKY_API unsigned int GetDayOfTheYear();

		/**
		* Get the Month.
		*
		* @return Returns a 'DATE_MONTH' enumeration with the month.
		*/
		BIOSKY_API DATE_MONTH GetMonth();

		/**
		* Get the Year.
		*
		* @return Returns an unsigned int with the year.
		*/
		BIOSKY_API unsigned int GetYear();

		/**
		* Takes an integer and converts it to the DATE_MONTH enumeration. The 
		* passed in value must be between [1,12]. Otherwise JANUARY is 
		* returned.
		*
		* @param month The integer representation of the month.
		*
		* @return Returns the DATE_MONTH corrosponding to the passed in month 
		*			integer. If the passed in value is out of the range [1,12]
		*			then JANUARY is returned.
		*/
		BIOSKY_API static DATE_MONTH IntToMonth(int month);

		/**
		* Determine if the passed in year is a leap year.
		*
		* @param year The year.
		*
		* @return Returns if the year is a leap year.
		*/
		BIOSKY_API static bool IsLeapYear(unsigned int year);

		/**
		* Get the month as an integer. Jan = 1; Feb = 2; ect.
		*
		* @param month The month to get the integer for.
		*
		* @return Return the integer of that month.
		*/
		BIOSKY_API static int MonthToInt(DATE_MONTH month);

		/**
		* Set the date.
		*
		* @param month The month for the new date.
		*
		* @param day The day for the new date.
		*
		* @param year The year for the new date.
		*/
		BIOSKY_API void SetDate(DATE_MONTH month, unsigned int day, unsigned int year);

		/**
		* Set the Day of the month.
		*
		* @param day The new day of the month. This must be between 1-366.
		*/
		BIOSKY_API void SetDay(unsigned int day);

		/**
		* Set the Month of the year.
		*
		* @param month The new month of the year.
		*/
		BIOSKY_API void SetMonth(DATE_MONTH month);

		/**
		* Set the year.
		*
		* @param year. The year for this date.
		*/
		BIOSKY_API void SetYear(unsigned int year);

		/**
		* Returns the 'DATE_MONTH' enumeration corrosponding to the passed in 
		* string version of the month.
		*
		* @param month A C-String with the month in it.
		*
		* @return Returns a 'DATE_MONTH' enumeration of the passed in string. 
		*			If the passed in string is not recognized then JANUARY is 
		*			returned.
		*/
		BIOSKY_API static DATE_MONTH StringToMonth(char * month);
	};

}//end namespace BIO

inline BIO::Date::Date() : _month(JANUARY), _day(1), _year(2000), _dayOfTheYear(1)
{}

inline BIO::Date::Date(DATE_MONTH month, unsigned int day, unsigned int year) : _month(JANUARY), _day(1), _year(2000), _dayOfTheYear(1)
{
	SetDate(month, day, year);
}

inline BIO::Date::Date(const Date & other) : _month(JANUARY), _day(1), _year(2000), _dayOfTheYear(1)
{
	_DeepCopy(other);
}

inline BIO::Date::~Date()
{}

inline BIO::Date & BIO::Date::operator=(const Date & other)
{
	if (this != &other)
		_DeepCopy(other);

	return *this;
}

inline bool BIO::Date::operator==(const Date & other) const
{
	return ((_month == other._month) &&
			(_day == other._day) &&
			(_year == other._year));
}

inline bool BIO::Date::operator != (const Date & other) const
{
	return !(*this == other);
}

inline unsigned int BIO::Date::CalculateDayOfYear(DATE_MONTH month, unsigned int day, unsigned int year)
{
	return _NumDaysBefore(month, year) + day;
}

inline unsigned int BIO::Date::GetDay()
{
	return _day;
}

inline unsigned int BIO::Date::GetDayOfTheYear()
{
	return _dayOfTheYear;
}

inline BIO::DATE_MONTH BIO::Date::GetMonth()
{
	return _month;
}

inline unsigned int BIO::Date::GetYear()
{
	return _year;
}

inline int BIO::Date::MonthToInt(DATE_MONTH month)
{
	switch (month)
	{
	case BIO::JANUARY:
		return 1;
	case BIO::FEBRUARY:
		return 2;
	case BIO::MARCH:
		return 3;
	case BIO::APRIL:
		return 4;
	case BIO::MAY:
		return 5;
	case BIO::JUNE:
		return 6;
	case BIO::JULY:
		return 7;
	case BIO::AUGUST:
		return 8;
	case BIO::SEPTEMBER:
		return 9;
	case BIO::OCTOBER:
		return 10;
	case BIO::NOVEMBER:
		return 11;
	case BIO::DECEMBER:
		return 12;
	default:
		return 1;
	}
}

inline void BIO::Date::SetDay(unsigned int day)
{
	SetDate(_month, day, _year);
}

inline void BIO::Date::SetMonth(DATE_MONTH month)
{
	SetDate(month, _day, _year);
}

inline void BIO::Date::SetYear(unsigned int year)
{
	SetDate(_month, _day, year);
}

#endif //___BIOENGINE_DATE_H__2014___