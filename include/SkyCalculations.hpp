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
//#include "IDateTime.hpp"
//#include "IGPS.h"

namespace BIO
{
	namespace SKY
	{
		/**
		* A class that will help with the calculations of everything in the sky.
		*/
		class SkyCalculations
		{
		protected:
			/**Is _dateTime pointing to a user variable.*/
			bool _userDateTime;
			/**The Date and Time for the sky calculations.*/
			//IDateTime * _dateTime;
			/**Is _gps pointing to a user variable.*/
			bool _userGPS;
			/**The GPS coordinates on the Earth for the sky calculations.*/
			//IGPS * _gps;

			/**
			* Clear this class an set it to new conditions.
			*/
			void _clear();
		private:
			/**
			* Copy constructor
			* This is hidden so it cannot be used.
			*/
			BIOSKY_API SkyCalculations(const SkyCalculations & other);

			/**
			* Assignment operator
			* This is hidden so it cannot be used.
			*/
			BIOSKY_API SkyCalculations & operator = (const SkyCalculations & other);
		public:
			/**
			* Constructor
			*
			* @note The pointers passed into this class allows you to update 
			*		these values how you see fit. Then the changes will 
			*		automatically be reflected in these calculations.
			* 
			* @param dateTime A pointer to the datetime object that these 
			*			calculations will be using. This is a pointer and you
			*			will be in charge of cleaning up the memory. This class
			*			does no memory management on objects you pass in.
			*
			* @param GPS A pointer to the GPS object that these calculations 
			*			will be using. This is a pointer and you will be in 
			*			charge of cleaning up the memory. This class does no 
			*			memory managment.
			*/
			//BIOSKY_API SkyCalculations(IDateTime * dateTime, IGPS * GPS);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyCalculations();

			/**
			* Get the date and time.
			*
			* @return Returns a pointer to this classes date and time object.
			*/
			//BIOSKY_API IDateTime * GetDateTime();

			/**
			* Get the GPS coordinates.
			*
			* @return Returns a pointer to this classes GPS coordinates.
			*/
			//BIOSKY_API IGPS * GetGPS();

			/**
			* Set the data and time object for this class.
			*
			* @param dateTime The IDateTime object this class will use.
			*/
		};
	}//end namespace SKY
}//end namespace BIO

//inline BIO::SKY::IDateTime * BIO::SKY::SkyCalculations::GetDateTime()
//{
//	return _dateTime;
//}

//inline BIO::SKY::IGPS * BIO::SKY::SkyCalculations::GetGPS()
//{
//	return _gps;
//}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//				Private Functions
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//inline BIO::SKY::SkyCalculations::SkyCalculations(const SkyCalculations & other) : _userDateTime(false), _dateTime(NULL), _userGPS(false), _gps(NULL)
//{
//}

inline BIO::SKY::SkyCalculations & BIO::SKY::SkyCalculations::operator = (const SkyCalculations & other)
{
	return *this;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			End Private Functions
///////////////////////////////////////////////////////////////////
#endif //___BIOSKY_SKYCALCULATIONS_HPP__2015___