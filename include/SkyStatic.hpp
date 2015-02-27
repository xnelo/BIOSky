/**
* @file SkyStatic.hpp
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

#ifndef ___BIOSKY_SKYSTATIC_HPP__2015___
#define ___BIOSKY_SKYSTATIC_HPP__2015___

#include "CompileConfig.h"
#include "Sky.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* This defines the interface for a static BIOSky. This sky doesn't 
		* change.
		*/
		class SkyStatic : public Sky
		{
		public:
			/**
			* Constructor
			*/
			BIOSKY_API SkyStatic(IDomeGeometry * skydome);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyStatic();

			/**
			* Update the sky simulation according to the currently stored time
			* and location in this object.
			*
			* @note By calling this function you are responsible for updating
			*		the DateTime object on your own.
			*/
			BIOSKY_API virtual void Update();

			/**
			* Update the sky simulation according to the passed in time. The
			* DateTime object will be updated first... then the sky will be
			* updated according to the current time and location stored in this
			* object.
			*
			* @param deltaTime The time passed since the last call to this
			*			Update function.
			*/
			BIOSKY_API virtual void Update(float deltaTime);

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateMoonPosition();

			/**
			* Update the SunPosition with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateSunPosition();
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyStatic::SkyStatic(IDomeGeometry * skydome) : Sky(skydome)
{}

inline BIO::SKY::SkyStatic::~SkyStatic()
{}

inline void BIO::SKY::SkyStatic::Update()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyStatic::Update(float deltaTime)
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyStatic::UpdateMoonPosition()
{
	/*DO NOTHING*/
}

inline void BIO::SKY::SkyStatic::UpdateSunPosition()
{
	/*DO NOTHING*/
}

#endif //___BIOSKY_SKYSTATIC_HPP__2015___