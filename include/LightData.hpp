/**
* @file LightData.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* A struct that holds the data for BIOSky light information.
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

#ifndef ___BIOSKY_LIGHTDATA_HPP__2015___
#define ___BIOSKY_LIGHTDATA_HPP__2015___

#include "CompileConfig.h"

namespace BIO
{
	namespace SKY
	{
		/**
		* RGBA Structure;
		*/
		struct RGBA
		{
		public:
			float B;
			float G;
			float R;
			float A;

			/**
			* Default Constructor
			*/
			RGBA() : R(1.0f), G(1.0f), B(1.0f), A(1.0f)
			{}

			/**
			* Constructor
			*/
			RGBA(float r, float g, float b, float a) : R(r), G(g), B(b), A(a)
			{}

			/**
			* Destructor
			*/
			~RGBA()
			{}
		};

		/**
		* Light data holds the information the is needed to set the light data
		* calculated from the BIOSky library.
		*/
		struct LightData
		{
		public:
			/**
			* Light Color
			*/
			RGBA Color;
			/**
			* Ambient color
			*/
			RGBA AmbientColor;

			/**
			* Constructor
			*/
			BIOSKY_API LightData();

			/**
			* Destructor
			*/
			BIOSKY_API ~LightData();
		};
	}
}

inline BIO::SKY::LightData::LightData() :
Color(),
AmbientColor(0.4f, 0.4f, 0.4f, 1.0f)
{}

inline BIO::SKY::LightData::~LightData()
{}

#endif //___BIOSKY_LIGHTDATA_HPP__2015___
