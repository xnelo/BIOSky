/**
* @file GPS.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This defines a GPS (Global Positioning System) coordinates.
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

#ifndef ___BIOSKY_GPS_HPP__2015___
#define ___BIOSKY_GPS_HPP__2015___

#include "CompileConfig.h"
#include "MathUtils.hpp"

namespace BIO
{
	/**
	* A class that contains a GPS (Global Positioning System) coordinate,
	* a latitude and a longitude.
	*/
	class GPS
	{
	private: 
		/**The latitude in degrees.*/
		float _latitude;
		/**The longitude in degrees.*/
		float _longitude;

		/**
		* Makes a deep copy of another GPS coordinate.
		*
		* @param other The GPS coordiante to make a copy of.
		*/
		BIOSKY_API void _deepCopy(const GPS & other);
	public:
		/**
		* Default Constructor
		*/
		BIOSKY_API GPS();

		/**
		* Constructor
		*
		* @param latDegrees The latitude in degrees.
		*
		* @param lonDegrees The longitude in degrees.
		*/
		BIOSKY_API GPS(float latDegrees, float lonDegrees);

		/**
		* Copy Constructor
		*/
		BIOSKY_API GPS(const GPS & other);

		/**
		* Destructor
		*/
		BIOSKY_API ~GPS();

		/**
		* Assignment operator
		*/
		BIOSKY_API GPS & operator = (const GPS & other);

		/**
		* Get the Latitude in Degrees.
		*
		* @return Returns a float with the latitude in degrees.
		*/
		BIOSKY_API float GetLatitude();

		/**
		* Get the Latitude in Radians.
		*
		* @return Returns a float with the latitude in radians.
		*/
		BIOSKY_API float GetLatitudeRadians();

		/**
		* Get the Longitude in Degrees.
		*
		* @return Returns a float with the longitude in degrees.
		*/
		BIOSKY_API float GetLongitude();

		/**
		* Get the Longitude in Radians.
		*
		* @return Returns a float with the longitude in Radians.
		*/
		BIOSKY_API float GetLongitudeRadians();

		/**
		* Set the Latitude.
		*
		* @param lat The latitude in Degrees.
		*/
		BIOSKY_API void SetLatitude(float lat);

		/**
		* Set the Latitude.
		*
		* @param lat The latitude in Radians.
		*/
		BIOSKY_API void SetLatitudeRadians(float lat);

		/**
		* Set the longitude.
		*
		* @param lon The longitude in degrees.
		*/
		BIOSKY_API void SetLongitude(float lon);

		/**
		* Set the longitude.
		*
		* @param lon The longitude in radians.
		*/
		BIOSKY_API void SetLongitudeRadians(float lon);

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
} //end namespace BIO

inline BIO::GPS::GPS() : _latitude(0.0f), _longitude(0.0f)
{}

inline BIO::GPS::GPS(float latDegrees, float lonDegrees) : _latitude(latDegrees), _longitude(lonDegrees)
{}

inline BIO::GPS::GPS(const GPS & other) : _latitude(0.0f), _longitude(0.0f)
{
	_deepCopy(other);
}

inline BIO::GPS & BIO::GPS::operator = (const GPS & other)
{
	if (this != &other)
		_deepCopy(other);

	return *this;
}

inline float BIO::GPS::GetLatitude()
{
	return _latitude;
}

inline float BIO::GPS::GetLatitudeRadians()
{
	return _latitude * MATH::DegreesToRadiansf;
}

inline float BIO::GPS::GetLongitude()
{
	return _longitude;
}

inline float BIO::GPS::GetLongitudeRadians()
{
	return _longitude * MATH::DegreesToRadiansf;
}

inline void BIO::GPS::SetLatitudeRadians(float lat)
{
	SetLatitude(lat * MATH::RadiansToDegreesf);
}

inline void BIO::GPS::SetLongitudeRadians(float lon)
{
	SetLongitude(lon * MATH::RadiansToDegreesf);
}

#endif //___BIOSKY_GPS_HPP__2015___