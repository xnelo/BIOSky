/**
* @file SkyCalculations.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Contains all the calculations that need to be made for a sky.
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

#ifndef ___BIOSKY_SKYCALCULATIONS_HPP__2015___
#define ___BIOSKY_SKYCALCULATIONS_HPP__2015___

#include "CompileConfig.h"

namespace BIO
{
	namespace SKY
	{
		/**
		* A class that will help with the calculations of everything in the sky.
		*/
		class SkyCalculations
		{
		public:
			/**
			* Constructor
			*/
			BIOSKY_API SkyCalculations();

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyCalculations();
		};
	}//end namespace SKY
}//end namespace BIO

#endif //___BIOSKY_SKYCALCULATIONS_HPP__2015___