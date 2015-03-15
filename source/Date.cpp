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

#if BIOSKY_TESTING == 1
	bool Date::Test(XNELO::TESTING::Test * test)
	{
		test->SetName("Date Tests");

		//static tests
		test->UnitTest(IntToMonth(0) == JANUARY, "Test IntToMonth 0");
		test->UnitTest(IntToMonth(1) == JANUARY, "Test IntToMonth 1");
		test->UnitTest(IntToMonth(2) == FEBRUARY, "Test IntToMonth 2");
		test->UnitTest(IntToMonth(3) == MARCH, "Test IntToMonth 3");
		test->UnitTest(IntToMonth(4) == APRIL, "Test IntToMonth 4");
		test->UnitTest(IntToMonth(5) == MAY, "Test IntToMonth 5");
		test->UnitTest(IntToMonth(6) == JUNE, "Test IntToMonth 6");
		test->UnitTest(IntToMonth(7) == JULY, "Test IntToMonth 7");
		test->UnitTest(IntToMonth(8) == AUGUST, "Test IntToMonth 8");
		test->UnitTest(IntToMonth(9) == SEPTEMBER, "Test IntToMonth 9");
		test->UnitTest(IntToMonth(10) == OCTOBER, "Test IntToMonth 10");
		test->UnitTest(IntToMonth(11) == NOVEMBER, "Test IntToMonth 11");
		test->UnitTest(IntToMonth(12) == DECEMBER, "Test IntToMonth 12");
		test->UnitTest(IntToMonth(13) == JANUARY, "Test IntToMonth 13");

		test->UnitTest(StringToMonth("January") == JANUARY, "Test StringToMonth January");
		test->UnitTest(StringToMonth("jan") == JANUARY, "Test StringToMonth jan");
		test->UnitTest(StringToMonth("February") == FEBRUARY, "Test StringToMonth February");
		test->UnitTest(StringToMonth("feb") == FEBRUARY, "Test StringToMonth feb");
		test->UnitTest(StringToMonth("March") == MARCH, "Test StringToMonth March");
		test->UnitTest(StringToMonth("mar") == MARCH, "Test StringToMonth Mar");
		test->UnitTest(StringToMonth("april") == APRIL, "Test StringToMonth april");
		test->UnitTest(StringToMonth("apr") == APRIL, "Test StringToMonth apr");
		test->UnitTest(StringToMonth("may") == MAY, "Test StringToMonth may");
		test->UnitTest(StringToMonth("june") == JUNE, "Test StringToMonth june");
		test->UnitTest(StringToMonth("jun") == JUNE, "Test StringToMonth jun");
		test->UnitTest(StringToMonth("july") == JULY, "Test StringToMonth july");
		test->UnitTest(StringToMonth("jul") == JULY, "Test StringToMonth jul");
		test->UnitTest(StringToMonth("august") == AUGUST, "Test StringToMonth august");
		test->UnitTest(StringToMonth("aug") == AUGUST, "Test StringToMonth aug");
		test->UnitTest(StringToMonth("September") == SEPTEMBER, "Test StringToMonth September");
		test->UnitTest(StringToMonth("Sept") == SEPTEMBER, "Test StringToMonth Sept");
		test->UnitTest(StringToMonth("Sep") == SEPTEMBER, "Test StringToMonth Sep");
		test->UnitTest(StringToMonth("OCtober") == OCTOBER, "Test StringToMonth OCtober");
		test->UnitTest(StringToMonth("oct") == OCTOBER, "Test StringToMonth Oct");
		test->UnitTest(StringToMonth("NOVEmber") == NOVEMBER, "Test StringToMonth NOVEmber");
		test->UnitTest(StringToMonth("nov") == NOVEMBER, "Test StringToMonth nov");
		test->UnitTest(StringToMonth("December") == DECEMBER, "Test StringToMonth December");
		test->UnitTest(StringToMonth("Dec") == DECEMBER, "Test StringToMonth Dec");
		test->UnitTest(StringToMonth("JIBERISH") == JANUARY, "Test StringToMonth JIBERISH");

		test->UnitTest(IsLeapYear(2014) == false, "Test Leap Year 2014:");
		test->UnitTest(IsLeapYear(1992) == true, "Test Leap Year 1992:");
		test->UnitTest(IsLeapYear(2000) == true, "Test Leap Year 2000:");
		test->UnitTest(IsLeapYear(1900) == false, "Test Leap Year 1900:");
		test->UnitTest(IsLeapYear(1600) == true, "Test Leap Year 1600:");
		test->UnitTest(IsLeapYear(1700) == false, "Test Leap Year 1700:");
		test->UnitTest(IsLeapYear(1996) == true, "Test Leap Year 1996:");

		test->UnitTest(DaysInMonth(JANUARY, 2014) == 31, "Num days in January:");
		test->UnitTest(DaysInMonth(FEBRUARY, 2014) == 28, "Num days in Feb 2014:");
		test->UnitTest(DaysInMonth(FEBRUARY, 2016) == 29, "Num days in Feb 2016:");
		test->UnitTest(DaysInMonth(MARCH, 2014) == 31, "Num days in March:");
		test->UnitTest(DaysInMonth(APRIL, 2014) == 30, "Num days in April:");
		test->UnitTest(DaysInMonth(MAY, 2014) == 31, "Num days in May:");
		test->UnitTest(DaysInMonth(JUNE, 2014) == 30, "Num days in June:");
		test->UnitTest(DaysInMonth(JULY, 2014) == 31, "Num days in July:");
		test->UnitTest(DaysInMonth(AUGUST, 2014) == 31, "Num days in August:");
		test->UnitTest(DaysInMonth(SEPTEMBER, 2014) == 30, "Num days in September:");
		test->UnitTest(DaysInMonth(OCTOBER, 2014) == 31, "Num days in October:");
		test->UnitTest(DaysInMonth(NOVEMBER, 2014) == 30, "Num days in November:");
		test->UnitTest(DaysInMonth(DECEMBER, 2014) == 31, "Num days in December:");

		test->UnitTest(_NumDaysBefore(JANUARY, 2014) == 0, "Num Days befor Jan. 2014:");
		test->UnitTest(_NumDaysBefore(FEBRUARY, 2014) == 31, "Num Days befor Feb. 2014:");
		test->UnitTest(_NumDaysBefore(MARCH, 2014) == 59, "Num Days befor Mar. 2014:");
		test->UnitTest(_NumDaysBefore(APRIL, 2014) == 90, "Num Days befor Apr. 2014:");
		test->UnitTest(_NumDaysBefore(MAY, 2014) == 120, "Num Days befor May 2014:");
		test->UnitTest(_NumDaysBefore(JUNE, 2014) == 151, "Num Days befor Jun. 2014:");
		test->UnitTest(_NumDaysBefore(JULY, 2014) == 181, "Num Days befor Jul. 2014:");
		test->UnitTest(_NumDaysBefore(AUGUST, 2014) == 212, "Num Days befor Aug. 2014:");
		test->UnitTest(_NumDaysBefore(SEPTEMBER, 2014) == 243, "Num Days befor Sept. 2014:");
		test->UnitTest(_NumDaysBefore(OCTOBER, 2014) == 273, "Num Days befor Oct. 2014:");
		test->UnitTest(_NumDaysBefore(NOVEMBER, 2014) == 304, "Num Days befor Nov. 2014:");
		test->UnitTest(_NumDaysBefore(DECEMBER, 2014) == 334, "Num Days befor Dec. 2014:");

		test->UnitTest(_NumDaysBefore(JANUARY, 2016) == 0, "Num Days befor Jan. 2016:");
		test->UnitTest(_NumDaysBefore(FEBRUARY, 2016) == 31, "Num Days befor Feb. 2016:");
		test->UnitTest(_NumDaysBefore(MARCH, 2016) == 60, "Num Days befor Mar. 2016:");
		test->UnitTest(_NumDaysBefore(APRIL, 2016) == 91, "Num Days befor Apr. 2016:");
		test->UnitTest(_NumDaysBefore(MAY, 2016) == 121, "Num Days befor May 2016:");
		test->UnitTest(_NumDaysBefore(JUNE, 2016) == 152, "Num Days befor Jun. 2016:");
		test->UnitTest(_NumDaysBefore(JULY, 2016) == 182, "Num Days befor Jul. 2016:");
		test->UnitTest(_NumDaysBefore(AUGUST, 2016) == 213, "Num Days befor Aug. 2016:");
		test->UnitTest(_NumDaysBefore(SEPTEMBER, 2016) == 244, "Num Days befor Sept. 2016:");
		test->UnitTest(_NumDaysBefore(OCTOBER, 2016) == 274, "Num Days befor Oct. 2016:");
		test->UnitTest(_NumDaysBefore(NOVEMBER, 2016) == 305, "Num Days befor Nov. 2016:");
		test->UnitTest(_NumDaysBefore(DECEMBER, 2016) == 335, "Num Days befor Dec. 2016:");

		test->UnitTest(CalculateDayOfYear(JANUARY, 1, 2034) == 1, "Day of Year January 1, 2034");
		test->UnitTest(CalculateDayOfYear(FEBRUARY, 1, 2015) == 32, "Day of Year February 1, 2015");
		test->UnitTest(CalculateDayOfYear(MARCH, 1, 2015) == 60, "Day of Year March 1, 2015");
		test->UnitTest(CalculateDayOfYear(MARCH, 1, 2016) == 61, "Day of Year March 1, 2016");
		test->UnitTest(CalculateDayOfYear(APRIL, 1, 2015) == 91, "Day of Year April 1, 2015");
		test->UnitTest(CalculateDayOfYear(APRIL, 1, 2016) == 92, "Day of Year April 1, 2016");
		test->UnitTest(CalculateDayOfYear(MAY, 1, 2015) == 121, "Day of Year May 1, 2015");
		test->UnitTest(CalculateDayOfYear(MAY, 1, 2016) == 122, "Day of Year May 1, 2016");
		test->UnitTest(CalculateDayOfYear(JUNE, 6, 1944) == 158, "Day of year June 6, 1944");
		test->UnitTest(CalculateDayOfYear(JUNE, 6, 2015) == 157, "Day of year June 6, 2016");
		test->UnitTest(CalculateDayOfYear(JULY, 1, 2015) == 182, "Day of Year JULY 1, 2015");
		test->UnitTest(CalculateDayOfYear(JULY, 1, 2016) == 183, "Day of Year July 1, 2016");
		test->UnitTest(CalculateDayOfYear(AUGUST, 1, 2015) == 213, "Day of Year August 1, 2015");
		test->UnitTest(CalculateDayOfYear(AUGUST, 17, 2015) == 229, "Day of Year August 1, 2015");
		test->UnitTest(CalculateDayOfYear(AUGUST, 1, 2016) == 214, "Day of Year August 1, 2016");
		test->UnitTest(CalculateDayOfYear(SEPTEMBER, 1, 2015) == 244, "Day of Year September 1, 2015");
		test->UnitTest(CalculateDayOfYear(SEPTEMBER, 1, 2016) == 245, "Day of Year September 1, 2016");
		test->UnitTest(CalculateDayOfYear(OCTOBER, 1, 2015) == 274, "Day of Year October 1, 2015");
		test->UnitTest(CalculateDayOfYear(OCTOBER, 1, 2016) == 275, "Day of Year October 1, 2016");
		test->UnitTest(CalculateDayOfYear(NOVEMBER, 1, 2015) == 305, "Day of Year November 1, 2015");
		test->UnitTest(CalculateDayOfYear(NOVEMBER, 1, 2016) == 306, "Day of Year November 1, 2016");
		test->UnitTest(CalculateDayOfYear(DECEMBER, 31, 2015) == 365, "Day of Year December 31, 2015");
		test->UnitTest(CalculateDayOfYear(DECEMBER, 31, 2016) == 366, "Day of Year December 31, 2016");

		test->UnitTest(MonthToInt(JANUARY) == 1, "January Is Month 1");
		test->UnitTest(MonthToInt(FEBRUARY) == 2, "January Is Month 2");
		test->UnitTest(MonthToInt(MARCH) == 3, "January Is Month 3");
		test->UnitTest(MonthToInt(APRIL) == 4, "January Is Month 4");
		test->UnitTest(MonthToInt(MAY) == 5, "January Is Month 5");
		test->UnitTest(MonthToInt(JUNE) == 6, "January Is Month 6");
		test->UnitTest(MonthToInt(JULY) == 7, "January Is Month 7");
		test->UnitTest(MonthToInt(AUGUST) == 8, "January Is Month 8");
		test->UnitTest(MonthToInt(SEPTEMBER) == 9, "January Is Month 9");
		test->UnitTest(MonthToInt(OCTOBER) == 10, "January Is Month 10");
		test->UnitTest(MonthToInt(NOVEMBER) == 11, "January Is Month 11");
		test->UnitTest(MonthToInt(DECEMBER) == 12, "January Is Month 12");

		Date date;

		test->UnitTest(date.GetDay() == 1, "Test Default Values 1");
		test->UnitTest(date.GetDayOfTheYear() == 1, "Test Default Values 2");
		test->UnitTest(date.GetMonth() == JANUARY, "Test Default Values 3");
		test->UnitTest(date.GetYear() == 2000, "Test Default Values 4");

		date.SetDate(AUGUST, 17, 2013);

		test->UnitTest(date.GetDay() == 17, "Test Set Date August 17, 2013 [1]");
		test->UnitTest(date.GetDayOfTheYear() == 229, "Test Set Date August 17, 2013 [2]");
		test->UnitTest(date.GetMonth() == AUGUST, "Test Set Date August 17, 2013 [3]");
		test->UnitTest(date.GetYear() == 2013, "Test Set Date August 17, 2013 [4]");

		date.SetYear(2016);
		test->UnitTest(date.GetDay() == 17, "Test Set Date August 17, 2016 [1]");
		test->UnitTest(date.GetDayOfTheYear() == 230, "Test Set Date August 17, 2016 [2]");
		test->UnitTest(date.GetMonth() == AUGUST, "Test Set Date August 17, 2016 [3]");
		test->UnitTest(date.GetYear() == 2016, "Test Set Date August 17, 2016 [4]");

		date.SetMonth(JANUARY);
		test->UnitTest(date.GetDay() == 17, "Test Set Date January 17, 2016 [1]");
		test->UnitTest(date.GetDayOfTheYear() == 17, "Test Set Date January 17, 2016 [2]");
		test->UnitTest(date.GetMonth() == JANUARY, "Test Set Date January 17, 2016 [3]");
		test->UnitTest(date.GetYear() == 2016, "Test Set Date January 17, 2016 [4]");

		date.SetDay(28);
		test->UnitTest(date.GetDay() == 28, "Test Set Date January 28, 2016 [1]");
		test->UnitTest(date.GetDayOfTheYear() == 28, "Test Set Date January 28, 2016 [2]");
		test->UnitTest(date.GetMonth() == JANUARY, "Test Set Date January 28, 2016 [3]");
		test->UnitTest(date.GetYear() == 2016, "Test Set Date January 28, 2016 [4]");

		//check error checking.
		date.SetDay(32);
		test->UnitTest(date.GetDay() == 31, "Test error checking [1]");
		test->UnitTest(date.GetDayOfTheYear() == 31, "Test error checking [2]");
		test->UnitTest(date.GetMonth() == JANUARY, "Test error checking [3]");
		test->UnitTest(date.GetYear() == 2016, "Test error checking [4]");

		date.SetMonth(FEBRUARY);
		test->UnitTest(date.GetDay() == 29, "Test error checking [5]");
		test->UnitTest(date.GetDayOfTheYear() == 60, "Test error checking [6]");
		test->UnitTest(date.GetMonth() == FEBRUARY, "Test error checking [7]");
		test->UnitTest(date.GetYear() == 2016, "Test error checking [8]");

		date.SetYear(2015);
		test->UnitTest(date.GetDay() == 28, "Test error checking [9]");
		test->UnitTest(date.GetDayOfTheYear() == 59, "Test error checking [10]");
		test->UnitTest(date.GetMonth() == FEBRUARY, "Test error checking [11]");
		test->UnitTest(date.GetYear() == 2015, "Test error checking [12]");

		date.SetMonth(MARCH);
		test->UnitTest(date.GetDay() == 28, "Test error checking [13]");
		test->UnitTest(date.GetDayOfTheYear() == 87, "Test error checking [14]");
		test->UnitTest(date.GetMonth() == MARCH, "Test error checking [15]");
		test->UnitTest(date.GetYear() == 2015, "Test error checking [16]");

		//test equality and assignment operators
		Date date2;
		test->UnitTest((date == date2) == false, "Dates equal false");
		test->UnitTest((date != date2) == true, "Dates not equal true");

		date2 = date;
		test->UnitTest((date == date2) == true, "Dates equal true");
		test->UnitTest((date != date2) == false, "Dates not equal false");

		test->UnitTest(date2.GetDay() == 28, "Test Assignment operator [1]");
		test->UnitTest(date2.GetDayOfTheYear() == 87, "Test Assignment operator [2]");
		test->UnitTest(date2.GetMonth() == MARCH, "Test Assignment operator [3]");
		test->UnitTest(date2.GetYear() == 2015, "Test Assignment operator [4]");

		date2.SetYear(2016);
		test->UnitTest(date2.GetDay() == 28, "Test different Dates [1]");
		test->UnitTest(date2.GetDayOfTheYear() == 88, "Test different Dates [2]");
		test->UnitTest(date2.GetMonth() == MARCH, "Test different Dates [3]");
		test->UnitTest(date2.GetYear() == 2016, "Test different Dates [4]");

		Date date3(date2);
		test->UnitTest(date3.GetDay() == 28, "Test Copy Constructor [1]");
		test->UnitTest(date3.GetDayOfTheYear() == 88, "Test Copy Constructor [2]");
		test->UnitTest(date3.GetMonth() == MARCH, "Test Copy Constructor [3]");
		test->UnitTest(date3.GetYear() == 2016, "Test Copy Constructor [4]");

		date3++;
		test->UnitTest(date3.GetDay() == 29, "Test incrementor [1]");
		test->UnitTest(date3.GetDayOfTheYear() == 89, "Test incrementor [2]");
		test->UnitTest(date3.GetMonth() == MARCH, "Test incrementor [3]");
		test->UnitTest(date3.GetYear() == 2016, "Test incrementor [4]");
		date3++;
		date3++;
		test->UnitTest(date3.GetDay() == 31, "Test incrementor [5]");
		test->UnitTest(date3.GetDayOfTheYear() == 91, "Test incrementor [6]");
		test->UnitTest(date3.GetMonth() == MARCH, "Test incrementor [7]");
		test->UnitTest(date3.GetYear() == 2016, "Test incrementor [8]");
		date3++;
		test->UnitTest(date3.GetDay() == 1, "Test incrementor [9]");
		test->UnitTest(date3.GetDayOfTheYear() == 92, "Test incrementor [10]");
		test->UnitTest(date3.GetMonth() == APRIL, "Test incrementor [11]");
		test->UnitTest(date3.GetYear() == 2016, "Test incrementor [12]");

		date3.SetDate(DECEMBER, 31, 2016); date3++;
		test->UnitTest(date3.GetDay() == 1, "Test incrementor [13]");
		test->UnitTest(date3.GetDayOfTheYear() == 1, "Test incrementor [14]");
		test->UnitTest(date3.GetMonth() == JANUARY, "Test incrementor [15]");
		test->UnitTest(date3.GetYear() == 2017, "Test incrementor [16]");

		//test increment from Jan to Feb
		date3.SetDate(JANUARY, 31, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 32) &&
			(date3.GetMonth() == FEBRUARY) &&
			(date3.GetYear() == 2017), "Test incrementor [17]");

		//test increment from Feb to March
		date3.SetDate(FEBRUARY, 28, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 60) &&
			(date3.GetMonth() == MARCH) &&
			(date3.GetYear() == 2017), "Test incrementor [18]");

		//test increment from April to MAY
		date3.SetDate(APRIL, 30, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 121) &&
			(date3.GetMonth() == MAY) &&
			(date3.GetYear() == 2017), "Test incrementor [19]");

		//test increment from May to JUNE
		date3.SetDate(MAY, 31, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 152) &&
			(date3.GetMonth() == JUNE) &&
			(date3.GetYear() == 2017), "Test incrementor [20]");

		//test increment from JUNE to JULY
		date3.SetDate(JUNE, 30, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 182) &&
			(date3.GetMonth() == JULY) &&
			(date3.GetYear() == 2017), "Test incrementor [21]");

		//test increment from JULY to AUGUST
		date3.SetDate(JULY, 31, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 213) &&
			(date3.GetMonth() == AUGUST) &&
			(date3.GetYear() == 2017), "Test incrementor [22]");

		//test increment from AUGUST to SEPTEMBER
		date3.SetDate(AUGUST, 31, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 244) &&
			(date3.GetMonth() == SEPTEMBER) &&
			(date3.GetYear() == 2017), "Test incrementor [23]");

		//test increment from SEPTEMBER to OCTOBER
		date3.SetDate(SEPTEMBER, 30, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 274) &&
			(date3.GetMonth() == OCTOBER) &&
			(date3.GetYear() == 2017), "Test incrementor [24]");

		//test increment from OCTOBER to NOVEMBER
		date3.SetDate(OCTOBER, 31, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 305) &&
			(date3.GetMonth() == NOVEMBER) &&
			(date3.GetYear() == 2017), "Test incrementor [25]");

		//test increment from NOVEMBER to December
		date3.SetDate(NOVEMBER, 30, 2017); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 335) &&
			(date3.GetMonth() == DECEMBER) &&
			(date3.GetYear() == 2017), "Test incrementor [26]");

		//test increment from Feb to March on leap year
		date3.SetDate(FEBRUARY, 29, 2020); date3++;
		test->UnitTest((date3.GetDay() == 1) &&
			(date3.GetDayOfTheYear() == 61) &&
			(date3.GetMonth() == MARCH) &&
			(date3.GetYear() == 2020), "Test incrementor [27]");

		return test->GetSuccess();
	}
#endif

}//end namespace BIO


