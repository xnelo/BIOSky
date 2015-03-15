/**
* @file GPS.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implements the GPS definitions.
*/
/*
* The zlib/libpng License
*
* Copyright (c) 2015 Spencer Hoffa
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

#include "GPS.hpp"

namespace BIO
{
	GPS::~GPS()
	{
		_latitude = 0.0f;
		_longitude = 0.0f;
	}

	void GPS::SetLatitude(float lat)
	{
		if (lat < -90.0f)
			lat = -90.0f;

		if (lat > 90.0f)
			lat = 90.0f;

		_latitude = lat;
	}

	void GPS::SetLongitude(float lon)
	{
		if (lon < -180.0f)
			lon = -180.0f;

		if (lon > 180.0f)
			lon = 180.0f;

		_longitude = lon;
	}

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//						Private functions
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	void GPS::_deepCopy(const GPS & other)
	{
		_latitude = other._latitude;
		_longitude = other._longitude;
	}
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	//					End Private Functions
	///////////////////////////////////////////////////////////////////////

#if BIOSKY_TESTING == 1
	bool GPS::Test(XNELO::TESTING::Test * test)
	{
		test->SetName("GPS Tests");

		GPS gps;

		test->UnitTest(gps.GetLatitude() == 0.0f, "Test Default Latitude Value");
		test->UnitTest(gps.GetLongitude() == 0.0f, "Test Default Longitude Value");

		gps.SetLatitude(41.0f);
		gps.SetLongitude(-112.0f);

		test->UnitTest(gps.GetLatitude() == 41.0f, "Test Set Latitude");
		test->UnitTest(gps.GetLongitude() == -112.0f, "Test Set Longitude");

		gps.SetLatitude(90.1f);
		gps.SetLongitude(-192.0f);

		test->UnitTest(gps.GetLatitude() == 90.0f, "Test Set Latitude Cap Upper Bounds");
		test->UnitTest(gps.GetLongitude() == -180.0f, "Test Set Longitude Cap Lower Bounds");

		gps.SetLatitude(-100.0f);
		gps.SetLongitude(200.0f);

		test->UnitTest(gps.GetLatitude() == -90.0f, "Test Set Latitude Cap Lower Bounds");
		test->UnitTest(gps.GetLongitude() == 180.0f, "Test Set Longitude Cap Upper Bounds");

		test->UnitTest(gps.GetLatitudeRadians() == -MATH::PId2f, "Test GetLatitude in radians");
		test->UnitTest(gps.GetLongitudeRadians() == MATH::PIf, "Test GetLongitude in radians");

		gps.SetLatitudeRadians(1.7f);
		gps.SetLongitudeRadians(-3.2f);

		test->UnitTest(gps.GetLatitude() == 90.0f, "Test Set Latitude Radians Cap Upper Bounds");
		test->UnitTest(gps.GetLongitude() == -180.0f, "Test Set Longitude Radians Cap Lower Bounds");

		gps.SetLatitudeRadians(-2.0f);
		gps.SetLongitudeRadians(4.0f);

		test->UnitTest(gps.GetLatitude() == -90.0f, "Test Set Latitude Radians Cap Lower Bounds");
		test->UnitTest(gps.GetLongitude() == 180.0f, "Test Set Longitude Radians Cap Upper Bounds");

		return test->GetSuccess();
	}
#endif
}//end namespace BIO