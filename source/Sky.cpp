/**
* @file Sky.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implement common functions.
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

#include "Sky.hpp"
#include "IDomeVertecies.hpp"

namespace BIO
{
	namespace SKY
	{
		Sky::Sky(IDomeGeometry * skydome) : _skydome(skydome), _moonPos(), _sunPos(), _error(OK)
		{
			if (_skydome == NULL)
				_error = BIOSKY_FAILED_TO_INIT__GEOMETRY_NULL;
		}

		Sky::~Sky()
		{
			_skydome = NULL;
		}

		void Sky::SetMoonPosition(float lunarAzimuth, float lunarZenith)
		{
			_moonPos.Azimuth = lunarAzimuth;
			_moonPos.Zenith = lunarZenith;

			//calculate unit positions
			float x = sin(lunarZenith) * sin(lunarAzimuth);
			float y = cos(lunarZenith);
			float z = sin(lunarZenith) * cos(lunarAzimuth);

			_skydome->SetMoonPosition(x, y, z);
		}

		void Sky::SetSunPosition(float SolarAzimuth, float SolarZenith)
		{
			_sunPos.Azimuth = SolarAzimuth;
			_sunPos.Zenith = SolarZenith;

			//calculate unit positions
			float x = sin(SolarZenith) * sin(SolarAzimuth);
			float y = cos(SolarZenith);
			float z = sin(SolarZenith) * cos(SolarAzimuth);

			_skydome->SetSunPosition(x,y,z);
		}

		BIOSKY_API void Sky::UpdateSkyColor()
		{
			double T = 3.5;
			PerezYxyCoefficients coeffs = GetPerezCoefficientsForTurbidity(T); //this doesn't need to be called every time this function is called.
			YyxColor sunYyx = GetYyxColorForZenithAndTurbidity(_sunPos.Zenith, T);//this doesn't need to be called every time this function is called.

			YyxColor Yyx;
			double gamma;
			RGBColor rgb;

			const float _103degrees = 1.79768913f; //103 degrees in radians
			const float _93degrees = 1.623156204f; //93 degrees in radians

			int alpha = 255;
			if (_sunPos.Zenith > _103degrees)//103 degrees
				alpha = 0;
			else if (_sunPos.Zenith >= _93degrees)//93 degrees
			{
				alpha -= (int)(((_sunPos.Zenith - _93degrees) / (_103degrees - _93degrees)) * 255);
			}
			//else alpha should be left at 255
			//BIO_LOG_CRITICAL("Sun Zenith: " << _sunPos.zenith << " alpha: " << alpha);


			//irr::video::S3DVertex * verts = (irr::video::S3DVertex *)_skydome->getMesh()->getMeshBuffer(0)->getVertices();
			IDomeVertecies * verts = _skydome->GetVertecies();
			for (int i = 0; i < verts->GetVertexCount(); i++)
			{
				Vector3D vertPos = verts->GetVertexPosition(i);
				SkyPosition pos = CartesianToSky(vertPos.X, vertPos.Y, vertPos.Z);

				if (PId2 <= pos.Zenith)
					pos.Zenith = (float)PId2 - 0.01f;

				gamma = GetPerezGamma(pos.Zenith, pos.Azimuth, _sunPos.Zenith, _sunPos.Azimuth);
				//if (MATH::BIO_PI2 < pointZenith)
				//	pointZenith = MATH::BIO_PI2;
				Yyx.Y = sunYyx.Y * GetPerezLuminance(pos.Zenith, gamma, coeffs.Y) / GetPerezLuminance(0, _sunPos.Zenith, coeffs.Y);
				Yyx.x = sunYyx.x * GetPerezLuminance(pos.Zenith, gamma, coeffs.x) / GetPerezLuminance(0, _sunPos.Zenith, coeffs.x);
				Yyx.y = sunYyx.y * GetPerezLuminance(pos.Zenith, gamma, coeffs.y) / GetPerezLuminance(0, _sunPos.Zenith, coeffs.y);

				rgb = GetRGBColorFromYxy(Yyx);

				//verts[i].Color.set(alpha, rgb.red * 255, rgb.green * 255, rgb.blue * 255);
				verts->SetVertexColor(i, alpha, (int)(rgb.red * 255), (int)(rgb.green * 255), (int)(rgb.blue * 255));
			}
		}

		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		//			Private Functions
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		Sky::PerezYxyCoefficients Sky::GetPerezCoefficientsForTurbidity(double turbidity)
		{
			PerezCoefficient coeffY, coeffx, coeffy;

			coeffY.A = .17872f * turbidity - 1.46303f;
			coeffY.B = -.3554f * turbidity + .42749f;
			coeffY.C = -.02266f * turbidity + 5.32505f;
			coeffY.D = .12064f * turbidity - 2.57705f;
			coeffY.E = -.06696f * turbidity + .37027f;

			coeffx.A = -.01925f * turbidity - .25922f;
			coeffx.B = -.06651f * turbidity + .00081f;
			coeffx.C = -.00041f * turbidity + .21247f;
			coeffx.D = -.06409f * turbidity - .89887f;
			coeffx.E = -.00325f * turbidity + .04517f;

			coeffy.A = -.01669f * turbidity - .26078f;
			coeffy.B = -.09495f * turbidity + .00921f;
			coeffy.C = -.00792f * turbidity + .21023f;
			coeffy.D = -.04405f * turbidity - 1.65369f;
			coeffy.E = -.01092f * turbidity + .05291f;

			PerezYxyCoefficients rtn;
			rtn.Y = coeffY;
			rtn.x = coeffx;
			rtn.y = coeffy;

			return rtn;//(PerezYxyCoefficients){ .Y = coeffY, .x = coeffx, .y = coeffy };
		}

		Sky::RGBColor Sky::GetRGBColorFromYxy(YyxColor YyxColor)
		{
			double Y = YyxColor.Y;
			double x = YyxColor.x;
			double y = YyxColor.y;

			double X = x / y * Y;
			double Z = ((1. - x - y) / y) * Y;

			RGBColor color;
			color.red = (float)(3.2404f * X - 1.5371f * Y - .4985f * Z);
			color.green = (float)(-.9692f * X + 1.8759f * Y + .0415f * Z);
			color.blue = (float)(0.0556f * X - .2040f * Y + 1.0573f * Z);

			float expo = -(1.f / 15000.f);
			color.red = fmax(0.0f, fmin(1.0f, 1.0f - exp(expo * color.red)));
			color.green = fmax(0.0f, fmin(1.0f, 1.0f - exp(expo * color.green)));
			color.blue = fmax(0.0f, fmin(1.0f, 1.0f - exp(expo * color.blue)));

			return color;
		}

		Sky::YyxColor Sky::GetYyxColorForZenithAndTurbidity(double zenith, double turbidity)
		{
			YyxColor color;
			double zenith2 = pow(zenith, 2);
			double zenith3 = pow(zenith, 3);
			double turbidity2 = pow(turbidity, 2);

			color.Y = ((4.0453f * turbidity - 4.971f) * tan((4.f / 9.f - turbidity / 120.f) * (PI - 2 * zenith)) - 0.2155f * turbidity + 2.4192f) * 1000.0f;
			//double Yz = (4.0453 * turbidity - 4.9710) * tan((4.0 / 9 - turbidity / 120.0f) * (MATH::BIO_PI - 2 * zenith)) - 0.2155 * turbidity + 2.4192;
			//double Y0 = (4.0453 * turbidity - 4.9710) * tan((4.0 / 9 - turbidity / 120.0f) * (MATH::BIO_PI)) - 0.2155 * turbidity + 2.4192;
			//color.Y = Yz / Y0;
			color.x = (.00165f * zenith3 - .00375f * zenith2 + .00209f * zenith + 0.f) * turbidity2 + (-0.02903f * zenith3 + .06377f * zenith2 - .03202f * zenith + .00394f) * turbidity + (.11693f * zenith3 - .21196f * zenith2 + .06052f * zenith + .25886f);
			color.y = (.00275f * zenith3 - .0061f * zenith2 + .00317f * zenith + 0.f) * turbidity2 + (-0.04214f * zenith3 + .0897f * zenith2 - .04153f * zenith + .00516f) * turbidity + (.15346f * zenith3 - .26756f * zenith2 + .0667f * zenith + .26688f);

			return color;
		}
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//End private Functions
		///////////////////////////////////////////////////////////////////////
	}//end namespace SKY
}//end namespace BIO


