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

#include "BIOSky.hpp"
#include <cmath>

#include "../source/MoonTexture.c"

namespace BIO
{
	namespace SKY
	{
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
			rtnValue->UVTextureCoordinates = new RawGeometry::Vector2D[totalVerts];
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
			rtnValue->UVTextureCoordinates[0].x = rtnValue->UVTextureCoordinates[0].y = 0.5f;
			vertOn++;

			double currHeight = radius;
			//double heightStep = radius / numHorozontalSegments;
			double currAngle = PId2;//BIOSKY_PI2;
			double angleStep = PId2 / (double)numHorozontalSegments;

			for (int i = 0; i < numHorozontalSegments; i++)
			{
				currAngle -= angleStep;

				//std::cout << "Current Angle: " << currAngle << " sin(currAngle): " << std::sin(currAngle) << std::endl;

				currHeight = std::sin(currAngle) * radius;

				if (i == numHorozontalSegments - 1)
					currHeight = 0;

				double currRadius = std::sqrt((radius * radius) - (currHeight * currHeight));

				double circleStep = PIx2 / numVerticalSegments;
				double currAngle = 0;
				for (int j = 0; j < numVerticalSegments; j++)
				{
					rtnValue->vertecies[vertOn].X = (float)(currRadius * std::cos(currAngle));
					rtnValue->vertecies[vertOn].Y = (float)(currHeight);
					rtnValue->vertecies[vertOn].Z = (float)(currRadius * std::sin(currAngle));

					rtnValue->UVTextureCoordinates[vertOn].x = (rtnValue->vertecies[vertOn].X / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
					rtnValue->UVTextureCoordinates[vertOn].y = (rtnValue->vertecies[vertOn].Z / (2 * (radius + rtnValue->vertecies[vertOn].Y))) + 0.5f;
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

					float dist = Distance((float)i, (float)j, c, c);

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