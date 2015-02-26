/**
* @file Vector3D.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines a 3D vector class.
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

#ifndef ___BIOSKY_VECTOR3D_HPP__2015___
#define ___BIOSKY_VECTOR3D_HPP__2015___

#include "CompileConfig.h"

namespace BIO
{
	namespace SKY
	{
		/**
		* A 3D Vector class that is capable of representing points/directions
		* in 3 Dimensional space. This is a simple vector class and is not
		* suppose to be a complete Vector 3D class with all the vector 
		* operations.
		*/
		class Vector3D
		{
		public:
			/**The X coordinate of the vector.*/
			float X;
			/**The Y coordinate of the vector.*/
			float Y;
			/**The Z coordinate of the vector.*/
			float Z;

			/**
			* Default Constructor
			*/
			BIOSKY_API Vector3D();

			/**
			* Constructor
			*
			* @param x the X coordinate.
			*
			* @param y the Y coordinate.
			*
			* @param z the z coordinate.
			*/
			BIOSKY_API Vector3D(float x, float y, float z);

			/**
			* Copy Constructor
			*/
			BIOSKY_API Vector3D(const Vector3D & other);

			/**
			* Assignment operator
			*/
			BIOSKY_API Vector3D & operator = (const Vector3D & other);

			/**
			* Get the X coordinate.
			*
			* @return Returns the X coordinate.
			*/
			//BIOSKY_API float GetX();
			
			/**
			* Get the Y coordinate.
			*
			* @return Returns the Y coordinate.
			*/
			//BIOSKY_API float GetY();

			/**
			* Get the Z coordinate.
			*
			* @return Returns the Z coordinate.
			*/
			//BIOSKY_API float GetZ();

			/**
			* Set the X coordinate.
			*
			* @param x The new X coordinate.
			*/
			//BIOSKY_API void SetX(float x);

			/**
			* Set the Y coordinate.
			*
			* @param y The new Y coordinate.
			*/
			//BIOSKY_API void SetY(float y);

			/**
			* Set the Z coordinate.
			*
			* @param z The new Z coordinate.
			*/
			//BIOSKY_API void SetZ(float z);
		};
	}//end namespace SKY
}//end namespace BIO

//implementation
inline BIO::SKY::Vector3D::Vector3D() : X(0.0f), Y(0.0f), Z(0.0f)
{}

inline BIO::SKY::Vector3D::Vector3D(float x, float y, float z) : X(x), Y(y), Z(z)
{
}

inline BIO::SKY::Vector3D::Vector3D(const Vector3D & other) : X(other.X), Y(other.Y), Z(other.Z)
{
}

inline BIO::SKY::Vector3D & BIO::SKY::Vector3D::operator = (const Vector3D & other)
{
	if (this != &other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}

	return *this;
}
/*
inline float BIO::SKY::Vector3D::GetX()
{
	return X;
}

inline float BIO::SKY::Vector3D::GetY()
{
	return Y;
}

inline float BIO::SKY::Vector3D::GetZ()
{
	return Z;
}

inline void BIO::SKY::Vector3D::SetX(float x)
{
	X = x;
}

inline void BIO::SKY::Vector3D::SetX(float y)
{
	Y = y;
}

inline void BIO::SKY::Vector3D::SetX(float z)
{
	Z = z;
}
//*/

#endif //___BIOSKY_VECTOR3D_HPP__2015___

