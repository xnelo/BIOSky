/**
* @file BIOSky.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* This include file will included all the needed header files for the BIOSky 
* library. Just include this file and you are ready to use the library.
*
* This file also has functions that are global to the library.
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

#ifndef ___BIOSKY_BIOSKY_HPP__2015___
#define ___BIOSKY_BIOSKY_HPP__2015___

//compile configurations
#include "CompileConfig.h"
//constants
#include "MathUtils.hpp"
#include "Error.hpp"
//Independent helper classes
#include "SkyPosition.hpp"
#include "SkyData.hpp"
#include "RawGeometry.hpp"
#include "Vector3D.hpp"
#include "Vector2D.hpp"
#include "Date.hpp"
#include "DateTime.hpp"
#include "GPS.hpp"
#include "LightData.hpp"
//Interfaces
#include "IDomeVertecies.hpp"
#include "IDomeGeometry.hpp"
#include "ISky.hpp"
//Sky functions
#include "BIOSkyFunctions.hpp"
//Sky classes
#include "Sky.hpp"
#include "SkyManual.hpp"
#include "SkyCalculations.hpp"
#include "SkyCalculated.hpp"
#include "SkyCalculatedStatic.hpp"
#include "SkyCalculatedDynamic.hpp"

#endif //___BIOSKY_BIOSKY_HPP__2015___