/**
* @file Date.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implements the Date class.
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

#include "Date.hpp"

#include <cctype>//access tolower function
/**
* String Compare Case Insensitive. Compare two c-strings to see
* if they are the same without case sensitivity. A == a in this
* function. This function checks the entire string and only stops
* when the null terminating('\0') character is reached.
*
* thanks to: larsmans
* @see http://stackoverflow.com/questions/5820810/case-insensitive-string-comp-in-c
*
* @param a A c-string of the first string.
* @param b A c-string for the second string.
*
* @returns Returns 0 if the two strings are equivalant. <0 is returned
*			if a and b do not match and a comes before b. >0 is
*			returned if a and b do not match and b comes before a.
*/
int strcmpCI(char const *a, char const *b)
{
	while ((a[0] != '\0') && (b[0] != '\0'))
	{
		int d = tolower(*a) - tolower(*b);
		if (d != 0)
			return d;

		a++;
		b++;
	}

	return tolower(*a) - tolower(*b);
}

namespace BIO
{
	//Pre increment
	Date & Date::operator++()
	{
		//increment the day number
		_day++;
		_dayOfTheYear++;

		if (_day > DaysInMonth(_month, _year))
		{
			_day = 1;
			
			//now increment the month
			switch (_month)
			{
			case JANUARY:
				_month = FEBRUARY;
				break;
			case FEBRUARY:
				_month = MARCH;
				break;
			case MARCH:
				_month = APRIL;
				break;
			case APRIL:
				_month = MAY;
				break;
			case MAY:
				_month = JUNE;
				break;
			case JUNE:
				_month = JULY;
				break;
			case JULY:
				_month = AUGUST;
				break;
			case AUGUST:
				_month = SEPTEMBER;
				break;
			case SEPTEMBER:
				_month = OCTOBER;
				break;
			case OCTOBER:
				_month = NOVEMBER;
				break;
			case NOVEMBER:
				_month = DECEMBER;
				break;
			case DECEMBER:
				_month = JANUARY;
				_year++;
				_dayOfTheYear = 1;
				break;
			default://do nothing...
				break;
			}
			
		}

		return *this;
	}

	//post increment
	Date Date::operator ++ (int)
	{
		Date tmp(*this);//copy the current date
		operator++();//pre increment
		return tmp;//return the old value
	}

	unsigned int Date::DaysInMonth(DATE_MONTH month, unsigned int year)
	{
		switch (month)
		{
		case JANUARY:
			return 31;
		case FEBRUARY:
			if (IsLeapYear(year))
				return 29;
			return 28;
		case MARCH:
			return 31;
		case APRIL:
			return 30;
		case MAY:
			return 31;
		case JUNE:
			return 30;
		case JULY:
			return 31;
		case AUGUST:
			return 31;
		case SEPTEMBER:
			return 30;
		case OCTOBER:
			return 31;
		case NOVEMBER:
			return 30;
		case DECEMBER:
			return 31;
		default:
			return 30;
		}
	}

	DATE_MONTH Date::IntToMonth(int month)
	{
		switch (month)
		{
		case 2: return FEBRUARY;
		case 3: return MARCH;
		case 4: return APRIL;
		case 5: return MAY;
		case 6: return JUNE;
		case 7: return JULY;
		case 8: return AUGUST;
		case 9: return SEPTEMBER;
		case 10: return OCTOBER;
		case 11: return NOVEMBER;
		case 12: return DECEMBER;
		case 1:
		default: return JANUARY;
		}
	}

	bool Date::IsLeapYear(unsigned int year)
	{
		if ((year % 4) == 0)
		{//it is divisible by 4
			if ((year % 100) == 0)
			{//it is divisible by 100
				if ((year % 400) == 0)
				{//it is divisible by 400
					return true;
				}
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	void Date::SetDate(DATE_MONTH month, unsigned int day, unsigned int year)
	{
		_month = month;
		_year = year;

		//is the passed in number of days within the 
		//correct range for the month
		if (day <= DaysInMonth(month, year))
			_day = day;
		else
			_day = DaysInMonth(month, year);

		_dayOfTheYear = CalculateDayOfYear(_month, _day, _year);
	}

	DATE_MONTH Date::StringToMonth(char * month)
	{
		
		if ((strcmpCI(month, "FEBRUARY") == 0) || (strcmpCI(month, "FEB") == 0))
			return FEBRUARY;
		else if ((strcmpCI(month, "MARCH") == 0) || (strcmpCI(month, "MAR") == 0))
			return MARCH;
		else if ((strcmpCI(month, "APRIL") == 0) || (strcmpCI(month, "APR") == 0))
			return APRIL;
		else if (strcmpCI(month, "MAY") == 0)
			return MAY;
		else if ((strcmpCI(month, "JUNE") == 0) || (strcmpCI(month, "JUN") == 0))
			return JUNE;
		else if ((strcmpCI(month, "JULY") == 0) || (strcmpCI(month, "JUL") == 0))
			return JULY;
		else if ((strcmpCI(month, "AUGUST") == 0) || (strcmpCI(month, "AUG") == 0))
			return AUGUST;
		else if ((strcmpCI(month, "SEPTEMBER") == 0) || (strcmpCI(month, "SEPT") == 0) || (strcmpCI(month, "SEP") == 0))
			return SEPTEMBER;
		else if ((strcmpCI(month, "OCTOBER") == 0) || (strcmpCI(month, "OCT") == 0))
			return OCTOBER;
		else if ((strcmpCI(month, "NOVEMBER") == 0) || (strcmpCI(month, "NOV") == 0))
			return NOVEMBER;
		else if ((strcmpCI(month, "DECEMBER") == 0) || (strcmpCI(month, "DEC") == 0))
			return DECEMBER;
		//else if ((strcmpCI(month, "JANUARY") == 0) || (strcmpCI(month, "JAN") == 0))
		//	return JANUARY;
		else return JANUARY;
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//
	//				Private Functions
	//
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	void Date::_DeepCopy(const Date & other)
	{
		_month = other._month;
		_day = other._day;
		_year = other._year;
		_dayOfTheYear = other._dayOfTheYear;
	}

	unsigned int Date::_NumDaysBefore(DATE_MONTH month, unsigned int year)
	{
		int addValue = 0;

		if (IsLeapYear(year))
			addValue = 1;

		switch (month)
		{
		case FEBRUARY:
			return 31;
		case MARCH:
			return 59 + addValue;
		case APRIL:
			return 90 + addValue;
		case MAY:
			return 120 + addValue;
		case JUNE:
			return 151 + addValue;
		case JULY:
			return 181 + addValue;
		case AUGUST:
			return 212 + addValue;
		case SEPTEMBER:
			return 243 + addValue;
		case OCTOBER:
			return 273 + addValue;
		case NOVEMBER:
			return 304 + addValue;
		case DECEMBER:
			return 334 + addValue;
		case JANUARY:
		default:
			return 0;
		}
	}
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	//			  End Private Functions
	///////////////////////////////////////////////////////////////////////////

}//end namespace BIO


