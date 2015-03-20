/**
* @file BIOSky.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implementation of functions defined in BIOSky.hpp.
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

#include "BIOSkyFunctions.hpp"
#include "MathUtils.hpp"

#include "lodepng.h"
//#include <iostream>
//#include <ctime>

#include "../source/MoonTexture.c"
#include "../source/NightSky_C.c"
//#include "ImageData.hpp"

#include <fstream>

#include <iostream>//needed for testing functions
#if BIOSKY_TESTING == 1
//Included all non-included headers for testing.
#include "DateTime.hpp"
#include "GPS.hpp"
#endif

namespace BIO
{
	namespace SKY
	{
		float CalculateMoonPhase(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year)
		{
			float UT = standardTime - UTCoffset;	// universal time

			float d = (float)DaysSinceJan02000(month, day, year);
			d = d + (UT / 24.0f);

			//Moon Phase from: http://www.aphayes.pwp.blueyonder.co.uk/library/moon.js

			//float j = d + 2451544.5;
			float T = (d - 1.5f) / 36525;
			float T2 = T*T;
			float T3 = T2*T;
			float T4 = T3*T;
			// Moons mean elongation Meeus second edition
			float D = 297.8501921f + 445267.1114034f*T - 0.0018819f*T2 + T3 / 545868.0f - T4 / 113065000.0f;
			// Moons mean anomaly M' Meeus second edition
			float MP = 134.9633964f + 477198.8675055f*T + 0.0087414f*T2 + T3 / 69699.0f - T4 / 14712000.0f;
			// Suns mean anomaly
			float M = 357.5291092f + 35999.0502909f*T - 0.0001536f*T2 + T3 / 24490000.0f;
			// phase angle
			//float pa = 180.0 - D - 6.289*sin(MP*MATH::DegreesToRadiansf) + 2.1*sin(M* MATH::DegreesToRadiansf) - 1.274*sin((2 * D - MP) * MATH::DegreesToRadiansf) - 0.658*sin((2 * D) * MATH::DegreesToRadiansf) - 0.214*sin((2 * MP) * MATH::DegreesToRadiansf) - 0.11*sin(D*MATH::DegreesToRadiansf);
			//modified
			float pa = 0.0f + D + 6.289f*sin(MP*MATH::DegreesToRadiansf) - 2.1f*sin(M* MATH::DegreesToRadiansf) + 1.274f*sin((2 * D - MP) * MATH::DegreesToRadiansf) + 0.658f*sin((2 * D) * MATH::DegreesToRadiansf) + 0.214f*sin((2 * MP) * MATH::DegreesToRadiansf) + 0.11f*sin(D*MATH::DegreesToRadiansf);
			return MATH::RevolutionReductionDegrees(pa);
		}

		SkyPosition CalculateMoonPosition(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude)
		{
			//Method from http://www.stjarnhimlen.se/comp/ppcomp.html
			// by: Paul Schlyter, Stockholm, Sweden

			//int monthint = Date::MonthToInt(month);
			float UT = standardTime - UTCoffset;	// universal time
			//
			float d = (float)DaysSinceJan02000(month, day, year);//(float)(367 * (int)year - (7 * ((int)year + ((monthint + 9) / 12))) / 4 + (275 * monthint) / 9 + (int)day - 730530);
			d = d + (UT / 24.0f);

			//Calculate the Moon Position ----------------------------------------------
			float N = MATH::RevolutionReductionDegrees(125.1228f - 0.0529538083f  * d);//in degrees (Long asc.node)
			float i = 5.1454f;//In degrees (Inclination)
			float w = MATH::RevolutionReductionDegrees(318.0634f + 0.1643573223f  * d);//in degrees (Arg.of perigee)
			float a = 60.2666f;//                                (Mean distance)
			float e = 0.054900f;//                               (Eccentricity)
			float M = MATH::RevolutionReductionDegrees(115.3654f + 13.0649929509f * d);//in degrees (Mean anomaly)

			//BIO_LOG_CRITICAL(std::endl <<
			//	"N: " << N << std::endl <<
			//	"i: " << i << std::endl <<
			//	"w: " << w << std::endl <<
			//	"a: " << a << std::endl <<
			//	"e: " << e << std::endl <<
			//	"M: " << M << std::endl
			//	);

			float E = M + (180 / MATH::PIf) * e * sin(M * MATH::DegreesToRadiansf) * (1 + e * cos(M * MATH::DegreesToRadiansf));//in degrees
			//BIO_LOG_CRITICAL("E0: " << E);

			float x = a * (cos(E * MATH::DegreesToRadiansf) - e);
			float y = a * sqrt(1 - e*e) * sin(E * MATH::DegreesToRadiansf);
			//BIO_LOG_CRITICAL("x: " << x << " y: " << y);

			float r = sqrt(x*x + y*y);// = 60.67134 Earth radii
			float v = MATH::RevolutionReductionDegrees(atan2(y, x) * MATH::RadiansToDegreesf);// in degrees = 259.8605_deg
			//BIO_LOG_CRITICAL("r: " << r << " v: " << v);

			float xeclip = r * (cos(N* MATH::DegreesToRadiansf) * cos((v + w)* MATH::DegreesToRadiansf) - sin(N* MATH::DegreesToRadiansf) * sin((v + w)* MATH::DegreesToRadiansf) * cos(i* MATH::DegreesToRadiansf));
			float yeclip = r * (sin(N* MATH::DegreesToRadiansf) * cos((v + w) * MATH::DegreesToRadiansf) + cos(N* MATH::DegreesToRadiansf) * sin((v + w)* MATH::DegreesToRadiansf) * cos(i* MATH::DegreesToRadiansf));
			float zeclip = r * sin((v + w)* MATH::DegreesToRadiansf) * sin(i* MATH::DegreesToRadiansf);
			//BIO_LOG_CRITICAL("xeclip: " << xeclip << " yeclip: " << yeclip << " zeclip: " << zeclip);

			//geocentric longetude and latitude
			float lonecl = atan2(yeclip, xeclip) * MATH::RadiansToDegreesf;//in degrees
			float latecl = atan2(zeclip, sqrt(xeclip*xeclip + yeclip*yeclip))* MATH::RadiansToDegreesf;//in degrees
			//BIO_LOG_CRITICAL("lonecl: " << lonecl << " latecl: " << latecl);

			float xh = r * cos(lonecl * MATH::DegreesToRadiansf) * cos(latecl * MATH::DegreesToRadiansf);
			float yh = r * sin(lonecl * MATH::DegreesToRadiansf) * cos(latecl * MATH::DegreesToRadiansf);
			float zh = r * sin(latecl * MATH::DegreesToRadiansf);

			float ecl = 23.4393f - 3.563E-7f * d; //in degrees

			float xequat = xh;
			float yequat = yh * cos(ecl * MATH::DegreesToRadiansf) - zh * sin(ecl * MATH::DegreesToRadiansf);
			float zequat = yh * sin(ecl * MATH::DegreesToRadiansf) + zh * cos(ecl * MATH::DegreesToRadiansf);

			float RA = MATH::RevolutionReductionDegrees(atan2(yequat, xequat) * MATH::RadiansToDegreesf); //in degrees
			float Dec = atan2(zequat, sqrt(xequat*xequat + yequat*yequat)) * MATH::RadiansToDegreesf; //in degrees
			
			//BIO_LOG_CRITICAL("RA: " << RA << " Dec: " << Dec);

			float ws = MATH::RevolutionReductionDegrees(282.9404f + 4.70935E-5f   * d); //in degrees (longitude of perihelion)
			float Ms = MATH::RevolutionReductionDegrees(356.0470f + 0.9856002585f * d);//in degrees (mean anomaly)

			float L = MATH::RevolutionReductionDegrees(ws + Ms);//mean longitude in degrees
			//BIO_LOG_CRITICAL("L: " << L);

			float GMST0 = (L / 15.0f) + 12.0f;//in hours
			//BIO_LOG_CRITICAL("GMST0: " << GMST0);
			//float GMST = GMST0 + UT;//in hours

			//BIO_LOG_CRITICAL("GMST0: " << GMST0);

			float SIDTIME = GMST0 + UT + (longitude * MATH::RadiansToDegreesf) / 15.0f;
			//BIO_LOG_CRITICAL("SIDTIME: " << SIDTIME);

			float HA = MATH::RevolutionReductionDegrees((SIDTIME * 15.0f) - RA);//in degrees
			//BIO_LOG_CRITICAL("HA: " << HA);

			x = cos(HA * MATH::DegreesToRadiansf) * cos(Dec * MATH::DegreesToRadiansf);
			y = sin(HA * MATH::DegreesToRadiansf) * cos(Dec * MATH::DegreesToRadiansf);
			float z = sin(Dec * MATH::DegreesToRadiansf);

			float xhor = x * sin(latitude) - z * cos(latitude);
			float yhor = y;
			float zhor = x * cos(latitude) + z * sin(latitude);

			float azimuth = atan2(yhor, xhor) + MATH::PIf;
			float altitude = asin(zhor);
			//--------------------------------------------------------------------------

			//(*v_out) = v;

			return SkyPosition(azimuth, MATH::PId2f - altitude);
			
		}

		float CalculateMoonVisibility(float SunAzimuth, float SunZenith, float MoonAzimuth, float MoonZenith)
		{
			const float SunDown = 1.6580f;
			const float SunUp = 1.4835f;
			const float SunDownValue = 1.0f;
			const float SunUpValue = 0.06f;

			if (SunZenith > SunDown)
				return SunDownValue;

			if (SunZenith < SunUp)
				return SunUpValue;

			return (SunZenith - SunUp) / (SunDown - SunUp);
		}

		SkyData CalculateSkyData(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude)
		{
			SkyData rtn;

			rtn.moonPos = CalculateMoonPosition(standardTime, UTCoffset, month, day, year, latitude, longitude);

			rtn.sunPos = CalculateSunPosition(standardTime, UTCoffset, month, day, year, latitude, longitude);

			rtn.northStarZenith = CalculateCelestialNorthPoleZenith(latitude);
			
			rtn.starRotation = CalculateStarRotation(standardTime, UTCoffset);

			rtn.phase = CalculateMoonPhase(standardTime, UTCoffset, month, day, year);

			rtn.moonVisibility = CalculateMoonVisibility(
				rtn.sunPos.Azimuth, rtn.sunPos.Zenith, 
				rtn.moonPos.Azimuth, rtn.moonPos.Zenith);

			return rtn;
		}

		SkyPosition CalculateSunPosition(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude)
		{
			//Method from http://www.stjarnhimlen.se/comp/ppcomp.html
			// by: Paul Schlyter, Stockholm, Sweden
			//int monthint = Date::MonthToInt(month);
			float UT = standardTime - UTCoffset;	// universal time
			//
			float d = (float)DaysSinceJan02000(month, day, year);//(float)(367 * (int)year - (7 * ((int)year + ((monthint + 9) / 12))) / 4 + (275 * monthint) / 9 + (int)day - 730530);
			d = d + (UT / 24.0f);

			float w = MATH::RevolutionReductionDegrees(282.9404f + 4.70935E-5f   * d); //in degrees (longitude of perihelion)
			float a = 1.000000f;//mean distance in a.u.
			float e = 0.016709f - 1.151E-9f * d; //(eccentricity)
			float M = MATH::RevolutionReductionDegrees(356.0470f + 0.9856002585f * d);//in degrees (mean anomaly)

			float oblecl = MATH::RevolutionReductionDegrees(23.4393f - 3.563E-7f * d);//in degrees
			float L = MATH::RevolutionReductionDegrees(w + M);//mean longitude in degrees

			float E = M + (180 / MATH::PIf) * e * sin(M * MATH::DegreesToRadiansf) * (1 + e * cos(M*MATH::DegreesToRadiansf));//in degrees
			E = MATH::RevolutionReductionDegrees(E);
			//BIO_LOG_CRITICAL("E: " << E);

			float x = cos(E * MATH::DegreesToRadiansf) - e;
			float y = sin(E * MATH::DegreesToRadiansf) * sqrt(1 - e*e);

			//BIO_LOG_CRITICAL("x: " << x << " y: " << y);

			float r = sqrt(x*x + y*y);
			float v = atan2(y, x) * MATH::RadiansToDegreesf;//in degrees

			//BIO_LOG_CRITICAL("r: " << r << " v: " << v);

			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			//
			//				Need lon for moon phase
			//
			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			float lon = MATH::RevolutionReductionDegrees(v + w);//longitude of sun in degrees

			//BIO_LOG_CRITICAL("long: " << lon);

			x = r * cos(lon * MATH::DegreesToRadiansf);//geocentric longitude
			y = r * sin(lon * MATH::DegreesToRadiansf);//geocentric latitude
			float z = 0.0;//sun is always zero

			//BIO_LOG_CRITICAL("x: " << x << " y: " << y << " z: " << z);

			float xequat = x;
			float yequat = y * cos(23.4406f * MATH::DegreesToRadiansf) - z * sin(23.4406f * MATH::DegreesToRadiansf);
			float zequat = y * sin(23.4406f * MATH::DegreesToRadiansf) + z * cos(23.4406f * MATH::DegreesToRadiansf);

			//BIO_LOG_CRITICAL("xequat: " << xequat << " yequat: " << yequat << " zequat: " << zequat);

			float RA = atan2(yequat, xequat) * MATH::RadiansToDegreesf; //In Degrees 
			float Dec = atan2(zequat, sqrt(xequat*xequat + yequat*yequat)) * MATH::RadiansToDegreesf;//in Degrees
			//BIO_LOG_CRITICAL("RA: " << RA << " Dec: " << Dec);

			float GMST0 = (L / 15.0f) + 12.0f;//in hours
			//float GMST = GMST0 + UT;//in hours

			//BIO_LOG_CRITICAL("GMST0: " << GMST0);

			float SIDTIME = GMST0 + UT + (longitude * MATH::RadiansToDegreesf) / 15.0f;

			//BIO_LOG_CRITICAL("SidTime: " << SIDTIME);

			float HA = (SIDTIME * 15.0f) - RA;//ha is in degrees

			//BIO_LOG_CRITICAL("HA: " << HA);

			x = cos(HA * MATH::DegreesToRadiansf) * cos(Dec * MATH::DegreesToRadiansf);
			y = sin(HA * MATH::DegreesToRadiansf) * cos(Dec * MATH::DegreesToRadiansf);
			z = sin(Dec * MATH::DegreesToRadiansf);

			//BIO_LOG_CRITICAL("x: " << x << " y: " << y << " z: " << z);

			float xhor = x * sin(latitude) - z * cos(latitude);
			float yhor = y;
			float zhor = x * cos(latitude) + z * sin(latitude);

			//BIO_LOG_CRITICAL("xhor: " << xhor << " yhor: " << yhor << " zhor: " << zhor);

			float azimuth = atan2(yhor, xhor) + MATH::PIf;
			float altitude = asin(zhor);//atan2(zhor, sqrt(xhor*xhor + yhor*yhor));

			//BIO_LOG_CRITICAL("azimuth: " << azimuth* MATH::BIO_180_PIf << " altitude: " << altitude* MATH::BIO_180_PIf);

			return SkyPosition(azimuth, MATH::PId2f - altitude);
		}

		float CalculateStarRotation(float standardTime, float UTCoffset)
		{
			//15 degrees in radians.
			const float _15Degrees = 0.2617993878f;//15.0f * MATH::DegreesToRadiansf;

			float UT = standardTime - UTCoffset;	// universal time

			//       Time            Degrees       Offset to start
			//                       per hour      stars at correct spot
			//    (Universal_Time * 15_Degrees) + 145_Degrees
			//return (UT * _15Degrees) + (145 * MATH::DegreesToRadiansf);
			return (UT * _15Degrees) + 2.530727415f;
		}

		RawGeometry * CreateSkyDomeGeometry(float radius, int numVerticalSegments, int numHorozontalSegments, bool fullDome)
		{
			RawGeometry * rtnValue = new RawGeometry();

			//is the numHorozontalSegments even
			if (numHorozontalSegments % 2 != 0)
				numHorozontalSegments += 1;

			if (fullDome)
			{
				//*
				//Create a sphere
				int totalVerts = ((numHorozontalSegments - 1) * numVerticalSegments) + 2;
				rtnValue->numVertecies = totalVerts;

				int totalTris = 2 * numVerticalSegments * (numHorozontalSegments - 1);
				int totalIndecies = totalTris * 3;
				rtnValue->numIndecies = totalIndecies;

				rtnValue->indecies = new unsigned short[totalIndecies];
				int indexOn = 0;

				rtnValue->vertecies = new Vector3D[totalVerts];
				rtnValue->UVTextureCoordinates = new Vector2D[totalVerts];
				int vertOn = 0;

				//set the top cap
				for (int i = 0; i < numVerticalSegments; i++)
				{
					rtnValue->indecies[indexOn++] = 0;
					rtnValue->indecies[indexOn++] = i + 1;
					rtnValue->indecies[indexOn++] = i + 2;
				}

				//fix the last index
				//doing it this way should be a little faster
				//than placing an if statment in the for loop
				rtnValue->indecies[indexOn - 1] = 1;
				//Done Setting the top cap -----------------

				//set the horozontal segments
				for (int i = 0; i < (numHorozontalSegments - 2); i++)
				{
					for (int j = 0; j < numVerticalSegments; j++)
					{
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1 + 1;

						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1 + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1 + 1;
					}

					rtnValue->indecies[indexOn - 2] = rtnValue->indecies[indexOn - 4] = (i * numVerticalSegments) + numVerticalSegments + 1;
					rtnValue->indecies[indexOn - 1] -= numVerticalSegments;
				}
				//Done Setting the horozontal segments

				//set the bottom cap --------------------------------
				for (int i = 1; i <= numVerticalSegments; i++)
				{
					rtnValue->indecies[indexOn++] = totalVerts - 1;
					rtnValue->indecies[indexOn++] = totalVerts - 1 - i;
					rtnValue->indecies[indexOn++] = totalVerts - 2 - i;
				}

				//fix the last index
				//doing it this way should be a little faster
				//than placing an if statment in the for loop
				rtnValue->indecies[indexOn - 1] = totalVerts - 2;
				//Done setting bottom cap ---------------------------

				//create the vertecies ----------------------------------
				//first vert is at 0,radius,0
				rtnValue->vertecies[0].X = 0; rtnValue->vertecies[0].Y = radius; rtnValue->vertecies[0].Z = 0;
				rtnValue->UVTextureCoordinates[0].X = rtnValue->UVTextureCoordinates[0].Y = 0.5f;
				vertOn++;

				double currHeight = radius;
				//double heightStep = radius / numHorozontalSegments;
				double currAngle = MATH::PId2;
				double angleStep = MATH::PI / (double)numHorozontalSegments;

				int middleIteration = (numHorozontalSegments / 2) - 1;

				for (int i = 0; i < numHorozontalSegments - 1; i++)
				{
					currAngle -= angleStep;

					//std::cout << "Current Angle: " << currAngle << " sin(currAngle): " << std::sin(currAngle) << std::endl;

					currHeight = std::sin(currAngle) * radius;

					if (i == middleIteration)
						currHeight = 0;

					double currRadius = std::sqrt((radius * radius) - (currHeight * currHeight));

					double circleStep = MATH::PIx2 / numVerticalSegments;
					double currAngle = 0;
					for (int j = 0; j < numVerticalSegments; j++)
					{
						rtnValue->vertecies[vertOn].X = (float)(currRadius * std::cos(currAngle));
						rtnValue->vertecies[vertOn].Y = (float)(currHeight);
						rtnValue->vertecies[vertOn].Z = (float)(currRadius * std::sin(currAngle));

						rtnValue->UVTextureCoordinates[vertOn].X = (rtnValue->vertecies[vertOn].X / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
						rtnValue->UVTextureCoordinates[vertOn].Y = (rtnValue->vertecies[vertOn].Z / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
						vertOn++;

						currAngle += circleStep;
					}
				}

				//final point
				rtnValue->vertecies[vertOn].X = 0; rtnValue->vertecies[vertOn].Y = -radius; rtnValue->vertecies[vertOn].Z = 0;
				rtnValue->UVTextureCoordinates[vertOn].X = rtnValue->UVTextureCoordinates[vertOn].Y = 0.5f;
				//Done Creating the vertecies ----------------------------
				//*/
			}
			else //hemisphere
			{

				//*
				//Create a Dome
				//float radius = 1.0f;
				//int numHorzSeg = 3; //Segments that lay horozontal on the sphere
				//int numVertSeg = 6; //Setments that lay Vertical on the sphere

				int totalVerts = (numHorozontalSegments * numVerticalSegments) + 1; //total number of vertecies on the sphere
				rtnValue->numVertecies = totalVerts;

				int totalTris = ((numHorozontalSegments - 1) * 2 * numVerticalSegments) + numVerticalSegments; //Total number of triangles that make up the sphere
				int totalIndecies = totalTris * 3; //Total number of Indecies to make the triangles
				rtnValue->numIndecies = totalIndecies;

				//irr::u16 * u2 = new irr::u16[totalIndecies];
				rtnValue->indecies = new unsigned short[totalIndecies];
				int indexOn = 0;

				//Vector3df * verts = new Vector3df[totalVerts];
				rtnValue->vertecies = new Vector3D[totalVerts];
				rtnValue->UVTextureCoordinates = new Vector2D[totalVerts];
				int vertOn = 0;

				//set the cap
				for (int i = 0; i < numVerticalSegments; i++)
				{
					rtnValue->indecies[indexOn++] = 0;
					rtnValue->indecies[indexOn++] = i + 1;
					rtnValue->indecies[indexOn++] = i + 2;
				}

				//fix the last index
				//doing it this way should be a little faster
				//than placing an if statment in the for loop
				rtnValue->indecies[indexOn - 1] = 1;

				//set the horozontal segments
				for (int i = 0; i < (numHorozontalSegments - 1); i++)
				{
					for (int j = 0; j < numVerticalSegments; j++)
					{
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1 + 1;

						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + numVerticalSegments + 1 + 1;
						rtnValue->indecies[indexOn++] = (i * numVerticalSegments) + j + 1 + 1;
					}

					rtnValue->indecies[indexOn - 2] = rtnValue->indecies[indexOn - 4] = (i * numVerticalSegments) + numVerticalSegments + 1;
					rtnValue->indecies[indexOn - 1] -= numVerticalSegments;
				}

				//create the vertecies
				//first vert is at 0,radius,0
				rtnValue->vertecies[0].X = 0; rtnValue->vertecies[0].Y = radius; rtnValue->vertecies[0].Z = 0;
				rtnValue->UVTextureCoordinates[0].X = rtnValue->UVTextureCoordinates[0].Y = 0.5f;
				vertOn++;

				double currHeight = radius;
				//double heightStep = radius / numHorozontalSegments;
				double currAngle = MATH::PId2;
				double angleStep = MATH::PId2 / (double)numHorozontalSegments;

				for (int i = 0; i < numHorozontalSegments; i++)
				{
					currAngle -= angleStep;

					//std::cout << "Current Angle: " << currAngle << " sin(currAngle): " << std::sin(currAngle) << std::endl;

					currHeight = std::sin(currAngle) * radius;

					if (i == numHorozontalSegments - 1)
						currHeight = 0;

					double currRadius = std::sqrt((radius * radius) - (currHeight * currHeight));

					double circleStep = MATH::PIx2 / numVerticalSegments;
					double currAngle = 0;
					for (int j = 0; j < numVerticalSegments; j++)
					{
						rtnValue->vertecies[vertOn].X = (float)(currRadius * std::cos(currAngle));
						rtnValue->vertecies[vertOn].Y = (float)(currHeight);
						rtnValue->vertecies[vertOn].Z = (float)(currRadius * std::sin(currAngle));

						rtnValue->UVTextureCoordinates[vertOn].X = (rtnValue->vertecies[vertOn].X / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
						rtnValue->UVTextureCoordinates[vertOn].Y = (rtnValue->vertecies[vertOn].Z / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
						vertOn++;

						currAngle += circleStep;
					}
				}
				//*/
			}
			return rtnValue;
		}

		RawGeometry * CreateNightSkyDomeGeometry(float radius)
		{
			RawGeometry * rtnVal = new RawGeometry();

			int numFaces = 264;

			rtnVal->numIndecies = numFaces * 3;
			rtnVal->indecies = new unsigned short[rtnVal->numIndecies];

			rtnVal->numVertecies = 146;
			rtnVal->UVTextureCoordinates = new Vector2D[rtnVal->numVertecies];
			rtnVal->vertecies = new Vector3D[rtnVal->numVertecies];

			struct vertex_struct {
				float x, y, z;
				float nx, ny, nz;
				float u, v;
			};
			struct vertex_struct vertices[] = {
				/* SPHERE: 146 vertices */
				{ -0.258819f, 0.965926f, 0.000001f, 0.278848f, -0.960326f, 0.000000f, 0.282584f, 0.241876f },
				{ -0.224144f, 0.965926f, 0.129410f, 0.241493f, -0.960326f, -0.139409f, 0.274412f, 0.227382f },
				{ -0.433013f, 0.866025f, 0.250001f, 0.446791f, -0.856624f, -0.257942f, 0.297548f, 0.203968f },
				{ -0.500000f, 0.866025f, 0.000001f, 0.515915f, -0.856624f, 0.000000f, 0.314328f, 0.233167f },
				{ -0.612373f, 0.707106f, 0.353554f, 0.621448f, -0.696432f, -0.358776f, 0.322139f, 0.178164f },
				{ -0.707107f, 0.707107f, 0.000001f, 0.717582f, -0.696432f, 0.000000f, 0.348527f, 0.223116f },
				{ -0.750000f, 0.499999f, 0.433013f, 0.754479f, -0.490860f, -0.435591f, 0.349687f, 0.148452f },
				{ -0.866026f, 0.500000f, 0.000000f, 0.871212f, -0.490860f, 0.000000f, 0.387243f, 0.211159f },
				{ -0.836516f, 0.258819f, 0.482963f, 0.837703f, -0.253578f, -0.483627f, 0.382286f, 0.112823f },
				{ -0.965926f, 0.258819f, 0.000000f, 0.967284f, -0.253578f, 0.000000f, 0.433294f, 0.196604f },
				{ -0.866025f, -0.000001f, 0.500000f, 0.866024f, 0.000000f, -0.499985f, 0.423319f, 0.068580f },
				{ -1.000000f, -0.000000f, -0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.490963f, 0.178809f },
				{ -1.000000f, -0.000000f, -0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.993060f, 0.685299f },
				{ -0.866025f, -0.000001f, 0.500000f, 0.866024f, 0.000000f, -0.499985f, 0.928330f, 0.573210f },
				{ -0.836516f, -0.258819f, 0.482962f, 0.837703f, 0.253578f, -0.483627f, 0.885168f, 0.615448f },
				{ -0.965926f, -0.258819f, -0.000000f, 0.967284f, 0.253578f, 0.000000f, 0.934549f, 0.700294f },
				{ -0.750000f, -0.500000f, 0.433012f, 0.754479f, 0.490860f, -0.435591f, 0.850393f, 0.649015f },
				{ -0.866025f, -0.500000f, -0.000000f, 0.871212f, 0.490860f, 0.000000f, 0.887645f, 0.711975f },
				{ -0.612372f, -0.707107f, 0.353553f, 0.621448f, 0.696432f, -0.358776f, 0.821401f, 0.677369f },
				{ -0.707107f, -0.707107f, -0.000001f, 0.717582f, 0.696432f, 0.000000f, 0.848358f, 0.722033f },
				{ -0.433013f, -0.866026f, 0.249999f, 0.446791f, 0.856624f, -0.257942f, 0.796240f, 0.702659f },
				{ -0.500000f, -0.866025f, -0.000001f, 0.515915f, 0.856624f, 0.000000f, 0.813922f, 0.731354f },
				{ -0.224144f, -0.965926f, 0.129408f, 0.241493f, 0.960326f, -0.139409f, 0.773430f, 0.726429f },
				{ -0.258819f, -0.965926f, -0.000001f, 0.278848f, 0.960326f, 0.000000f, 0.782283f, 0.740535f },
				{ -0.129410f, 0.965926f, 0.224145f, 0.139409f, -0.960326f, -0.241493f, 0.260088f, 0.218915f },
				{ -0.250000f, 0.866025f, 0.433013f, 0.257942f, -0.856624f, -0.446791f, 0.268417f, 0.187070f },
				{ -0.353554f, 0.707106f, 0.612373f, 0.358776f, -0.696432f, -0.621448f, 0.276811f, 0.152430f },
				{ -0.433013f, 0.499999f, 0.750000f, 0.435591f, -0.490860f, -0.754479f, 0.285813f, 0.112926f },
				{ -0.482963f, 0.258818f, 0.836516f, 0.483627f, -0.253578f, -0.837703f, 0.296229f, 0.065775f },
				{ -0.500000f, -0.000001f, 0.866025f, 0.499985f, 0.000000f, -0.866024f, 0.309643f, 0.006950f },
				{ -0.500000f, -0.000001f, 0.866025f, 0.499985f, 0.000000f, -0.866024f, 0.816247f, 0.508511f },
				{ -0.482963f, -0.258820f, 0.836516f, 0.483627f, 0.253578f, -0.837703f, 0.799987f, 0.566666f },
				{ -0.433013f, -0.500001f, 0.749999f, 0.435591f, 0.490860f, -0.754479f, 0.786655f, 0.613119f },
				{ -0.353554f, -0.707107f, 0.612372f, 0.358776f, 0.696432f, -0.621448f, 0.775725f, 0.652168f },
				{ -0.250000f, -0.866026f, 0.433012f, 0.257942f, 0.856624f, -0.446791f, 0.766580f, 0.686650f },
				{ -0.129410f, -0.965926f, 0.224143f, 0.139409f, 0.960326f, -0.241493f, 0.758711f, 0.718639f },
				{ -0.000000f, 0.965926f, 0.258820f, 0.000000f, -0.960326f, -0.278848f, 0.243449f, 0.218745f },
				{ -0.000000f, 0.866025f, 0.500001f, 0.000000f, -0.856624f, -0.515915f, 0.234740f, 0.187002f },
				{ -0.000000f, 0.707106f, 0.707107f, 0.000000f, -0.696432f, -0.717582f, 0.224690f, 0.152805f },
				{ -0.000000f, 0.499999f, 0.866026f, 0.000000f, -0.490860f, -0.871212f, 0.212735f, 0.114094f },
				{ -0.000000f, 0.258818f, 0.965926f, 0.000000f, -0.253578f, -0.967284f, 0.198183f, 0.068052f },
				{ -0.000000f, -0.000001f, 1.000000f, 0.000000f, 0.000000f, -1.000000f, 0.180393f, 0.010402f },
				{ -0.000000f, -0.000001f, 1.000000f, 0.000000f, 0.000000f, -1.000000f, 0.686842f, 0.508511f },
				{ -0.000000f, -0.258820f, 0.965925f, 0.000000f, 0.253578f, -0.967284f, 0.701832f, 0.567004f },
				{ -0.000000f, -0.500001f, 0.866025f, 0.000000f, 0.490860f, -0.871212f, 0.713510f, 0.613899f },
				{ -0.000000f, -0.707107f, 0.707106f, 0.000000f, 0.696432f, -0.717582f, 0.723568f, 0.653181f },
				{ -0.000000f, -0.866026f, 0.499999f, 0.000000f, 0.856624f, -0.515915f, 0.732888f, 0.687615f },
				{ -0.000000f, -0.965926f, 0.258818f, 0.000000f, 0.960326f, -0.278848f, 0.742069f, 0.719253f },
				{ 0.129409f, 0.965926f, 0.224145f, -0.139409f, -0.960326f, -0.241493f, 0.228954f, 0.226916f },
				{ 0.250000f, 0.866025f, 0.433013f, -0.257942f, -0.856624f, -0.446791f, 0.205540f, 0.203781f },
				{ 0.353553f, 0.707106f, 0.612373f, -0.358776f, -0.696432f, -0.621448f, 0.179738f, 0.179190f },
				{ 0.433012f, 0.499999f, 0.750000f, -0.435591f, -0.490860f, -0.754479f, 0.150029f, 0.151641f },
				{ 0.482962f, 0.258818f, 0.836516f, -0.483627f, -0.253578f, -0.837703f, 0.114406f, 0.119041f },
				{ 0.499999f, -0.000001f, 0.866025f, -0.499985f, 0.000000f, -0.866024f, 0.070177f, 0.078005f },
				{ 0.499999f, -0.000001f, 0.866025f, -0.499985f, 0.000000f, -0.866024f, 0.574767f, 0.573202f },
				{ 0.482962f, -0.258820f, 0.836516f, -0.483627f, 0.253578f, -0.837703f, 0.616993f, 0.616368f },
				{ 0.433012f, -0.500001f, 0.749999f, -0.435591f, 0.490860f, -0.754479f, 0.650553f, 0.651143f },
				{ 0.353553f, -0.707107f, 0.612372f, -0.358776f, 0.696432f, -0.621448f, 0.678904f, 0.680135f },
				{ 0.249999f, -0.866026f, 0.433012f, -0.257942f, 0.856624f, -0.446791f, 0.704193f, 0.705296f },
				{ 0.129409f, -0.965926f, 0.224143f, -0.139409f, 0.960326f, -0.241493f, 0.727963f, 0.728105f },
				{ 0.224143f, 0.965926f, 0.129410f, -0.241493f, -0.960326f, -0.139409f, 0.220487f, 0.241241f },
				{ 0.433012f, 0.866025f, 0.250001f, -0.446791f, -0.856624f, -0.257942f, 0.188641f, 0.232911f },
				{ 0.612372f, 0.707106f, 0.353554f, -0.621448f, -0.696432f, -0.358776f, 0.153999f, 0.224516f },
				{ 0.749999f, 0.499999f, 0.433013f, -0.754479f, -0.490860f, -0.435591f, 0.114494f, 0.215512f },
				{ 0.836516f, 0.258819f, 0.482963f, -0.837703f, -0.253578f, -0.483627f, 0.067340f, 0.205089f },
				{ 0.866025f, -0.000001f, 0.500000f, -0.866024f, 0.000000f, -0.499985f, 0.008513f, 0.191662f },
				{ 0.866025f, -0.000001f, 0.500000f, -0.866024f, 0.000000f, -0.499985f, 0.510037f, 0.685266f },
				{ 0.836516f, -0.258819f, 0.482962f, -0.837703f, 0.253578f, -0.483627f, 0.568195f, 0.701539f },
				{ 0.749999f, -0.500000f, 0.433012f, -0.754479f, 0.490860f, -0.435591f, 0.614650f, 0.714877f },
				{ 0.612372f, -0.707107f, 0.353553f, -0.621448f, 0.696432f, -0.358776f, 0.653700f, 0.725810f },
				{ 0.433012f, -0.866026f, 0.249999f, -0.446791f, 0.856624f, -0.257942f, 0.688182f, 0.734955f },
				{ 0.224143f, -0.965926f, 0.129408f, -0.241493f, 0.960326f, -0.139409f, 0.720173f, 0.742824f },
				{ 0.258819f, 0.965926f, 0.000001f, -0.278848f, -0.960326f, 0.000000f, 0.220316f, 0.257879f },
				{ 0.499999f, 0.866025f, 0.000001f, -0.515915f, -0.856624f, 0.000000f, 0.188572f, 0.266589f },
				{ 0.707106f, 0.707107f, 0.000001f, -0.717582f, -0.696432f, 0.000000f, 0.154373f, 0.276640f },
				{ 0.866025f, 0.500000f, 0.000000f, -0.871212f, -0.490860f, 0.000000f, 0.115657f, 0.288596f },
				{ 0.965925f, 0.258819f, 0.000000f, -0.967284f, -0.253578f, 0.000000f, 0.069605f, 0.303151f },
				{ 0.999999f, -0.000000f, -0.000000f, -1.000000f, 0.000000f, 0.000000f, 0.011937f, 0.320946f },
				{ 0.999999f, -0.000000f, -0.000000f, -1.000000f, 0.000000f, 0.000000f, 0.510009f, 0.814704f },
				{ 0.965925f, -0.258819f, -0.000000f, -0.967284f, 0.253578f, 0.000000f, 0.568520f, 0.799708f },
				{ 0.866025f, -0.500000f, -0.000000f, -0.871212f, 0.490860f, 0.000000f, 0.615424f, 0.788027f },
				{ 0.707106f, -0.707107f, -0.000001f, -0.717582f, 0.696432f, 0.000000f, 0.654711f, 0.777968f },
				{ 0.499999f, -0.866025f, -0.000001f, -0.515915f, 0.856624f, 0.000000f, 0.689147f, 0.768647f },
				{ 0.258818f, -0.965926f, -0.000001f, -0.278848f, 0.960326f, 0.000000f, 0.720786f, 0.759466f },
				{ 0.224143f, 0.965926f, -0.129409f, -0.241493f, -0.960326f, 0.139409f, 0.228488f, 0.272374f },
				{ 0.433012f, 0.866026f, -0.249999f, -0.446791f, -0.856624f, 0.257942f, 0.205352f, 0.295788f },
				{ 0.612372f, 0.707107f, -0.353553f, -0.621448f, -0.696432f, 0.358776f, 0.180760f, 0.321591f },
				{ 0.749999f, 0.500000f, -0.433012f, -0.754479f, -0.490860f, 0.435591f, 0.153212f, 0.351304f },
				{ 0.836516f, 0.258819f, -0.482963f, -0.837703f, -0.253578f, 0.483627f, 0.120613f, 0.386932f },
				{ 0.866025f, 0.000000f, -0.500000f, -0.866024f, 0.000000f, 0.499985f, 0.079580f, 0.431175f },
				{ 0.866025f, 0.000000f, -0.500000f, -0.866024f, 0.000000f, 0.499985f, 0.574740f, 0.926792f },
				{ 0.836516f, -0.258819f, -0.482963f, -0.837703f, 0.253578f, 0.483627f, 0.617902f, 0.884553f },
				{ 0.749999f, -0.500000f, -0.433013f, -0.754479f, 0.490860f, 0.435591f, 0.652676f, 0.850986f },
				{ 0.612372f, -0.707106f, -0.353554f, -0.621448f, 0.696432f, 0.358776f, 0.681668f, 0.822632f },
				{ 0.433012f, -0.866025f, -0.250001f, -0.446791f, 0.856624f, 0.257942f, 0.706829f, 0.797342f },
				{ 0.224143f, -0.965926f, -0.129410f, -0.241493f, 0.960326f, 0.139409f, 0.729638f, 0.773572f },
				{ 0.129409f, 0.965926f, -0.224143f, -0.139409f, -0.960326f, 0.241493f, 0.242812f, 0.280841f },
				{ 0.249999f, 0.866026f, -0.433012f, -0.257942f, -0.856624f, 0.446791f, 0.234483f, 0.312685f },
				{ 0.353553f, 0.707107f, -0.612372f, -0.358776f, -0.696432f, 0.621448f, 0.226088f, 0.347326f },
				{ 0.433012f, 0.500001f, -0.749999f, -0.435591f, -0.490860f, 0.754479f, 0.217087f, 0.386830f },
				{ 0.482962f, 0.258820f, -0.836516f, -0.483627f, -0.253578f, 0.837703f, 0.206670f, 0.433981f },
				{ 0.499999f, 0.000001f, -0.866025f, -0.499985f, 0.000000f, 0.866024f, 0.193256f, 0.492806f },
				{ 0.499999f, 0.000001f, -0.866025f, -0.499985f, 0.000000f, 0.866024f, 0.686823f, 0.991489f },
				{ 0.482962f, -0.258818f, -0.836516f, -0.483627f, 0.253578f, 0.837703f, 0.703083f, 0.933335f },
				{ 0.433012f, -0.499999f, -0.750000f, -0.435591f, 0.490860f, 0.754479f, 0.716414f, 0.886881f },
				{ 0.353553f, -0.707106f, -0.612373f, -0.358776f, 0.696432f, 0.621448f, 0.727345f, 0.847833f },
				{ 0.249999f, -0.866025f, -0.433013f, -0.257942f, 0.856624f, 0.446791f, 0.736489f, 0.813351f },
				{ 0.129409f, -0.965926f, -0.224144f, -0.139409f, 0.960326f, 0.241493f, 0.744358f, 0.781361f },
				{ -0.000000f, 0.965926f, -0.258818f, 0.000000f, -0.960326f, 0.278848f, 0.259451f, 0.281011f },
				{ -0.000000f, 0.866026f, -0.499999f, 0.000000f, -0.856624f, 0.515915f, 0.268160f, 0.312754f },
				{ -0.000000f, 0.707107f, -0.707106f, 0.000000f, -0.696432f, 0.717582f, 0.278210f, 0.346951f },
				{ -0.000000f, 0.500001f, -0.866025f, 0.000000f, -0.490860f, 0.871212f, 0.290165f, 0.385662f },
				{ -0.000000f, 0.258820f, -0.965925f, 0.000000f, -0.253578f, 0.967284f, 0.304717f, 0.431704f },
				{ -0.000001f, 0.000001f, -0.999999f, 0.000000f, 0.000000f, 0.999969f, 0.322508f, 0.489354f },
				{ -0.000001f, 0.000001f, -0.999999f, 0.000000f, 0.000000f, 0.999969f, 0.816227f, 0.991488f },
				{ -0.000000f, -0.258818f, -0.965926f, 0.000000f, 0.253578f, 0.967284f, 0.801237f, 0.932996f },
				{ -0.000000f, -0.499999f, -0.866025f, 0.000000f, 0.490860f, 0.871212f, 0.789559f, 0.886101f },
				{ -0.000001f, -0.707106f, -0.707107f, 0.000000f, 0.696432f, 0.717582f, 0.779501f, 0.846819f },
				{ -0.000000f, -0.866025f, -0.500000f, 0.000000f, 0.856624f, 0.515915f, 0.770181f, 0.812386f },
				{ -0.000000f, -0.965926f, -0.258819f, 0.000000f, 0.960326f, 0.278848f, 0.761000f, 0.780748f },
				{ -0.129410f, 0.965926f, -0.224143f, 0.139409f, -0.960326f, 0.241493f, 0.273946f, 0.272839f },
				{ -0.250000f, 0.866026f, -0.433012f, 0.257942f, -0.856624f, 0.446791f, 0.297360f, 0.295975f },
				{ -0.353554f, 0.707107f, -0.612372f, 0.358776f, -0.696432f, 0.621448f, 0.323162f, 0.320566f },
				{ -0.433013f, 0.500001f, -0.749999f, 0.435591f, -0.490860f, 0.754479f, 0.352871f, 0.348115f },
				{ -0.482963f, 0.258820f, -0.836516f, 0.483627f, -0.253578f, 0.837703f, 0.388494f, 0.380715f },
				{ -0.500000f, 0.000001f, -0.866025f, 0.499985f, 0.000000f, 0.866024f, 0.432724f, 0.421751f },
				{ -0.500000f, 0.000001f, -0.866025f, 0.499985f, 0.000000f, 0.866024f, 0.928302f, 0.926799f },
				{ -0.482963f, -0.258818f, -0.836516f, 0.483627f, 0.253578f, 0.837703f, 0.886076f, 0.883633f },
				{ -0.433013f, -0.499999f, -0.750000f, 0.435591f, 0.490860f, 0.754479f, 0.852515f, 0.848857f },
				{ -0.353554f, -0.707106f, -0.612373f, 0.358776f, 0.696432f, 0.621448f, 0.824164f, 0.819865f },
				{ -0.250000f, -0.866025f, -0.433013f, 0.257942f, 0.856624f, 0.446791f, 0.798876f, 0.794705f },
				{ -0.129410f, -0.965926f, -0.224144f, 0.139409f, 0.960326f, 0.241493f, 0.775106f, 0.771896f },
				{ -0.224144f, 0.965926f, -0.129409f, 0.241493f, -0.960326f, 0.139409f, 0.282413f, 0.258515f },
				{ -0.433013f, 0.866026f, -0.249999f, 0.446791f, -0.856624f, 0.257942f, 0.314259f, 0.266844f },
				{ -0.612373f, 0.707107f, -0.353553f, 0.621448f, -0.696432f, 0.358776f, 0.348901f, 0.275240f },
				{ -0.750000f, 0.500000f, -0.433012f, 0.754479f, -0.490860f, 0.435591f, 0.388406f, 0.284243f },
				{ -0.836516f, 0.258819f, -0.482962f, 0.837703f, -0.253578f, 0.483627f, 0.435560f, 0.294666f },
				{ -0.866025f, 0.000000f, -0.500000f, 0.866024f, 0.000000f, 0.499985f, 0.494388f, 0.308093f },
				{ -0.866025f, 0.000000f, -0.500000f, 0.866024f, 0.000000f, 0.499985f, 0.993032f, 0.814735f },
				{ -0.836516f, -0.258819f, -0.482963f, 0.837703f, 0.253578f, 0.483627f, 0.934874f, 0.798462f },
				{ -0.750000f, -0.500000f, -0.433013f, 0.754479f, 0.490860f, 0.435591f, 0.888419f, 0.785124f },
				{ -0.612372f, -0.707106f, -0.353554f, 0.621448f, 0.696432f, 0.358776f, 0.849368f, 0.774191f },
				{ -0.433013f, -0.866025f, -0.250000f, 0.446791f, 0.856624f, 0.257942f, 0.814886f, 0.765045f },
				{ -0.224144f, -0.965926f, -0.129410f, 0.241493f, 0.960326f, 0.139409f, 0.782896f, 0.757177f },
				{ -0.000000f, 1.000000f, 0.000001f, 0.000000f, -1.000000f, 0.000000f, 0.251450f, 0.249878f },
				{ -0.000001f, -1.000000f, -0.000001f, 0.000000f, 1.000000f, 0.000000f, 0.751534f, 0.750000f }
			};

			unsigned short indexes[] = {
				/* SPHERE 264 faces */
				0, 1, 2, 0, 2, 3, 3, 2, 4, 3, 4, 5, 5, 4, 6, 5, 6, 7, 7, 6, 8, 7, 8, 9, 9, 8, 10, 9, 10, 11, 12, 13, 14, 12, 14, 15, 15, 14, 16, 15, 16, 17, 17, 16, 18, 17, 18, 19, 19, 18, 20, 19, 20, 21, 22, 23, 21, 22, 21, 20, 1, 24, 25, 1, 25, 2, 2, 25, 26, 2, 26, 4, 4, 26, 27, 4, 27, 6, 6, 27, 28, 6, 28, 8, 8, 28, 29, 8, 29, 10, 13, 30, 31, 13, 31, 14, 14, 31, 32, 14, 32, 16, 16, 32, 33, 16, 33, 18, 18, 33, 34, 18, 34, 20, 20, 34, 35, 20, 35, 22, 24, 36, 37, 24, 37, 25, 25, 37, 38, 25, 38, 26, 26, 38, 39, 26, 39, 27, 27, 39, 40, 27, 40, 28, 28, 40, 41, 28, 41, 29, 30, 42, 43, 30, 43, 31, 31, 43, 44, 31, 44, 32, 32, 44, 45, 32, 45, 33, 33, 45, 46, 33, 46, 34, 34, 46, 47, 34, 47, 35, 36, 48, 49, 36, 49, 37, 37, 49, 50, 37, 50, 38, 38, 50, 51, 38, 51, 39, 39, 51, 52, 39, 52, 40, 40, 52, 53, 40, 53, 41, 42, 54, 55, 42, 55, 43, 43, 55, 56, 43, 56, 44, 44, 56, 57, 44, 57, 45, 45, 57, 58, 45, 58, 46, 46, 58, 59, 46, 59, 47, 48, 60, 61, 48, 61, 49, 49, 61, 62, 49, 62, 50, 50, 62, 63, 50, 63, 51, 51, 63, 64, 51, 64, 52, 52, 64, 65, 52, 65, 53, 54, 66, 67, 54, 67, 55, 55, 67, 68, 55, 68, 56, 56, 68, 69, 56, 69, 57, 57, 69, 70, 57, 70, 58, 58, 70, 71, 58, 71, 59, 60, 72, 73, 60, 73, 61, 61, 73, 74, 61, 74, 62, 62, 74, 75, 62, 75, 63, 63, 75, 76, 63, 76, 64, 64, 76, 77, 64, 77, 65, 66, 78, 79, 66, 79, 67, 67, 79, 80, 67, 80, 68, 68, 80, 81, 68, 81, 69, 69, 81, 82, 69, 82, 70, 70, 82, 83, 70, 83, 71, 72, 84, 85, 72, 85, 73, 73, 85, 86, 73, 86, 74, 74, 86, 87, 74, 87, 75, 75, 87, 88, 75, 88, 76, 76, 88, 89, 76, 89, 77, 78, 90, 91, 78, 91, 79, 79, 91, 92, 79, 92, 80, 80, 92, 93, 80, 93, 81, 81, 93, 94, 81, 94, 82, 82, 94, 95, 82, 95, 83, 84, 96, 97, 84, 97, 85, 85, 97, 98, 85, 98, 86, 86, 98, 99, 86, 99, 87, 87, 99, 100, 87, 100, 88, 88, 100, 101, 88, 101, 89, 90, 102, 103, 90, 103, 91, 91, 103, 104, 91, 104, 92, 92, 104, 105, 92, 105, 93, 93, 105, 106, 93, 106, 94, 94, 106, 107, 94, 107, 95, 96, 108, 109, 96, 109, 97, 97, 109, 110, 97, 110, 98, 98, 110, 111, 98, 111, 99, 99, 111, 112, 99, 112, 100, 100, 112, 113, 100, 113, 101, 102, 114, 115, 102, 115, 103, 103, 115, 116, 103, 116, 104, 104, 116, 117, 104, 117, 105, 105, 117, 118, 105, 118, 106, 106, 118, 119, 106, 119, 107, 108, 120, 121, 108, 121, 109, 109, 121, 122, 109, 122, 110, 110, 122, 123, 110, 123, 111, 111, 123, 124, 111, 124, 112, 112, 124, 125, 112, 125, 113, 114, 126, 127, 114, 127, 115, 115, 127, 128, 115, 128, 116, 116, 128, 129, 116, 129, 117, 117, 129, 130, 117, 130, 118, 118, 130, 131, 118, 131, 119, 120, 132, 133, 120, 133, 121, 121, 133, 134, 121, 134, 122, 122, 134, 135, 122, 135, 123, 123, 135, 136, 123, 136, 124, 124, 136, 137, 124, 137, 125, 126, 138, 139, 126, 139, 127, 127, 139, 140, 127, 140, 128, 128, 140, 141, 128, 141, 129, 129, 141, 142, 129, 142, 130, 130, 142, 143, 130, 143, 131, 144, 1, 0, 23, 22, 145, 144, 24, 1, 22, 35, 145, 144, 36, 24, 35, 47, 145, 144, 48, 36, 47, 59, 145, 144, 60, 48, 59, 71, 145, 144, 72, 60, 71, 83, 145, 144, 84, 72, 83, 95, 145, 144, 96, 84, 95, 107, 145, 144, 108, 96, 107, 119, 145, 144, 120, 108, 119, 131, 145, 144, 132, 120, 131, 143, 145, 144, 0, 132, 132, 0, 3, 132, 3, 133, 133, 3, 5, 133, 5, 134, 134, 5, 7, 134, 7, 135, 135, 7, 9, 135, 9, 136, 136, 9, 11, 136, 11, 137, 138, 12, 15, 138, 15, 139, 139, 15, 17, 139, 17, 140, 140, 17, 19, 140, 19, 141, 141, 19, 21, 141, 21, 142, 23, 143, 142, 23, 142, 21, 143, 23, 145
			};

			//copy the vertecies
			for (int i = 0; i < rtnVal->numVertecies; i++)
			{
				vertex_struct tmp = vertices[i];
				rtnVal->vertecies[i].X = tmp.x * radius;
				rtnVal->vertecies[i].Y = tmp.y * radius;
				rtnVal->vertecies[i].Z = tmp.z * radius;

				rtnVal->UVTextureCoordinates[i].X = tmp.u;
				rtnVal->UVTextureCoordinates[i].Y = tmp.v;
			}

			//copy the indecies
			for (int i = 0; i < rtnVal->numIndecies; i++)
			{
				rtnVal->indecies[i] = indexes[i];
			}

			return rtnVal;
		}

		unsigned char * CreateMoonTexture(int * width, int * height)
		{
			(*width) = moonImageData.width;
			(*height) = moonImageData.height;

			//format B G R A all 8 bits;
			unsigned char * rtn = new unsigned char[moonImageData.width * moonImageData.height * 4];

			memcpy(rtn, moonImageData.pixel_data, moonImageData.width * moonImageData.height * 4);

			/*
			//convert from RGBA format to BGRA format
			for (unsigned int y = 0; y < moonImageData.height; y++)
			{
				for (unsigned int x = 0; x < moonImageData.width; x++)
				{
					int index = ((y * moonImageData.width) + x) * 4;

					rtn[index] = moonImageData.pixel_data[index + 2];
					rtn[index + 1] = moonImageData.pixel_data[index + 1];
					rtn[index + 2] = moonImageData.pixel_data[index];
					rtn[index + 3] = moonImageData.pixel_data[index + 3];
				}
			}
			//*/

			return rtn;
		}

		unsigned char * CreateNightSkyTexture(int * width, int * height)
		{
			std::string filename = "NightSky.png";

			//std::vector<unsigned char> image;
			unsigned char * image = NULL;
			//unsigned char * buffer = NULL;
			//std::size_t bufferSize = loadFile(&buffer, filename);
			unsigned int w, h;

			//if ((buffer == NULL) || (bufferSize == 0))
			//	//std::cout << "Error loading file" << std::endl;
			//	return NULL;

			//lodepng::State tmp;
			
			//clock_t t = clock();
			//int error = lodepng::decode(image, w, h, buffer, bufferSize);// tmp, buffer);
			unsigned int error = lodepng_decode32(&image, &w, &h, xd_data, sizeof(xd_data));//buffer, bufferSize);
			//t = clock() - t;

			//std::cout << "Deconding png took: " << t << " clicks and " << ((float)t) / CLOCKS_PER_SEC << " seconds" << std::endl;
			

			if (error != 0)
				//std::cout << "Error Decoding PNG" << std::endl;
				return NULL;

			(*width) = w;
			(*height) = h;

			//format B G R A all 8 bits;
			unsigned char * rtn = new unsigned char[w * h * 4];

			//t = clock();
			for (unsigned int i = 0; i < w * h * 4; i = i + 4)
			{
				//RGBA
				rtn[i] = image[i + 2];
				rtn[i + 1] = image[i + 1];
				rtn[i + 2] = image[i];
				rtn[i + 3] = image[i + 3];
			}
			//t = clock() - t;
			//std::cout << "Converting to correct format took: " << t << " clicks and " << ((float)t) / CLOCKS_PER_SEC << " seconds"<<std::endl;

			delete[] image;
			//delete[] buffer;

			return rtn;
		}

		unsigned char * CreateSunTexture(int sideLength)
		{
			//get the center point of the texture
			float c = sideLength / 2.0f; //cx = cy = c

			//calculate the radius of the sun
			float radius = c * 0.45f;

			//calculate the fade radius of the sun
			float radiusF = c * 0.55f;

			//calculate the glow radius of the sun
			float radiusG = c * 0.98f;

			const float glowStartPercent = 0.125f;

			//format B G R A all 8 bits;
			unsigned char * rtn = new unsigned char[sideLength * sideLength * 4];

			for (int i = 0; i < sideLength; i++)
			{
				for (int j = 0; j < sideLength; j++)
				{
					int index = ((i * sideLength) + j) * 4;

					//B value
					rtn[index] = 255;

					//G value
					rtn[index + 1] = 255;

					//R Value
					rtn[index + 2] = 255;

					float dist = MATH::Distance((float)i, (float)j, c, c);

					if (dist <= radius)
					{
						//A Value
						rtn[index + 3] = 255; //completely opaque
					}
					else if (dist <= radiusF)
					{
						//fade the sun
						//A Value
						rtn[index + 3] = (unsigned char)((((1 - ((dist - radius) / (radiusF - radius))) * (1.0f - glowStartPercent)) + glowStartPercent) * 255.0f);
					}
					else if (dist <= radiusG)
					{
						//Add the glow
						//A Value
						rtn[index + 3] = (unsigned char)(((1 - ((dist - radius) / (radiusG - radius))) * glowStartPercent) * 255.0f);
					}
					else
					{
						//A Value
						rtn[index + 3] = 0; //completely transparent
					}
				}
			}

			return rtn;
		}


		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		//					Private Functions
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		

		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//			End private functions
		///////////////////////////////////////////////////////////////////////

#if BIOSKY_TESTING == 1
		bool LibraryTests(XNELO::TESTING::Test * test)
		{
			test->SetName("BIOSky Functions Test");

			//test DaysSince Jan 2000
			test->UnitTest(DaysSinceJan02000(JANUARY, 0, 2000) == 0, "Days Since Dec 31, 1999[1]");
			test->UnitTest(DaysSinceJan02000(DECEMBER, 31, 1999) == 0, "Days Since Dec 31, 1999[2]");
			test->UnitTest(DaysSinceJan02000(JANUARY, 1, 2000) == 1, "Days Since Dec 31, 1999[3]");
			test->UnitTest(DaysSinceJan02000(DECEMBER, 31, 2000) == 366, "Days Since Dec 31, 1999[4]");
			test->UnitTest(DaysSinceJan02000(DECEMBER, 31, 2001) == 366+365, "Days Since Dec 31, 1999[4]");
			test->UnitTest(DaysSinceJan02000(JANUARY, 31, 2002) == 366 + 365 + 31, "Days Since Dec 31, 1999[4]");

			test->UnitTest(CalculateJulianDay(JANUARY, 1, 2000) == 2451544.5f, "Julian Day[1]");
			test->UnitTest(CalculateJulianDay(JANUARY, 1, 1999) == 2451179.5f, "Julian Day[2]");
			test->UnitTest(CalculateJulianDay(JUNE, 19, 1987) == 2446965.5f, "Julian Day[3]");
			test->UnitTest(CalculateJulianDay(JANUARY, 27, 1988) == 2447187.5f, "Julian Day[4]");
			test->UnitTest(CalculateJulianDay(JANUARY, 1, 1900) == 2415020.5f, "Julian Day[5]");
			test->UnitTest(CalculateJulianDay(JANUARY, 1, 1600) == 2305447.5f, "Julian Day[6]");
			test->UnitTest(CalculateJulianDay(DECEMBER, 31, -123) == 1676496.5f, "Julian Day[7]");
			test->UnitTest(CalculateJulianDay(JANUARY, 1, -122) == 1676497.5f, "Julian Day[8]");
			test->UnitTest(CalculateJulianDay(JANUARY, 2, -4712) == 0.5f, "Julian Day[9]");
			//std::cout << "Julian Date: " << CalculateJulianDay(APRIL, 10, 837) << std::endl;

			SkyPosition pos;

			float positionTolerance = 0.0001f;

			pos = CalculateSunPosition(0.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 6.1641f, positionTolerance, "Azimuth 0000");
			test->UnitTest(pos.Zenith, 2.02607f, positionTolerance, "Zenith 0000");

			pos = CalculateSunPosition(1.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 0.149771f, positionTolerance, "Azimuth 0100");
			test->UnitTest(pos.Zenith, 2.02295f, positionTolerance, "Zenith 0100");

			pos = CalculateSunPosition(2.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 0.408165f, positionTolerance, "Azimuth 0200");
			test->UnitTest(pos.Zenith, 1.96833f, positionTolerance, "Zenith 0200");

			pos = CalculateSunPosition(3.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 0.642539f, positionTolerance, "Azimuth 0300");
			test->UnitTest(pos.Zenith, 1.86901f, positionTolerance, "Zenith 0300");

			pos = CalculateSunPosition(4.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 0.84901f, positionTolerance, "Azimuth 0400");
			test->UnitTest(pos.Zenith, 1.73481f, positionTolerance, "Zenith 0400");

			pos = CalculateSunPosition(5.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.03111f, positionTolerance, "Azimuth 0500");
			test->UnitTest(pos.Zenith, 1.57522f, positionTolerance, "Zenith 0500");

			pos = CalculateSunPosition(6.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.19597f, positionTolerance, "Azimuth 0600");
			test->UnitTest(pos.Zenith, 1.39801f, positionTolerance, "Zenith 0600");

			pos = CalculateSunPosition(7.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.35237f, positionTolerance, "Azimuth 0700");
			test->UnitTest(pos.Zenith, 1.20921f, positionTolerance, "Zenith 0700");

			pos = CalculateSunPosition(8.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.51102f, positionTolerance, "Azimuth 0800");
			test->UnitTest(pos.Zenith, 1.01374f, positionTolerance, "Zenith 0800");

			pos = CalculateSunPosition(9.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.68757f, positionTolerance, "Azimuth 0900");
			test->UnitTest(pos.Zenith, 0.816463f, positionTolerance, "Zenith 0900");

			pos = CalculateSunPosition(10.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 1.91086f, positionTolerance, "Azimuth 1000");
			test->UnitTest(pos.Zenith, 0.624135f, positionTolerance, "Zenith 1000");

			pos = CalculateSunPosition(11.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 2.24444f, positionTolerance, "Azimuth 1100");
			test->UnitTest(pos.Zenith, 0.450884f, positionTolerance, "Zenith 1100");

			pos = CalculateSunPosition(12.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 2.8096f, positionTolerance, "Azimuth 1200");
			test->UnitTest(pos.Zenith, 0.334566f, positionTolerance, "Zenith 1200");

			pos = CalculateSunPosition(13.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 3.55137f, positionTolerance, "Azimuth 1300");
			test->UnitTest(pos.Zenith, 0.342509f, positionTolerance, "Zenith 1300");

			pos = CalculateSunPosition(14.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 4.08532f, positionTolerance, "Azimuth 1400");
			test->UnitTest(pos.Zenith, 0.468305f, positionTolerance, "Zenith 1400");

			pos = CalculateSunPosition(15.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 4.40162f, positionTolerance, "Azimuth 1500");
			test->UnitTest(pos.Zenith, 0.644841f, positionTolerance, "Zenith 1500");

			pos = CalculateSunPosition(16.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 4.61754f, positionTolerance, "Azimuth 1600");
			test->UnitTest(pos.Zenith, 0.838138f, positionTolerance, "Zenith 1600");

			pos = CalculateSunPosition(17.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 4.79109f, positionTolerance, "Azimuth 1700");
			test->UnitTest(pos.Zenith, 1.0354f, positionTolerance, "Zenith 1700");

			pos = CalculateSunPosition(18.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 4.9489f, positionTolerance, "Azimuth 1800");
			test->UnitTest(pos.Zenith, 1.23025f, positionTolerance, "Zenith 1800");

			pos = CalculateSunPosition(19.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.1058f, positionTolerance, "Azimuth 1900");
			test->UnitTest(pos.Zenith, 1.41787f, positionTolerance, "Zenith 1900");

			pos = CalculateSunPosition(20.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.27222f, positionTolerance, "Azimuth 2000");
			test->UnitTest(pos.Zenith, 1.59327f, positionTolerance, "Zenith 2000");

			pos = CalculateSunPosition(21.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.45673f, positionTolerance, "Azimuth 2100");
			test->UnitTest(pos.Zenith, 1.75023f, positionTolerance, "Zenith 2100");

			pos = CalculateSunPosition(22.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.66617f, positionTolerance, "Azimuth 2200");
			test->UnitTest(pos.Zenith, 1.88079f, positionTolerance, "Zenith 2200");

			pos = CalculateSunPosition(23.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.90341f, positionTolerance, "Azimuth 2300");
			test->UnitTest(pos.Zenith, 1.97533f, positionTolerance, "Zenith 2300");

			pos = CalculateMoonPosition(17.0f, -4, MARCH, 14, 2015, -10.141931686131018f * MATH::DegreesToRadiansf, -61.875f * MATH::DegreesToRadiansf);
			test->UnitTest(pos.Azimuth, 4.03603f, positionTolerance, "Moon Azimuth[A]");
			test->UnitTest(pos.Zenith, 2.36212f, positionTolerance, "Moon Zenith [A]");

			pos = CalculateSunPosition(23.0f, -7, JUNE, 6, 2003, 0.7155849933f, -1.954768762f);
			test->UnitTest(pos.Azimuth, 5.90343f, positionTolerance, "Moon Azimuth[B]");
			test->UnitTest(pos.Zenith, 1.9753f, positionTolerance, "Moon Zenith [B]");

			pos = CalculateMoonPosition(0, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 3.71909f, positionTolerance, "Moon Azimuth[1]");
			test->UnitTest(pos.Zenith, 1.33495f, positionTolerance, "Moon Zenith [1]");

			pos = CalculateMoonPosition(4, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 4.38435f, positionTolerance, "Moon Azimuth[2]");
			test->UnitTest(pos.Zenith, 1.92706f, positionTolerance, "Moon Zenith [2]");

			pos = CalculateMoonPosition(8, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 5.10441f, positionTolerance, "Moon Azimuth[3]");
			test->UnitTest(pos.Zenith, 2.66568f, positionTolerance, "Moon Zenith [3]");

			pos = CalculateMoonPosition(12, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 1.18376f, positionTolerance, "Moon Azimuth[4]");
			test->UnitTest(pos.Zenith, 2.65039f, positionTolerance, "Moon Zenith [4]");
			
			pos = CalculateMoonPosition(16, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 1.88796f, positionTolerance, "Moon Azimuth[5]");
			test->UnitTest(pos.Zenith, 1.90209f, positionTolerance, "Moon Zenith [5]");

			pos = CalculateMoonPosition(20, 0, AUGUST, 17, 2005, 0.715f, 0);
			test->UnitTest(pos.Azimuth, 2.55941f, positionTolerance, "Moon Azimuth[6]");
			test->UnitTest(pos.Zenith, 1.29385f, positionTolerance, "Moon Zenith [6]");

			//CalculateStarRotation
			float rotationTolerance = 0.00001f;
			float starRot;

			starRot = CalculateStarRotation(22.0f, -7.0f);
			test->UnitTest(starRot, 10.12290966f, rotationTolerance, "StarRotation [1]");

			starRot = CalculateStarRotation(4.0f, 8.0f);
			test->UnitTest(starRot, 1.483529864f, rotationTolerance, "StarRotation [2]");

			starRot = CalculateStarRotation(4.23f, -2.0f);
			test->UnitTest(starRot, 4.161737601f, rotationTolerance, "StarRotation [3]");

			starRot = CalculateStarRotation(15.092f, 11.0f);
			test->UnitTest(starRot, 3.60201051f, rotationTolerance, "StarRotation [4]");


			//CalculateCelestialNorthPoleZenith
			float zenith;

			zenith = CalculateCelestialNorthPoleZenith(21.06f * MATH::DegreesToRadiansf);
			test->UnitTest(zenith, 1.203229986f, rotationTolerance, "StarZenith [0]");

			zenith = CalculateCelestialNorthPoleZenith(-21.06f * MATH::DegreesToRadiansf);
			test->UnitTest(zenith, 1.938362667f, rotationTolerance, "StarZenith [1]");

			SkyPosition sun = CalculateSunPosition(6.6f, -6, MARCH, 13, 2015, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);
			SkyPosition moon = CalculateMoonPosition(6.6f, -6, MARCH, 13, 2015, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);
			SkyData tmp;
			
			tmp = CalculateSkyData(6.6f, -6, MARCH, 13, 2015, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);

			test->UnitTest(tmp.moonPos.Azimuth, moon.Azimuth, positionTolerance, "SkyData[1a]");
			test->UnitTest(tmp.moonPos.Zenith, moon.Zenith, positionTolerance, "SkyData[1b]");

			test->UnitTest(tmp.northStarZenith, 49.0f * MATH::DegreesToRadiansf, positionTolerance, "SkyData[2]");

			float phaseTolerance = 0.01f;

			test->UnitTest(tmp.phase, 267.164f, phaseTolerance, "SkyData[3]");

			test->UnitTest(tmp.starRotation, 5.829399701f, rotationTolerance, "SkyData[4]");

			//std::cout << "Phase: " << tmp.phase << std::endl;

			test->UnitTest(tmp.sunPos.Azimuth, sun.Azimuth, positionTolerance, "SkyData[5a]");
			test->UnitTest(tmp.sunPos.Zenith, sun.Zenith, positionTolerance, "SkyData[5b]");

			float moonPhaseTolerance = 2.0f;

			tmp = CalculateSkyData(13.5f, -7.0f, JANUARY, 2, 2003, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);
			
			//test->UnitTest(tmp.phase, 0, moonPhaseTolerance, "New Moon Correct");
			test->UnitTest(
				((std::abs(tmp.phase - 0.0f) <= moonPhaseTolerance) ||
				(std::abs(tmp.phase - 360.0f) <= moonPhaseTolerance)),
				"New Moon Correct [1]");
			//std::cout << "New Moon Phase: " << tmp.phase << std::endl;

			tmp = CalculateSkyData(22.516666f, -6.0f, JULY, 24, 2006, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);

			test->UnitTest(
				((std::abs(tmp.phase - 0.0f) <= moonPhaseTolerance) ||
				(std::abs(tmp.phase - 360.0f) <= moonPhaseTolerance)),
				"New Moon Correct [2]");
			//std::cout << "New Moon Phase: " << tmp.phase << std::endl;

			tmp = CalculateSkyData(16.783f, -7.0f, FEBRUARY, 18, 2015, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);

			test->UnitTest(
				((std::abs(tmp.phase - 0.0f) <= moonPhaseTolerance) ||
				(std::abs(tmp.phase - 360.0f) <= moonPhaseTolerance)),
				"New Moon Correct [3]");
			//std::cout << "New Moon Phase: " << tmp.phase << std::endl;

			tmp = CalculateSkyData(4.66666f, -6.0f, SEPTEMBER, 26, 1992, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);

			test->UnitTest(
				((std::abs(tmp.phase - 0.0f) <= moonPhaseTolerance) ||
				(std::abs(tmp.phase - 360.0f) <= moonPhaseTolerance)),
				"New Moon Correct [4]");
			//std::cout << "New Moon Phase: " << tmp.phase << std::endl;
			
			//for (int i = 0; i < 28; i++)
			//{
			//	tmp = CalculateSkyData(1.00f, -7.0f, MARCH, i, 2015, 41 * MATH::DegreesToRadiansf, -112 * MATH::DegreesToRadiansf);
			//
			//	std::cout << "DAY: " << i << "000 PhaseAngle: " << tmp.phase << std::endl;
			//}

			/*
			//timing for memcpy of the moonImageData
			unsigned char * rtn = new unsigned char[moonImageData.width * moonImageData.height * 4];
			int repititions = 100000000;
			clock_t t = clock();
			for (int i = 0; i < repititions; i++)
				memcpy(rtn, moonImageData.pixel_data, moonImageData.width * moonImageData.height * 4);
			t = clock() - t;

			std::cout << "Clicks: " << t << " Seconds: " << ((float)t) / CLOCKS_PER_SEC << " s" << std::endl;
			std::cout << "Clicks per memcpy: " << ((float)t) / repititions << " Seconds: " << ((((float)t) / repititions) / CLOCKS_PER_SEC) << std::endl;
			std::cout << "This means you can call memcpy on the moon image " << (1.0 / 60.0 / ((((double)t) / repititions) / CLOCKS_PER_SEC)) << " times per frame (at 60fps).\n";

			delete[] rtn;
			*/

			return test->GetSuccess();
		}

		bool BIOSkyTests()
		{
			XNELO::TESTING::OutStreamGenerator gen;
			gen.SetOutputStream(&std::cout);

			XNELO::TESTING::TestSuite tests("BIOSky Tests");
			tests.SetReportGenerator(&gen);

			//all tests added here
			tests.AddTestFunction(&MATH::MathTests);
			tests.AddTestFunction(&SkyPosition::Test);
			tests.AddTestFunction(&Vector3D::Test);
			tests.AddTestFunction(&Vector2D::Test);
			tests.AddTestFunction(&Date::Test);
			tests.AddTestFunction(&DateTime::Test);
			tests.AddTestFunction(&GPS::Test);
			tests.AddTestFunction(&LibraryTests);

			tests.ExecuteTests();

			return tests.GetSuccess();
		}
#else
		bool BIOSkyTests()
		{
			std::cout << "Testing Disabled." << std::endl;
			return false;
		}
#endif 





	}//end namespace SKY
}//end namespace BIO