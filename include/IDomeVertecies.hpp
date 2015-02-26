/**
* @file IDomeVertecies.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines an interface for a class that returns data from the Dome Geometry 
* implementation to the BIOSky Library.
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

#ifndef ___BIOSKY_IDOMEVERTECIES_HPP__2015___
#define ___BIOSKY_IDOMEVERTECIES_HPP__2015___

#include "Vector3D.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* The interface that a user must implement when implementing a new 
		* geometry type. This class will hold all of the vertex data that will
		* be passed back to the BIOSky library so it can modify the geometry.
		*
		* @note This is a pure virtual class.
		*/
		class IDomeVertecies
		{
		public:
			/**
			* This returns the number of vertecies that are stored.
			*
			* @return Returns an int with the number of vertecies.
			*/
			virtual int GetVertexCount() = 0;

			/**
			* Get the position of the vertex referenced by the index. The 
			* passed in index number will always be between 0 and 
			* GetVertexCount() - 1.
			*
			* @param index The index of the vertex we are requesting position
			*			data for. Will always be between 0 and 
			*			GetVertexCount() - 1.
			*
			* @return A Vector3D with the point position contained in it.
			*/
			virtual Vector3D GetVertexPosition(int index) = 0;

			/**
			* Set the Color for the vertex specified by the index. The passed
			* in index will always be between 0 and GetVertexCounter() -1.
			*
			* @param index The index of the vertex we are requesting position
			*			data for. Will always be between 0 and 
			*			GetVertexCount() - 1.
			*
			* @param A The Alpha value for the new color. This is within the
			*			range of [0,255].
			*
			* @param R The Red value for the new color. This is within the
			*			range of [0,255].
			*
			* @param G The Green value for the new color. This is within the
			*			range of [0,255].
			*
			* @param B The Blue value for the new color. This is within the
			*			range of [0,255].
			*/
			virtual void SetVertexColor(int index, int A, int R, int G, int B) = 0;
		};
	}//end namespace SKY
}//end namespace BIO

#endif //___BIOSKY_IDOMEVERTECIES_HPP__2015___


