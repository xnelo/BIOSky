/**
* @file SkyCalculated.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines the interface for a static sky (One that does not change).
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

#ifndef ___BIOSKY_SKYCALCULATED_HPP__2015___
#define ___BIOSKY_SKYCALCULATED_HPP__2015___

#include "CompileConfig.h"
#include "Sky.hpp"
#include "SkyCalculations.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* This defines the interface for a static BIOSky. This sky doesn't 
		* change.
		*/
		class SkyCalculated : public Sky, public SkyCalculations
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
			BIOSKY_API SkyCalculated(IDomeGeometry * skydome, DateTime * dateTime, GPS * gps);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyCalculated();

			/**
			* Update the sky simulation according to the currently stored time
			* and location in this object.
			*
			* @note By calling this function you are responsible for updating
			*		the DateTime object on your own.
			*/
			BIOSKY_API virtual void Update() = 0;

			/**
			* Update the sky simulation according to the passed in time. The
			* DateTime object will be updated first... then the sky will be
			* updated according to the current time and location stored in this
			* object.
			*
			* @param deltaTime The time passed since the last call to this
			*			Update function.
			*/
			BIOSKY_API virtual void Update(float deltaTime) = 0;

			/**
			* Update All the sky objects. Calling this function is equivalent
			* to calling all of the UpdateMoonPosition, UpdateStarPosition,
			* UpdateStarRotation, UpdateSkyColor, and UpdateSunPosition. This
			* function is however slightly optimized so that calculations do
			* not need to be made twice.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateAllSkyObjects();

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateMoonPosition();

			/**
			* Update the position of the North Star according to the current
			* latitude of the observer.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateStarPosition();

			/**
			* Update the rotation of the stars in the sky. This depends on the
			* current time and longitude.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
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

inline BIO::SKY::SkyCalculated::SkyCalculated(IDomeGeometry * skydome, DateTime * dateTime, GPS * gps) :
Sky(skydome), SkyCalculations(dateTime, gps)
{}

inline BIO::SKY::SkyCalculated::~SkyCalculated()
{}

inline void BIO::SKY::SkyCalculated::UpdateAllSkyObjects()
{
	SkyData skyInfo;
	skyInfo = CalculateAllSkyData();

	SetMoonPosition(skyInfo.moonPos);
	SetStarPosition(skyInfo.northStarZenith, skyInfo.starRotation);
	SetSunPosition(skyInfo.sunPos);

	SetMoonPhase(skyInfo.phase);

	UpdateSkyColor();
}

inline void BIO::SKY::SkyCalculated::UpdateMoonPosition()
{
	SetMoonPosition(CalculateMoonPosition());
}

inline void BIO::SKY::SkyCalculated::UpdateStarPosition()
{
	SetStarPosition(CalculateCelestialNorthZenith(), CalculateStarRotation());
}

inline void BIO::SKY::SkyCalculated::UpdateStarRotation()
{
	SetStarPosition(CalculateCelestialNorthZenith(), CalculateStarRotation());
}

inline void BIO::SKY::SkyCalculated::UpdateSunPosition()
{
	SetSunPosition(CalculateSunPosition());
}

#endif //___BIOSKY_SKYCALCULATED_HPP__2015___