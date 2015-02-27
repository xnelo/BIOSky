/**
* @file SkyCalculations.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implements the SkyCalculation class.
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

#include "SkyCalculations.hpp"

namespace BIO
{
	namespace SKY
	{
		SkyCalculations::SkyCalculations(DateTime * dateTime, GPS * gps) : _userDateTime(false), _dateTime(NULL), _userGPS(false), _gps(NULL)
		{
			if (dateTime == NULL)
			{
				_dateTime = new DateTime();
				_userDateTime = false;
			}
			else
			{
				_dateTime = dateTime;
				_userDateTime = true;
			}

			if (gps == NULL)
			{
				_gps = new GPS();
				_userDateTime = false;
			}
			else
			{
				_gps = gps;
				_userGPS = true;
			}
		}

		SkyCalculations::~SkyCalculations()
		{
			_delete();
		}

		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		//				Private Functions
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		void SkyCalculations::_delete()
		{
			_deleteDateTime();

			_deleteGPS();
		}

		void SkyCalculations::_deleteDateTime()
		{
			if ((_userDateTime == false) && (_dateTime != NULL))
				delete _dateTime;

			_dateTime = NULL;
			_userDateTime = false;
		}

		void SkyCalculations::_deleteGPS()
		{
			if ((_userGPS == false) && (_gps != NULL))
				delete _gps;

			_gps = NULL;
			_userGPS = false;
		}
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//			End Private Functions
		///////////////////////////////////////////////////////////////////
	}//end namespace SKY
}//end namespace BIO


