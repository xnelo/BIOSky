/**
* @file Error.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines error codes for the BIOEngine Library.
*/
/*
* The zlib/libpng License
*
* Copyright (c) 2014 Spencer Hoffa
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

#ifndef ___BIOENGINE_ERROR_H__2014___
#define ___BIOENGINE_ERROR_H__2014___

#include <string>

namespace BIO
{
	/**Define the ErrorType. Right now it is an integer.*/
	typedef int ErrorType;

	const ErrorType OK = 0;
	const ErrorType RENDERER_DEVICE_FAILED_TO_INIT = -1;
	const ErrorType LEVEL_FAILED_TO_LOAD__INVALID_FORMAT = -2;
	const ErrorType LEVEL_FAILED_TO_LOAD__FILE_DOESNT_EXIST = -3;
	const ErrorType ERROR_CREATING_OBJECT = -4;
	const ErrorType TERRAIN_CREATION_ERROR = -5;
	const ErrorType BIOSKY_FAILED_TO_INIT__GEOMETRY_NULL = -6;

	/**
	* Look up the string explination of an error code.
	*
	* @param errorCode The Code that we need to look up.
	*
	* @return Returns a string with the explination of the error code.
	*/
	inline std::string ErrorCodeToString(ErrorType errorCode)
	{
		switch (errorCode)
		{
		case LEVEL_FAILED_TO_LOAD__FILE_DOESNT_EXIST:
			return "Level Failed to Load: File Doesn't Exist";
		case LEVEL_FAILED_TO_LOAD__INVALID_FORMAT:
			return "Level Failed to Load: Invalid Format";
		case RENDERER_DEVICE_FAILED_TO_INIT:
			return "Rendering Device Failed to Initialize";
		case ERROR_CREATING_OBJECT:
			return "Object Failed to load.";
		case BIOSKY_FAILED_TO_INIT__GEOMETRY_NULL:
			return "Sky Failed To Init: Geometry NULL";
		case OK:
			return "OK";
		default:
			return "ERROR CODE UNRECOGNIZED";
		}
	}

}//end namespace BIO

#endif //___BIOENGINE_ERROR_H__2014___