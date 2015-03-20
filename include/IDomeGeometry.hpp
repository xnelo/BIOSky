/**
* @file IDomeGeometry.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines the interface for the Skydome Geometry.
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

#ifndef ___BIOSKY_IDOMEGEOMETRY_HPP__2015___
#define ___BIOSKY_IDOMEGEOMETRY_HPP__2015___

#include "IDomeVertecies.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* An interface allowing users to use any rendering system they like 
		* while allowing this library to update that geometry as needed.
		*/
		class IDomeGeometry
		{
		public:
			/**
			* Return a class that inherits from IDomeVertecies. This class 
			* should contain, or at least be able to access, the vertex data 
			* for this geometry. You are required to implement the 
			* IDomeVertecies interface and in the implementation of this 
			* function return a pointer to the structure you created. You are 
			* also required to manage the memory. The BIOSky library will not
			* delete the returned object when it is done with it, you are 
			* required to do that. A perfect place would be in the 
			* UnlockGeometry function.
			*
			* @return Returns an object that inherits from IDomeVertecies and 
			*			has access to all the domes vertecies.
			*/
			virtual IDomeVertecies * GetVertecies() = 0;

			/**
			*
			*/
			virtual unsigned char * GetMoonTexturePixels() = 0;

			/**
			* This function is the request for the BIOSky to take control of 
			* the geometry buffers. When this is called the BIOSky library will
			* be editing the Skydome geometry until the UnlockGeometry function
			* is called.
			*
			* @note You may or may not need to do anything or even keep track
			* of when these functions are called. This is for graphic drivers
			* and enginges that need to lock the geometry buffers. Some locks 
			* will not happen until you return the vertex data. The BIOSky 
			* library will call LockGeometry before it edits skydome geometry 
			* and will call UnlockGeometry when it is done.
			*/
			virtual void LockGeometry() = 0;

			/**
			*
			*/
			virtual void LockMoonTexture() = 0;

			/**
			* Set Position of the moon.
			*
			* @param unitX The unit x position of the moon. This is the position
			*			on the unit circle(circle with radius of 1). When you
			*			implement this function you should multiply by the
			*			radius of the dome.
			*
			* @param unitY The unit y position of the moon. This is the position
			*			on the unit circle(circle with radius of 1). When you
			*			implement this function you should multiply by the
			*			radius of the dome.
			*
			* @param unitZ The unit z position of the moon. This is the position
			*			on the unit circle(circle with radius of 1). When you
			*			implement this function you should multiply by the
			*			radius of the dome.
			*/
			virtual void SetMoonPosition(float unitX, float unitY, float unitZ) = 0;

			/**
			* Set the rotation of the Stars.
			*/
			virtual void SetStarRotation(float x, float y, float z) = 0;

			/**
			* Set Position of the sun.
			*
			* @param unitX The unit x position of the sun. This is the position 
			*			on the unit circle(circle with radius of 1). When you 
			*			implement this function you should multiply by the 
			*			radius of the dome.
			*
			* @param unitY The unit y position of the sun. This is the position 
			*			on the unit circle(circle with radius of 1). When you 
			*			implement this function you should multiply by the 
			*			radius of the dome.
			*
			* @param unitZ The unit z position of the sun. This is the position 
			*			on the unit circle(circle with radius of 1). When you 
			*			implement this function you should multiply by the 
			*			radius of the dome.
			*/
			virtual void SetSunPosition(float unitX, float unitY, float unitZ) = 0;

			/**
			* When this function is called this means that the BIOSky library 
			* has finished editing the skydome geometry and has released all
			* pointers to the geometry and buffers.
			*
			* @note You may or may not need to do anything or even keep track
			* of when these functions are called. This is for graphic drivers
			* and enginges that need to lock the geometry buffers. Some locks 
			* will not happen until you return the vertex data. The BIOSky 
			* library will call LockGeometry before it edits skydome geometry 
			* and will call UnlockGeometry when it is done.
			*/
			virtual void UnlockGeometry() = 0;

			/**
			*
			*/
			virtual void UnlockMoonTexture() = 0;
		};
	}//end namespace SKY
}//end namespace BIO

#endif //___BIOSKY_IDOMEGEOMETRY_HPP__2015___