/**
* @file Vector2D.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines a 2D vector class.
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

#ifndef ___BIOSKY_VECTOR2D_HPP__2015___
#define ___BIOSKY_VECTOR2D_HPP__2015___

#include "CompileConfig.h"

namespace BIO
{
	/**
	* A 3D Vector class that is capable of representing points/directions
	* in 3 Dimensional space. This is a simple vector class and is not
	* suppose to be a complete Vector 3D class with all the vector
	* operations.
	*/
	class Vector2D
	{
	public:
		/**The X coordinate of the vector.*/
		float X;
		/**The Y coordinate of the vector.*/
		float Y;

		/**
		* Default Constructor
		*/
		BIOSKY_API Vector2D();

		/**
		* Constructor
		*
		* @param x the X coordinate.
		*
		* @param y the Y coordinate.
		*/
		BIOSKY_API Vector2D(float x, float y);

		/**
		* Copy Constructor
		*/
		BIOSKY_API Vector2D(const Vector2D & other);

		/**
		* Assignment operator
		*/
		BIOSKY_API Vector2D & operator = (const Vector2D & other);
	};
}//end namespace BIO

//implementation
inline BIO::Vector2D::Vector2D() : X(0.0f), Y(0.0f)
{}

inline BIO::Vector2D::Vector2D(float x, float y) : X(x), Y(y)
{
}

inline BIO::Vector2D::Vector2D(const Vector2D & other) : X(other.X), Y(other.Y)
{
}

inline BIO::Vector2D & BIO::Vector2D::operator = (const Vector2D & other)
{
	if (this != &other)
	{
		X = other.X;
		Y = other.Y;
	}

	return *this;
}

#endif //___BIOSKY_VECTOR2D_HPP__2015___