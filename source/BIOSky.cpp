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

#include "../source/MoonTexture.c"

namespace BIO
{
	namespace SKY
	{
		SkyPosition CalculateSunPosition(float standardTime, float UTCoffset, DATE_MONTH month, unsigned int day, unsigned int year, float latitude, float longitude)
		{
			//Method from http://www.stjarnhimlen.se/comp/ppcomp.html
			// by: Paul Schlyter, Stockholm, Sweden
			int monthint = Date::MonthToInt(month);
			float UT = standardTime - UTCoffset;	// universal time

			float d = (float)(367 * (int)year - (7 * ((int)year + ((monthint + 9) / 12))) / 4 + (275 * monthint) / 9 + (int)day - 730530);
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

			float lon = MATH::RevolutionReductionDegrees(v + w);//longitude of sun in degrees

			//BIO_LOG_CRITICAL("long: " << lon);

			x = r * cos(lon * MATH::DegreesToRadiansf);
			y = r * sin(lon * MATH::DegreesToRadiansf);
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

			return SkyPosition(azimuth,MATH::PId2f - altitude);
		}

		RawGeometry * CreateSkyDomeGeometry(float radius, int numVerticalSegments, int numHorozontalSegments)
		{
			RawGeometry * rtnValue = new RawGeometry();

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

			return rtnValue;
		}

		unsigned char * CreateMoonTexture(int * width, int * height)
		{
			(*width) = imageData.width;
			(*height) = imageData.height;

			//format B G R A all 8 bits;
			unsigned char * rtn = new unsigned char[imageData.width * imageData.height * 4];

			for (unsigned int y = 0; y < imageData.height; y++)
			{
				for (unsigned int x = 0; x < imageData.width; x++)
				{
					int index = ((y * imageData.width) + x) * 4;

					rtn[index] = imageData.pixel_data[index + 2];
					rtn[index + 1] = imageData.pixel_data[index + 1];
					rtn[index + 2] = imageData.pixel_data[index];
					rtn[index + 3] = imageData.pixel_data[index + 3];
				}
			}

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
	}//end namespace SKY
}//end namespace BIO