/**
* @file SkyData.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This holds all of the data used in sky positioning.
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

#ifndef ___BIOSKY_SKYDATA_HPP__2015___
#define ___BIOSKY_SKYDATA_HPP__2015___

#include "CompileConfig.h"
#include "SkyPosition.hpp"

namespace BIO
{
	namespace SKY
	{
		/**
		* This structure holds all of the data needed to set sun and moon 
		* positions, star rotations, north star zenith, and moon phase.
		*/
		struct SkyData
		{
		public:
			/**The moon's position*/
			SkyPosition moonPos;
			/**The sun's position*/
			SkyPosition sunPos;
			/**Star rotation around celestial North Pole, in radians.*/
			float starRotation;
			/**
			* The north star's zenith angle in radians. The azimuth is always 
			* north. The zenith could be negative and will be in the southern 
			* hemisphere.
			*/
			float northStarZenith;
			/**
			* Moon's phase as a decimal. It is in the range from [0,1]. 0 is 
			* new moon and 1 is full moon.
			*/
			float phase;

			/**
			* Constructor
			*/
			BIOSKY_API SkyData();

			/**
			* Destructor
			*/
			BIOSKY_API ~SkyData();
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyData::SkyData() :
moonPos(),
sunPos(),
starRotation(0.0f),
northStarZenith(0.0f),
phase(1.0f)
{}

inline BIO::SKY::SkyData::~SkyData()
{}

#endif //___BIOSKY_SKYDATA_HPP__2015___


