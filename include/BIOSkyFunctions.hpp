/**
* @file BIOSkyFunctions.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This file defines the global functions used in the BIOSky library.
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

#ifndef ___BIOSKY_BIOSKYFUNCTIONS_HPP__2015___
#define ___BIOSKY_BIOSKYFUNCTIONS_HPP__2015___

#include "CompileConfig.h"
#include "SkyPosition.hpp"
#include "SkyData.hpp"
#include "Date.hpp"
#include "RawGeometry.hpp"
#include "MathUtils.hpp"

//define library public functions
namespace BIO
{
	namespace SKY
	{
		/**
		* Calculate the Zenith of the celestial North Pole.
		*
		* @param latitude The latitude of the location we are calculating
		*			the Moon Position for in radians. Location North of the
		*			equator are positive and locations South of the equator
		*			are negative. Range: -PI/2 to PI/2.
		*/
		BIOSKY_API float CalculateCelestialNorthPoleZenith(float latitude);

		/**
		* Calculate the Julian Date.
		*
		* @param month The Month of the year.
		*
		* @param day The day of the month.
		*
		* @param year The year.
		*/
		BIOSKY_API float CalculateJulianDay(DATE_MONTH month, unsigned int day, int year);

		/**
		* Calculate the Phase of the moon;
		*
		* @param standardTime The time in 24 hr decimal format. For Example
		*			1:15 PM == 13.25.
		*
		* @param UTCoffset The number of hours offset from UTC-0 time.
		*
		* @param month The month of the year.
		*
		* @param day The day of the month.
		*
		* @param year The year.
		*
		* @return Returns a float with the moon phase between [0,360]
		*/
		BIOSKY_API float CalculateMoonPhase(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year);

		/**
		* Calculate Moon Position
		*
		* @param standardTime The time in 24 hr decimal format. For Example
		*			1:15 PM == 13.25.
		*
		* @param UTCoffset The number of hours offset from UTC-0 time.
		*
		* @param month The month of the year.
		*
		* @param day The day of the month.
		*
		* @param year The year.
		*
		* @param latitude The latitude of the location we are calculating
		*			the Moon Position for in radians. Location North of the
		*			equator are positive and locations South of the equator
		*			are negative. Range: -PI/2 to PI/2.
		*
		* @param longitude The longitude of the location we are calculating
		*			the Moon Position for in radians. Locations east of the
		*			prime meridian are positive and locations west of the
		*			prime meridian are negative. Range: -PI to PI.
		*
		* @return Returns a structure containing the Azimuth and Zenith of
		*			the moon in the sky. The Azimuth is the angle from due
		*			North the sun appears and the Zenith is the angle from
		*			directly above where the sun appears.
		*/
		BIOSKY_API SkyPosition CalculateMoonPosition(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude);

		/**
		* Calculate all of the information needed for the positioning of 
		* celestial objects in the BIOSky library.
		*
		* @param standardTime The time in 24 hr decimal format. For Example
		*			1:15 PM == 13.25.
		*
		* @param UTCoffset The number of hours offset from UTC-0 time.
		*
		* @param month The month of the year.
		*
		* @param day The day of the month.
		*
		* @param year The year.
		*
		* @param latitude The latitude of the location we are calculating
		*			the Moon Position for in radians. Location North of the
		*			equator are positive and locations South of the equator
		*			are negative. Range: -PI/2 to PI/2.
		*
		* @param longitude The longitude of the location we are calculating
		*			the Moon Position for in radians. Locations east of the
		*			prime meridian are positive and locations west of the
		*			prime meridian are negative. Range: -PI to PI.
		*
		* @return Returns a SkyData structure with all of the data needed to 
		*			position the sun, moon, stars, and moon phase.
		*/
		BIOSKY_API SkyData CalculateSkyData(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude);

		/**
		* Calculates the rotation angle of the stars around the celestial north
		* pole depending on time and location.
		*
		* @param standardTime The time in 24 hr decimal format. For Example
		*			1:15 PM == 13.25.
		*
		* @param UTCoffset The number of hours offset from UTC-0 time.
		*/
		BIOSKY_API float CalculateStarRotation(float standardTime, float UTCoffset);

		/**
		* Calculate the position of the sun.
		*
		* @param standardTime The time in 24 hr decimal format. For Example
		*			1:15 PM == 13.25.
		*
		* @param UTCoffset The number of hours offset from UTC-0 time.
		*
		* @param month The month of the year.
		*
		* @param day The day of the month.
		*
		* @param year The year.
		*
		* @param latitude The latitude of the location we are calculating
		*			the Sun Position for in radians. Location North of the
		*			equator are positive and locations South of the equator
		*			are negative. Range: -PI/2 to PI/2.
		*
		* @param longitude The longitude of the location we are calculating
		*			the Sun Position for in radians. Locations east of the
		*			prime meridian are positive and locations west of the
		*			prime meridian are negative. Range: -PI to PI.
		*
		* @return Returns a structure containing the Azimuth and Zenith of
		*			the sun in the sky. The Azimuth is the angle from due
		*			North the sun appears and the Zenith is the angle from
		*			directly above where the sun appears.
		*/
		BIOSKY_API SkyPosition CalculateSunPosition(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude);

		/**
		* Creates a skydome geometry that is compliant with this engine's
		* proccesses.
		*
		* @note The skydome is actually a sphere.
		*
		* @param radius The radius of the skydome to create. Default = 1.0f
		*
		* @param numVerticalSegments This is an integer that represents the
		*			number of Segments(lines) that go up and down. You can
		*			think of this as longitude lines on a globe.  In other
		*			words... How many longitude lines do you want? That is
		*			what this parameter represents. Default = 12
		*
		* @param numHorozontalSegments This is an integer that represents
		*			the numbero segments(lines) that go left to right. You
		*			can think of this a latitude lines on a globe. In other
		*			words... How many latitude lines do you want? That is
		*			what this parameter represents. Default = 12. 
		*
		*			This value must be an even number. If the value passed in 
		*			is not an even number then this function will add one to 
		*			the passed in value to make it an even number.
		*
		* @param fullDome Does this create a full sphere or just a hemisphere. 
		*			Pass in true for a full sphere or false for a hemisphere
		*			sky dome geometry. Default = true;
		*
		* @return Returns a pointer to a Geometry Object. This object was
		*			created on the heap so you are responsible for deleting
		*			it after calling this function.
		*/
		BIOSKY_API RawGeometry * CreateSkyDomeGeometry(float radius = 1.0f, int numVerticalSegments = 12, int numHorozontalSegments = 12, bool fullDome = true);

		BIOSKY_API RawGeometry * CreateNightSkyDomeGeometry(float radius = 1.0f);

		BIOSKY_API unsigned char * CreateNightSkyTexture(int * width, int * height);

		/**
		* Creates a texture for the moon billboard. The resulting
		* texture will be a square image with dimensions widthxheight. The
		* format of the returned will be:
		* [ B1, G1, R1, A1, B2, G2, R2, A2, B3, G3 ....]
		* So the overall returned size will be widthxheightx4 unsigned char
		* elements.
		*
		* The size of the returned array is based on the image that is
		* being used as the texture. For this reason you need to supply a
		* width and height variable that this function will modify to
		* return the width and hight of the texture.
		*
		* @param[out] width The width of the returned texture.
		*
		* @param[out] height The height of the returned texture.
		*
		* @return Returns an array of unsigned char data points
		*			representing a square texture. This function creates
		*			the data on the heap and returns a pointer to that data.
		*			You are responsible for deleting this data when you are
		*			done with it.
		*/
		BIOSKY_API unsigned char * CreateMoonTexture(int * width, int * height);

		/**
		* Create the texture used for the sun billboard. The resulting
		* texture will be a square image with sideLength the length of one
		* of the square's sides. The formate of the returned will be:
		* [ B1, G1, R1, A1, B2, G2, R2, A2, B3, G3 ....].
		*
		* @param sideLength The length of the side of the square.
		*
		* @return Returns an array of unsigned char data points
		*			representing a square texture. This function creates
		*			the data on the heap and returns a pointer to that data.
		*			You are responsible for deleting this data when you are
		*			done with it.
		*/
		BIOSKY_API unsigned char * CreateSunTexture(int sideLength);

		BIOSKY_API int DaysSinceJan02000(DATE_MONTH month, unsigned int day, unsigned int year);

		/**
		* Run tests on the BIOSky library.
		*
		* @return Returns true iff all the tests passed.
		*/
#ifdef BIOSKY_TESTING
		BIOSKY_API bool BIOSkyTests();
#else
		BIOSKY_API bool BIOSkyTests();
#endif 

	}//end namespace SKY
}//end namespace BIO

inline float BIO::SKY::CalculateCelestialNorthPoleZenith(float latitude)
{
	//const float RA = 0.0f;
	//const float Dec = MATH::PId2f;//radians

	//float x = 0.0f; //= cos(HA) * cos(Dec); cos(Dec) = cos(PI/2) = 0.0
	//float y = 0.0f; //= sin(HA) * cos(Dec); cos(Dec) = cos(PI/2) = 0.0
	//const float z = 1.0f; //=sin(Dec); sin(Dec) = sin(PI/2) = 1.0

	//float xhor = 0.0f - z * cos(latitude); //= x * sin(latitude) - z * cos(latitude);
	//float yhor = 0.0f;//=y
	//float zhor = sin(latitude); //= x * cos(latitude) + z * sin(latitude) = 0 * cos(...) + 1.0 * sin(lat) = 0 + sin(lat = sin(lat)

	//const float azimuth = MATH::PIf; //= atan2(yhor, xhor) + MATH::PIf; atan(0, anything) == 0
	//float altitude = latitude; // = asin(zhor) = asin(sin(latitude)) = latitude

	//return SkyPosition(MATH::PIf, MATH::PId2f - latitude);//azimuth, MATH::PId2f - altitude);
	return MATH::PId2f - latitude;
}

inline float BIO::SKY::CalculateJulianDay(DATE_MONTH month, unsigned int day, int year)
{
	int monthInt = Date::MonthToInt(month);

	if (monthInt <= 2)
	{
		year -= 1;
		monthInt += 12;
	}

	int A = 0;
	int B = 0;

	if (year > 1582)
	{
		A = (int)(year / 100);
		B = 2 - A + (int)(A / 4);
	}

	return (int)(365.25*(year + 4716)) + (int)(30.6001 * (monthInt + 1)) + day + B - 1524.5f;
}

inline int BIO::SKY::DaysSinceJan02000(DATE_MONTH month, unsigned int day, unsigned int year)
{
	int monthint = Date::MonthToInt(month);

	int d = (367 * (int)year - (7 * ((int)year + ((monthint + 9) / 12))) / 4 + (275 * monthint) / 9 + (int)day - 730530);

	return d;
}

#endif //___BIOSKY_BIOSKYFUNCTIONS_HPP__2015___