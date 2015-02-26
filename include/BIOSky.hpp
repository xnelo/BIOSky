/**
* @file BIOSky.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This include file will included all the needed header files for the BIOSky 
* library. Just include this file and you are ready to use the library.
*
* This file also has functions that are global to the library.
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

#ifndef ___BIOSKY_BIOSKY_HPP__2015___
#define ___BIOSKY_BIOSKY_HPP__2015___

//compile configurations
#include "CompileConfig.h"
//constants
#include "Constants.hpp"
#include "Error.hpp"
//Independent helper classes
#include "SkyPosition.hpp"
#include "RawGeometry.hpp"
#include "Vector3D.hpp"
//Interfaces
#include "IDomeVertecies.hpp"
#include "IDomeGeometry.hpp"
#include "ISky.hpp"
//Sky classes
#include "Sky.hpp"
#include "SkyStatic.hpp"
#include "SkyStaticManual.hpp"

//define library public functions
namespace BIO
{ 
	namespace SKY
	{
		/**
		* This function converts the 3D Skydome coordinates to its
		* corosponding UV coordinates.
		*
		* @param domeX The X coordinate on the skydome.
		*
		* @param domeY The Y coordinate on the skydome.
		*
		* @param domeZ The Z coordinate on the skydome.
		*
		* @param domeRadius The Radius of the skydome.
		*
		* @return Returns a BIO2DVector that contains the x(u) y(v)
		*			coordinate on the texture mape of the skydome.
		*/
		//BIOSKY_API Vector2df ConvertDomeToTextureCoordinate(float domeX, float domeY, float domeZ, float domeRadius);

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
		float Distance(float x1, float y1, float x2, float y2);

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
		BIOSKY_API float SquaredDistance(float x1, float y1, float x2, float y2);

	}//end namespace SKY
}//end namespace BIO

inline float BIO::SKY::Distance(float x1, float y1, float x2, float y2)
{
	return sqrt(SquaredDistance(x1, y1, x2, y2));
}

inline float BIO::SKY::SquaredDistance(float x1, float y1, float x2, float y2)
{
	float tmpx, tmpy;
	tmpx = (x2 - x1);
	tmpy = (y2 - y1);

	return (tmpx * tmpx) + (tmpy * tmpy);
}

#endif //___BIOSKY_BIOSKY_HPP__2015___