/**
* @file CompileConfig.h
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines all the compilation configurations that will be used to build this 
* project.
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

#ifndef ___BIOSKY_COMPILECONFIG_H__2015___
#define ___BIOSKY_COMPILECONFIG_H__2015___

#define _BIOSKY_PLATFORM_WINDOWS_	1
#define _BIOSKY_PLATFORM_LINUX_		2
#define _BIOSKY_PLATFORM_MAC_		3

//WIN32 for Windows32
//WIN64 for Windows64
#if defined(_WIN32) || \
	defined(_WIN64) || \
	defined(WIN32) || \
	defined(WIN64)

	#ifdef _BIOSKY_PLATFORM_
		#undef _BIOSKY_PLATFORM_
	#endif
	#define _BIOSKY_PLATFORM_ _BIOSKY_PLATFORM_WINDOWS_
#endif

#if defined(__linux)
	#ifdef _BIOSKY_PLATFORM_
		#undef _BIOSKY_PLATFORM_
	#endif
	#define _BIOSKY_PLATFORM_ _BIOSKY_PLATFORM_LINUX_
#endif 

#if _BIOSKY_PLATFORM_ == _BIOSKY_PLATFORM_WINDOWS_
	#ifndef _BIOSKY_STATIC_LIB_
		#ifdef BIOSKY_EXPORTS
			#define BIOSKY_API __declspec(dllexport)
		#else
			#define BIOSKY_API __declspec(dllimport)
		#endif //BIOSKY_EXPORTS
	#else //_BIOSKY_STATIC_LIB_ was defined
		//BIOSKY_API needs to do nothing
		#define BIOSKY_API
	#endif //_BIOSKY_STATIC_LIB_
#elif _BIOSKY_PLATFORM_ == _BIOSKY_PLATFORM_LINUX_
	//BIOSKY_API needs to do nothing
	#define BIOSKY_API
#endif //_BIOSKY_PLATFORM_

//COPIED DIRECTLY FROM stddef.h
//This is here just in case in some file
//doesn't have a heder with NULL in it.
/* Define NULL pointer value */
#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else  /* __cplusplus */
		#define NULL    ((void *)0)
	#endif  /* __cplusplus */
#endif  /* NULL */

//Do we include tests... They are off by default
#ifdef BIOSKY_INCLUDE_TESTS
#define BIOSKY_TESTING 1
#else //testing off
#define BIOSKY_TESTING 0
#endif //BIOSKY_INCLUDE_TESTS

#endif //___BIOSKY_COMPILECONFIG_H__2015___