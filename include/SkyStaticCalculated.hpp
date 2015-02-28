/**
* @file SkyStaticCalculated.hpp
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

#ifndef ___BIOSKY_SKYSTATICCALCULATED_HPP__2015___
#define ___BIOSKY_SKYSTATICCALCULATED_HPP__2015___

#include "CompileConfig.h"
#include "SkyStatic.hpp"
#include "SkyCalculations.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* An implementation of the IBIOSkyStatic interface. This class will 
		* calculate sun and moon position based on time, date, and position on
		* the earth.
		*/
		class SkyStaticCalculated : public SkyStatic, public SkyCalculations
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
			BIOSKY_API SkyStaticCalculated(IDomeGeometry * geometry, DateTime * dateTime, GPS * gps);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyStaticCalculated();

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateMoonPosition();

			BIOSKY_API virtual void UpdateStarPosition();
			BIOSKY_API virtual void UpdateStarRotation();

			/**
			* Update the SunPosition with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateSunPosition();
		};
	}//end namespace SKY
}//end namespace BIO

inline void BIO::SKY::SkyStaticCalculated::UpdateMoonPosition()
{
	SetMoonPosition(CalculateMoonPosition());
}

inline void BIO::SKY::SkyStaticCalculated::UpdateStarPosition()
{
	SetStarPosition(CalculateCelestialNorthZenith(), CalculateStarRotation());
}

inline void BIO::SKY::SkyStaticCalculated::UpdateStarRotation()
{
	SetStarPosition(CalculateCelestialNorthZenith(), CalculateStarRotation());
}

inline void BIO::SKY::SkyStaticCalculated::UpdateSunPosition()
{
	SetSunPosition(CalculateSunPosition());
}

#endif //___BIOSKY_SKYSTATICCALCULATED_HPP__2015___