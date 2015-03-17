/**
* @file SkyManual.hpp
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

#ifndef ___BIOSKY_SKYMANUAL_HPP__2015___
#define ___BIOSKY_SKYMANUAL_HPP__2015___

#include "CompileConfig.h"
#include "Sky.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* This sky type has the attributes manually set by the user.
		*/
		class SkyManual : public Sky
		{
		public:
			/**
			* Constructor
			*
			* @param skydome A pointer to the rendering skydome geometry.
			*
			* @param sunAzimuth The azimuth of the sun.
			*
			* @param sunZenith The zenith of the sun.
			*
			* @param moonAzimuth The azimuth of the moon.
			*
			* @param moonZenith The zenith of the moon.
			*/
			SkyManual(IDomeGeometry * skydome, float sunAzimuth, float sunZenith, float moonAzimuth, float moonZenith);

			/**
			* Destructor
			*/
			virtual ~SkyManual();

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
			virtual void UpdateAllSkyObjects();

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			virtual void UpdateMoonPosition();

			/**
			* Update the position of the North Star according to the current
			* latitude of the observer.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			virtual void UpdateStarPosition();

			/**
			* Update the rotation of the stars in the sky. This depends on the
			* current time and longitude.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			virtual void UpdateStarRotation();

			/**
			* Update the SunPosition with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			virtual void UpdateSunPosition();
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyManual::SkyManual(IDomeGeometry * skydome, float sunAzimuth, float sunZenith, float moonAzimuth, float moonZenith) : Sky(skydome)
{
	SetSunPosition(sunAzimuth, sunZenith);

	SetMoonPosition(moonAzimuth, moonZenith);

	//SetStarPosition(.855f, .785f);

	UpdateSkyColor();
}

inline BIO::SKY::SkyManual::~SkyManual()
{
}

inline void BIO::SKY::SkyManual::Update()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::Update(float deltaTime)
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::UpdateAllSkyObjects()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::UpdateMoonPosition()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::UpdateStarPosition()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::UpdateStarRotation()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyManual::UpdateSunPosition()
{
	/*DO NOTHING*/
}

#endif //___BIOSKY_SKYMANUAL_HPP__2015___