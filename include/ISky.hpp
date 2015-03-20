/**
* @file ISky.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines an interface for all sky objects.
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

#ifndef ___BIOSKY_ISKY_HPP__2015___
#define ___BIOSKY_ISKY_HPP__2015___

#include "Error.hpp"
#include "SkyPosition.hpp"

namespace BIO
{
	namespace SKY
	{
		class ISky
		{
		public:
			/**
			* Get the error code for this class. If the return value is 0 or ==
			* BIO::SKY::OK then there is no error. If the value is != 0 then
			* there was an error. Use BIO::SKY::ErrorCodeToString function to
			* get string version of the error.
			*
			* @return Returns the error code.
			*/
			virtual ErrorType GetErrorCode() = 0;

			/**
			* Set the phase of the moon and simultaneously update the moon 
			* texture.
			*
			* @param phase The phase of the moon. This value must be within the
			*			range of [0,360] inclusive. 0 and 360 corrosponds to 
			*			the new moon, 45 to Waxing cresent, 90 first quarter, 
			*			..., 180 to full moon, 135 to waning gibbous, ect.
			*
			* @note See https://www.youtube.com/watch?v=XX4mRAfTkTE for 
			*		detailed explination. See 
			*		http://xnelo.com/images/LunarPhase.png for an image 
			*		explaining the phase angle.
			*/
			virtual void SetMoonPhase(float phase) = 0;

			/**
			* Set the position in the sky of the moon.
			*
			* @param pos A struct containing the position of the moon.
			*/
			virtual void SetMoonPosition(SkyPosition pos) = 0;

			/**
			* Set the position in the sky of the moon.
			*
			* @param lunarAzimuth The angle from north in Radians.
			*
			* @param lunarZenith The angle from the zenith position in Radians.
			*/
			virtual void SetMoonPosition(float lunarAzimuth, float lunarZenith) = 0;

			/**
			* Set the visibility of the moon.
			*
			* @param visibility A value between [0, 1] for the visibility of 
			*			the moon. 0 the moon is completely invisible, and 1
			*			the moon is completely visible.
			*/
			virtual void SetMoonVisibility(float visibility) = 0;

			/**
			* Set the position of the Stars.
			*
			* @param zenith The zenith is the only thing that changes on the 
			*			position of the stars. The azimuth is always pointing 
			*			north and the zenith depends on latitude. The equation 
			*			is (PI/2) - latitude Radians (90 - latitude Degrees). 
			*			This function takes the zenith in Radians.
			*
			* @param rotation The Rotation around the celestial pole.
			*/
			virtual void SetStarPosition(float zenith, float rotation) = 0;

			/**
			* Set the position in the sky of the sun.
			*
			* @param pos A struct containing the position of the sun.
			*/
			virtual void SetSunPosition(SkyPosition pos) = 0;

			/**
			* Set the position in the sky of the sun.
			*
			* @param SolarAzimuth The angle from north.
			*
			* @param SolarZenith The angle from the zenith position.
			*/
			virtual void SetSunPosition(float SolarAzimuth, float SolarZenith) = 0;

			/**
			* Update the sky simulation according to the currently stored time
			* and location in this object.
			*
			* @note By calling this function you are responsible for updating
			*		the DateTime object on your own.
			*/
			virtual void Update() = 0;

			/**
			* Update the sky simulation according to the passed in time. The
			* DateTime object will be updated first... then the sky will be
			* updated according to the current time and location stored in this
			* object.
			*
			* @param deltaTime The time passed since the last call to this
			*			Update function.
			*/
			virtual void Update(float deltaTime) = 0;

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
			virtual void UpdateAllSkyObjects() = 0;

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note If an Update functin is called then this function should 
			*		NOT be called every frame.
			*/
			virtual void UpdateMoonPosition() = 0;

			/**
			* Update the position of the North Star according to the current
			* latitude of the observer.
			*
			* @note If an Update functin is called then this function should 
			*		NOT be called every frame.
			*/
			virtual void UpdateStarPosition() = 0;

			/**
			* Update the rotation of the stars in the sky. This depends on the 
			* current time and longitude.
			*
			* @note If an Update functin is called then this function should 
			*		NOT be called every frame.
			*/
			virtual void UpdateStarRotation() = 0;

			/**
			* Update the sky color.
			*
			* @note If an Update functin is called then this function should 
			*		NOT be called every frame.
			*
			* @note In classes that derive from BIO::SKY::IBIOSkyStatic this
			*		function will not be automatically updated. If you change
			*		the sun's position then you need to call this function if
			*		you want the sky colors to be updated to the suns position.
			*		In sky classes that derive from BIO::SKY::BIOSkyDynamic then
			*		the updating of the sky is automatically taken care of in
			*		when either of the IBIOSky::Update functions are called.
			*/
			virtual void UpdateSkyColor() = 0;

			/**
			* Update the SunPosition with the current parameters.
			*
			* @note If an Update functin is called then this function should 
			*		NOT be called every frame.
			*/
			virtual void UpdateSunPosition() = 0;
		};
	}
}

#endif //___BIOSKY_ISKY_HPP__2015___