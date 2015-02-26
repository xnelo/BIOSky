/**
* @file RawGeometry.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This file contains the class for a static sky where the attributes of the sky
* are manually placed by the creator of this class.
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

#ifndef ___BIOSKY_RAWGEOMETRY_HPP__2015___
#define ___BIOSKY_RAWGEOMETRY_HPP__2015___

#include "CompileConfig.h"
#include "Vector3D.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* RawGeometry class holds geometry data. Indecies, vertecies and UV 
		* coordinates.
		*/
		class RawGeometry
		{
		public:

			/**A stuct for a 2D vector*/
			struct Vector2D
			{
				float x, y;
			};

			/**The indecies for the geometry*/
			unsigned short * indecies;
			/**Number of indecies.*/
			int numIndecies;
			/**Vertex corrdinates data*/
			Vector3D *vertecies;
			/**UV Coordinates.*/
			RawGeometry::Vector2D *UVTextureCoordinates;
			/**Number of vertecies.*/
			int numVertecies;

			/**
			* Default Constructor
			*/
			RawGeometry();

			/**
			* Destructor
			*/
			~RawGeometry();
		};
	}//end namespace SKY
}// end namespace BIO

inline BIO::SKY::RawGeometry::RawGeometry() : indecies(NULL), numIndecies(0), vertecies(NULL), UVTextureCoordinates(NULL), numVertecies(0)
{}

inline BIO::SKY::RawGeometry::~RawGeometry()
{
	if (indecies)
	{
		delete[] indecies;
		indecies = NULL;
	}

	if (vertecies)
	{
		delete[] vertecies;
		vertecies = NULL;
	}

	if (UVTextureCoordinates)
	{
		delete[] UVTextureCoordinates;
		UVTextureCoordinates = NULL;
	}

	numIndecies = 0;
	numVertecies = 0;
}

#endif //___BIOSKY_RAWGEOMETRY_HPP__2015___