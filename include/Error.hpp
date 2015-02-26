/**
* @file Error.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines the interface for the Skydome Geometry.
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

#ifndef ___BIOSKY_ERROR_HPP__2015___
#define ___BIOSKY_ERROR_HPP__2015___

#include <string>

namespace BIO
{
	namespace SKY
	{
		typedef int ErrorType;

		const ErrorType OK = 0;
		const ErrorType BIOSKY_FAILED_TO_INIT__GEOMETRY_NULL = -1;

		/**
		* Converts an error code into a legible string.
		*
		* @param errorCode The error code to look up.
		*
		* @return Returns a string with the error code string in it.
		*/
		std::string ErrorCodeToString(ErrorType errorCode);
	}//end namespace SKY
}//end namespace BIO

inline std::string BIO::SKY::ErrorCodeToString(ErrorType errorCode)
{
	switch (errorCode)
	{
	case BIOSKY_FAILED_TO_INIT__GEOMETRY_NULL:
		return "BIOSky class failed to Initialize: The passed in Geometry was NULL.";
	case OK:
		return "OK";
	default:
		return "Error Code Unrecognized.";
	}
}

#endif //___BIOSKY_ERROR_HPP__2015___





