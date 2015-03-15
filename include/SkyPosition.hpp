/**
* @file SkyPosition.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Defines a class that holds the Horozontal or (Azimuth/Zenith) coordinates for
* a position in the sky.
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

#ifndef ___BIOSKY_SKYPOSITION_HPP__2015___
#define ___BIOSKY_SKYPOSITION_HPP__2015___

#include "CompileConfig.h"

namespace BIO
{
	namespace SKY
	{
		/**
		* Holds an Azimuth and Zenith angle. The Zenith angle is the angle from
		* straight up and the Azimuth angle is the angle from North.
		*/
		struct SkyPosition
		{
		public:
			float Azimuth;
			float Zenith;

			/**
			* Default Constructor
			*/
			BIOSKY_API SkyPosition();

			/**
			* Constructor
			*
			* @param azimuth The azimuth angle.
			* @param zenith The zenith angle.
			*/
			BIOSKY_API SkyPosition(float azimuth, float zenith);


#if BIOSKY_TESTING == 1
			/**
			* Test this class.
			*
			* @param test A pointer to a Test class that holds all the function
			*				for testing and will hold all the results of the 
			*				testing.
			*
			* @return Returns true iff all the tests pass.
			*/
			static bool Test(XNELO::TESTING::Test * test);
#endif
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyPosition::SkyPosition() : 
Azimuth(0.0f), 
Zenith(0.0f)
{}

inline BIO::SKY::SkyPosition::SkyPosition(float azimuth, float zenith) : 
Azimuth(azimuth), 
Zenith(zenith)
{}

#if BIOSKY_TESTING == 1
inline bool BIO::SKY::SkyPosition::Test(XNELO::TESTING::Test * test)
{
	test->SetName("SkyPosition Tests");

	float tolerance = 0.005f;

	SkyPosition skyPos;
	SkyPosition skyPos2(2.34f, 1.23f);

	test->UnitTest(((skyPos.Azimuth == 0.0f) && 
					(skyPos.Zenith == 0.0f)) == true, "Test Default constructor");

	test->UnitTest(skyPos2.Azimuth, 2.34f, tolerance, "Test constructor[1]");
	test->UnitTest(skyPos2.Zenith, 1.23f, tolerance, "Test constructor[2]");

	return test->GetSuccess();
}
#endif

#endif //___BIOSKY_SKYPOSITION_HPP__2015___


