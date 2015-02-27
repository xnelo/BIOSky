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
#include "Date.hpp"
#include "RawGeometry.hpp"

//define library public functions
namespace BIO
{
	namespace SKY
	{
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
		*			what this parameter represents. Default = 6
		*
		* @return Returns a pointer to a Geometry Object. This object was
		*			created on the heap so you are responsible for deleting
		*			it after calling this function.
		*/
		BIOSKY_API RawGeometry * CreateSkyDomeGeometry(float radius = 1.0f, int numVerticalSegments = 12, int numHorozontalSegments = 6);

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

	}//end namespace SKY
}//end namespace BIO

#endif //___BIOSKY_BIOSKYFUNCTIONS_HPP__2015___