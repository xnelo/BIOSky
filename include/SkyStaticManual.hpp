/**
* @file SkyStaticManual.hpp
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

#ifndef ___BIOSKY_SKYSTATICMANUAL_HPP__2015___
#define ___BIOSKY_SKYSTATICMANUAL_HPP__2015___

#include "CompileConfig.h"
#include "SkyStatic.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* This sky type has the attributes manually set by the user.
		*/
		class SkyStaticManual : public SkyStatic
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
			BIOSKY_API SkyStaticManual(IDomeGeometry * skydome, float sunAzimuth, float sunZenith, float moonAzimuth, float moonZenith);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyStaticManual();
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyStaticManual::SkyStaticManual(IDomeGeometry * skydome, float sunAzimuth, float sunZenith, float moonAzimuth, float moonZenith) : SkyStatic(skydome)
{
	SetSunPosition(sunAzimuth, sunZenith);

	SetMoonPosition(moonAzimuth, moonZenith);

	UpdateSkyColor();
}

inline BIO::SKY::SkyStaticManual::~SkyStaticManual()
{
}

#endif //___BIOSKY_SKYSTATICMANUAL_HPP__2015___