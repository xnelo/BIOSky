/**
* @file MathUtils.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This contains math helper functions used in the BIOEngine library.
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

#ifndef ___BIOENGINE_MATHUTILS_H__2014___
#define ___BIOENGINE_MATHUTILS_H__2014___

#include <cmath>

namespace BIO
{
	namespace MATH
	{
		/**Useful constants*/
		const double PI = 3.1415926535897932384626433832795;/**The Constant PI = 3.14159265...*/
		const double PId2 = 3.1415926535897932384626433832795 / 2.0;/**The Constant PI divided by 2*/
		const double PIx2 = 3.1415926535897932384626433832795 * 2.0;/**The Constant PI Times 2*/
		const double DegreesToRadians = PI / 180.0;/**The Constant PI / 180 Degrees*/
		const double RadiansToDegrees = 180.0 / PI;/**The Constant 180 Degrees / PI*/

		//To avoid casting here are the float equivalants.
		const float PIf = 3.1415926535897932384626433832795f;/**(float)The Constant PI = 3.14159265...*/
		const float PId2f = 3.1415926535897932384626433832795f / 2.0f;/**(float)The Constant PI divided by 2*/
		const float PIx2f = 3.1415926535897932384626433832795f * 2.0f;/**(float)The Constant PI Times 2*/
		const float DegreesToRadiansf = PIf / 180.0f;/**(float)The Constant PI / 180 Degrees*/
		const float RadiansToDegreesf = 180.0f / PIf;/**(float)The Constant 180 Degrees / PI*/


		/**
		* Calculate the distance between two points. The equation is:
		*
		* Distance = SquareRoot((x1-x2)^2 + (y1-y2)^2);
		*
		* @param x1 The x coordinate of the first point.
		* 
		* @param y1 The y coordinate of the first point.
		*
		* @param x2 The x coordinate of the second point.
		*
		* @param y2 The y coordinate of the second point.
		*
		* @return Returns the distance between two points.
		*/
		template<typename numType>
		numType Distance(numType x1, numType y1, numType x2, numType y2);

		/**
		* Calculate the distance between two points squared. The equation is:
		*
		* Distance^2 = (x1-x2)^2 + (y1-y2)^2
		*
		* @param x1 The x coordinate of the first point.
		* 
		* @param y1 The y coordinate of the first point.
		*
		* @param x2 The x coordinate of the second point.
		*
		* @param y2 The y coordinate of the second point.
		*
		* @return Returns the squared distance between two points.
		*/
		template<typename numType>
		numType SquaredDistance(numType x1, numType y1, numType x2, numType y2);
	}//end namespace MATH
}//end namespace BIO

template<typename numType>
inline numType BIO::MATH::Distance(numType x1, numType y1, numType x2, numType y2)
{
	return sqrt(SquaredDistance(x1, y1, x2, y2));
}

template<typename numType>
inline numType BIO::MATH::SquaredDistance(numType x1, numType y1, numType x2, numType y2)
{
	numType tmpx, tmpy;
	tmpx = (x2 - x1);
	tmpy = (y2 - y1);

	return (tmpx * tmpx) + (tmpy * tmpy);
}

#endif //___BIOENGINE_MATHUTILS_H__2014___