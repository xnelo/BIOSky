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
#include "DateTime.hpp"
#include "GPS.hpp"
#include "SkyPosition.hpp"
#include "BIOSkyFunctions.hpp"

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
			DateTime * _dateTime;
			/**Is _gps pointing to a user variable.*/
			bool _userGPS;
			/**The GPS coordinates on the Earth for the sky calculations.*/
			GPS * _gps;

			/**
			* Delete all the pointers in this class
			*
			* @note It actually only deletes if this class created the pointer.
			*/
			void _delete();
			
			/**
			* Delete the date time pointer.
			*
			* @note It actually only deletes if this class created the pointer.
			*/
			void _deleteDateTime();

			/**
			* Delete the GPS pointer.
			*
			* @note It actually only deletes if this class created the pointer.
			*/
			void _deleteGPS();
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
			BIOSKY_API SkyCalculations(DateTime * dateTime, GPS * gps);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~SkyCalculations();

			/**
			* Calculate the position of the sun.
			*
			* @return Returns a SkyPosition object with the azimuth and zenith
			*			of the sun's current position.
			*/
			BIOSKY_API SkyPosition CalculateSunPosition();

			/**
			* Get the date and time.
			*
			* @return Returns a pointer to this classes date and time object.
			*/
			BIOSKY_API DateTime * GetDateTime();

			/**
			* Get the GPS coordinates.
			*
			* @return Returns a pointer to this classes GPS coordinates.
			*/
			BIOSKY_API GPS * GetGPS();

			/**
			* Set the data and time object for this class.
			*
			* @param dateTime The DateTime object this class will use.
			*
			* @note The parameter dateTime will not be deleted by this class. 
			*		You are responsible for any memory managment that needs to
			*		occur.
			*/
			BIOSKY_API void SetDateTime(DateTime * dateTime);

			/**
			* Set the date and time for this class.
			*
			* @param dateTime The DateTime to set this class to.
			*/
			BIOSKY_API void SetDateTime(DateTime dateTime);

			/**
			* Set the GPS coordinates for this class.
			*
			* @param gps The GPS object this class will use.
			*
			* @note The parameter gps will not be deleted by this class. You 
			*		are responsible for any memory managment that needs to 
			*		occur.
			*/
			BIOSKY_API void SetGPS(GPS * gps);

			/**
			* Set the GPS coordinates for this clas.
			*
			* @param gps The GPS coordinates to set this class to.
			*/
			BIOSKY_API void SetGPS(GPS gps);
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyPosition BIO::SKY::SkyCalculations::CalculateSunPosition()
{
	return BIO::SKY::CalculateSunPosition(
		_dateTime->GetTimeHours(),	//Time in hours
		_dateTime->GetUTCOffset(),  //Offset from UTC-0 time
		_dateTime->GetMonth(),		//Month of the year
		_dateTime->GetDay(),		//Day of the month
		_dateTime->GetYear(),		//Year
		_gps->GetLatitudeRadians(),	//Latitude in Radians
		_gps->GetLongitudeRadians()	//Longitude in Radians
		);
}

inline BIO::DateTime * BIO::SKY::SkyCalculations::GetDateTime()
{
	return _dateTime;
}

inline BIO::GPS * BIO::SKY::SkyCalculations::GetGPS()
{
	return _gps;
}

inline void BIO::SKY::SkyCalculations::SetDateTime(DateTime * dateTime)
{
	_deleteDateTime();
	_dateTime = dateTime;
}

inline void BIO::SKY::SkyCalculations::SetDateTime(DateTime dateTime)
{
	(*_dateTime) = dateTime;
}

inline void BIO::SKY::SkyCalculations::SetGPS(GPS * gps)
{
	_deleteGPS();
	_gps = gps;
}

inline void BIO::SKY::SkyCalculations::SetGPS(GPS gps)
{
	(*_gps) = gps;
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//				Private Functions
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
inline BIO::SKY::SkyCalculations::SkyCalculations(const SkyCalculations & other) : _userDateTime(false), _dateTime(NULL), _userGPS(false), _gps(NULL)
{
}

inline BIO::SKY::SkyCalculations & BIO::SKY::SkyCalculations::operator = (const SkyCalculations & other)
{
	return *this;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//			End Private Functions
///////////////////////////////////////////////////////////////////
#endif //___BIOSKY_SKYCALCULATIONS_HPP__2015___