/**
* @file SkyStaticCalculated.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Definition of the BIOSkyStaticCalculated class.
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

#include "SkyStaticCalculated.hpp"

namespace BIO
{
	namespace SKY
	{
		SkyStaticCalculated::SkyStaticCalculated(IDomeGeometry * geometry, DateTime * dateTime, GPS * gps) :
			SkyStatic(geometry),
			SkyCalculations(dateTime, gps)
		{
			UpdateSunPosition();

			UpdateMoonPosition();

			UpdateSkyColor();
		}

		SkyStaticCalculated::~SkyStaticCalculated()
		{}
	}//end namespace SKY
}//end namespace BIO


