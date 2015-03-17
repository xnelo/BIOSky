/**
* @file SkyCalculatedStatic.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This file contains the class for a static sky where the attributes of the sky
* are calculated according to time and date.
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

#ifndef ___BIOSKY_SKYCALCULATEDSTATIC_HPP__2015___
#define ___BIOSKY_SKYCALCULATEDSTATIC_HPP__2015___

#include "CompileConfig.h"
#include "SkyCalculated.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* An implementation of the IBIOSkyStatic interface. This class will 
		* calculate sun and moon position based on time, date, and position on
		* the earth.
		*/
		class SkyCalculatedStatic : public SkyCalculated
		{
		public:
			/**
			* Constructor
			*
			* @param geometry A pointer to the geometry class that renders the 
			*			skydome.
			*
			* @param dateTime A pointer to the DateTime object that keeps track 
			*			of the current time. This allows for the user to update
			*			the date and time in his own application and it is 
			*			automatically updated in the library.
			*
			* @param gps A pointer to the GPS object the keeps track of the 
			*			current GPS location. This allows the user to update
			*			the GPS location in his own application and it is 
			*			automatically updated in the library.
			*/
			SkyCalculatedStatic(IDomeGeometry * geometry, DateTime * dateTime, GPS * gps);

			/**
			* Destructor
			*/
			virtual ~SkyCalculatedStatic();

			/**
			* Update the sky simulation according to the currently stored time
			* and location in this object.
			*
			* @note By calling this function you are responsible for updating
			*		the DateTime object on your own.
			*/
			virtual void Update();

			/**
			* Update the sky simulation according to the passed in time. The
			* DateTime object will be updated first... then the sky will be
			* updated according to the current time and location stored in this
			* object.
			*
			* @param deltaTime The time passed since the last call to this
			*			Update function.
			*/
			virtual void Update(float deltaTime);
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyCalculatedStatic::SkyCalculatedStatic(IDomeGeometry * geometry, DateTime * dateTime, GPS * gps) :
SkyCalculated(geometry, dateTime, gps)
{
	UpdateAllSkyObjects();
}

inline BIO::SKY::SkyCalculatedStatic::~SkyCalculatedStatic()
{}

inline void BIO::SKY::SkyCalculatedStatic::Update()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyCalculatedStatic::Update(float deltaTime)
{
	/*DO NOTHING*/
}

#endif //___BIOSKY_SKYCALCULATEDSTATIC_HPP__2015___