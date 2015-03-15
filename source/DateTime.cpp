/**
* @file DateTime.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implement the DateTime class.
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

#include "DateTime.hpp"

#include <iostream>

namespace BIO
{

	void DateTime::AddTime(float timeToAdd)
	{
		_time += timeToAdd;

		//if (_time >= 86400.0f)//initial implementation
		while (_time >= 86400.0f)//allows adding of multiple days
		{
			_time -= 86400.0f;

			_date++;
		}
	}

	void DateTime::SetTime(float newTime)
	{
		if ((newTime >= 86400.0f) || (newTime < 0.0f))
		{
			_time = 0.0f;
			return;
		}

		_time = newTime;
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//	
	//						Private Functions
	//
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	void DateTime::_DeepCopy(const DateTime & other)
	{
		_date = other._date;
		_time = other._time;
		_UTCOffset = other._UTCOffset;
	}
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	//				   End Private Functions
	///////////////////////////////////////////////////////////////////////////

#if BIOSKY_TESTING == 1
	bool DateTime::Test(XNELO::TESTING::Test * test)
	{
		test->SetName("DateTime Tests");

		//Test default values and getters.
		DateTime dt;

		test->UnitTest(dt.GetDay() == 1, "Test Default Values and Getters [1]");
		test->UnitTest(dt.GetDayOfYear() == 1, "Test Default Values and Getters [2]");
		test->UnitTest(dt.GetMonth() == JANUARY, "Test Default Values and Getters [3]");
		test->UnitTest(dt.GetTime() == 0.0f, "Test Default Values and Getters [4]");
		test->UnitTest(dt.GetYear() == 2000, "Test Default Values and Getters [5]");

		Date nd;
		nd.SetDate(AUGUST, 17, 2015);
		dt.SetDate(nd);
		test->UnitTest(dt.GetDate().GetDay() == 17, "Test SetDate [1]");
		test->UnitTest(dt.GetDate().GetDayOfTheYear() == 229, "Test SetDate [2]");
		test->UnitTest(dt.GetDate().GetMonth() == AUGUST, "Test SetDate [3]");
		test->UnitTest(dt.GetTime() == 0.0f, "Test SetDate [4]");
		test->UnitTest(dt.GetDate().GetYear() == 2015, "Test SetDate [5]");

		nd.SetDate(JUNE, 6, 1944);
		dt.SetDate(JUNE, 6, 1944);
		test->UnitTest(dt.GetDate() == nd, "Test SetDate[6]");

		nd.SetDay(24);
		dt.SetDay(24);
		test->UnitTest(dt.GetDate() == nd, "Test SetDate[7]");

		nd.SetMonth(DECEMBER);
		dt.SetMonth(DECEMBER);
		test->UnitTest(dt.GetDate() == nd, "Test SetDate[8]");

		nd.SetYear(1999);
		dt.SetYear(1999);
		test->UnitTest(dt.GetDate() == nd, "Test SetDate[9]");

		//Time nt; nt.SetTimeHours(15.4f);
		//dt.SetTimeHours(nt);
		//test->UnitTest(dt.GetTimeHours() == 15.4f, "Test SetTime [1]");

		dt.SetTime(15389.2f);
		test->UnitTest(dt.GetTime() == 15389.2f, "Test SetTime [1]");

		dt.SetTimeHours(2.34f);
		test->UnitTest(dt.GetTimeHours() == 2.34f, "Test SetTime hours [1]");

		dt.SetTimeHours(27.33f);
		test->UnitTest(dt.GetTimeHours() == 0.0f, "Test SetTime catch error.");

		dt.SetTimeHours(5.4f);
		DateTime dt2(dt);
		test->UnitTest(dt2.GetDay() == 24, "Test Copy Constructor [1]");
		test->UnitTest(dt2.GetDayOfYear() == 358, "Test Copy Constructor [2]");
		test->UnitTest(dt2.GetMonth() == DECEMBER, "Test Copy Constructor [3]");
		test->UnitTest(dt2.GetTimeHours() == 5.4f, "Test Copy Constructor [4]");
		test->UnitTest(dt2.GetYear() == 1999, "Test Copy Constructor [5]");

		DateTime dt3;
		dt3 = dt;
		test->UnitTest(dt3.GetDay() == 24, "Test Assignment Operator [1]");
		test->UnitTest(dt3.GetDayOfYear() == 358, "Test Assignment Operator [2]");
		test->UnitTest(dt3.GetMonth() == DECEMBER, "Test Assignment Operator [3]");
		test->UnitTest(dt3.GetTimeHours() == 5.4f, "Test Assignment Operator [4]");
		test->UnitTest(dt3.GetYear() == 1999, "Test Assignment Operator [5]");

		////////////////////////////
		//Test Add Time functions.
		////////////////////////////
		float tolerance = 0.01f;
		float avgFrameRate = 0.01612f;

		dt.AddTime(avgFrameRate);
		test->UnitTest(dt.GetTime(), 19440.01612f, tolerance, "Add Time correct");
		dt.AddTime(avgFrameRate); dt.AddTime(avgFrameRate);
		test->UnitTest(dt.GetTime(), 19440.04836f, tolerance, "Add Time correct 2");

		dt.SetTime(86398.0f);
		test->UnitTest(dt.GetDay() == 24, "Test Auto Day Increment [1]");
		test->UnitTest(dt.GetDayOfYear() == 358, "Test Auto Day Increment [2]");
		test->UnitTest(dt.GetMonth() == DECEMBER, "Test Auto Day Increment [3]");
		test->UnitTest(dt.GetTime() == 86398.0f, "Test Auto Day Increment [4]");
		test->UnitTest(dt.GetYear() == 1999, "Test Auto Day Increment [5]");

		dt.AddTime(3.0f);//add 3 seconds
		test->UnitTest(dt.GetDay() == 25, "Test Auto Day Increment [6]");
		test->UnitTest(dt.GetDayOfYear() == 359, "Test Auto Day Increment [7]");
		test->UnitTest(dt.GetMonth() == DECEMBER, "Test Auto Day Increment [8]");
		test->UnitTest(dt.GetTime() == 1.0f, "Test Auto Day Increment [9]");
		test->UnitTest(dt.GetYear() == 1999, "Test Auto Day Increment [10]");

		dt.AddTime(172800.0f);//Add 2 days
		test->UnitTest(dt.GetDay() == 27, "Test Auto Day Increment [11]");
		test->UnitTest(dt.GetDayOfYear() == 361, "Test Auto Day Increment [12]");
		test->UnitTest(dt.GetMonth() == DECEMBER, "Test Auto Day Increment [13]");
		test->UnitTest(dt.GetTime() == 1.0f, "Test Auto Day Increment [14]");
		test->UnitTest(dt.GetYear() == 1999, "Test Auto Day Increment [15]");

		test->UnitTest((dt == dt3) == false, "Test Equality operator == false");
		test->UnitTest((dt2 == dt3) == true, "Test Equality operator == true");

		test->UnitTest((dt != dt3) == true, "Test Equality operator == true");
		test->UnitTest((dt2 != dt3) == false, "Test Equality operator == false");

		//test UTCOffset
		dt.SetUTCOffset(-3.0f);
		test->UnitTest(dt.GetUTCOffset() == -3.0f, "Test SetUTCOffset works");

		dt.SetUTCOffset(-12.0f);
		test->UnitTest(dt.GetUTCOffset() == -12.0f, "Test SetUTCOffset works on lower bound");

		dt.SetUTCOffset(12.0f);
		test->UnitTest(dt.GetUTCOffset() == 12.0f, "Test SetUTCOffset works on upper bound");

		dt.SetUTCOffset(-18.0f);
		test->UnitTest(dt.GetUTCOffset() == -12.0f, "Test SetUTCOffset works cap at lower bound");

		dt.SetUTCOffset(176.0f);
		test->UnitTest(dt.GetUTCOffset() == 12.0f, "Test SetUTCOffset works cap at upper bound");

		return test->GetSuccess();
	}
#endif

}//end namespace BIO


