/**
* @file Sky.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implements the basic functions that are common to all skies as defined in 
* ISky interface.
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

#ifndef ___BIOSKY_SKY_HPP__2015___
#define ___BIOSKY_SKY_HPP__2015___

#include "CompileConfig.h"
#include "ISky.hpp"
#include "IDomeGeometry.hpp"
#include "MathUtils.hpp"
#include <vector>

namespace BIO
{
	namespace SKY
	{
		/**
		* This class defines the interface for a sky class. All BIOSky Skies
		* should inherit from this class. This is an abstract class.
		*/
		class Sky : public ISky
		{
		private:
			struct RGB
			{
				float R;
				float G;
				float B;
			};
			struct InterpolationData
			{
				float angle;
				RGB light;
			};

		protected:
			/**
			* Pointer to the skydome geometry
			*/
			IDomeGeometry * _skydome;
			/**
			* The position of the moon.
			*/
			SkyPosition _moonPos;
			/**
			* The position of the sun.
			*/
			SkyPosition _sunPos;
			/**
			* The error code for this class. If it is 0 or BIO::SKY::OK then 
			* there was no error. Otherwise there is an error.
			*/
			ErrorType _error;

			/**
			*
			*/
			std::vector<InterpolationData> _lightInterpolation;

			/**
			* A structure to hold the coefficients used in the Perez skymodel
			* calculations.
			*/
			struct PerezCoefficient 
			{
				double A, B, C, D, E;
			};

			/**
			* A structure to hold the color coefficients for the Perez skymodel
			* calculations.
			*/
			struct PerezYxyCoefficients
			{
				PerezCoefficient Y, x, y;
			};

			/**
			* A structure for holding an RGB color.
			*/
			struct RGBColor
			{
				float red, green, blue;
			};

			/**
			* A structure that holds a color in Yyx format.
			*/
			struct YyxColor {
				double Y, y, x;
			};

			/**
			* This function calculates the coefficients used in the perez
			* equation calculated with a specific turbidity.
			*
			* @note The code in this function is from boliva on gamedev.net and
			*		obtained from http://www.gamedev.net/user/222007-boliva/
			*
			* @param turbidity The turbidity to calculate coefficients for.
			*
			* @return Returns the PerezYxyCoefficients.
			*/
			BIOSKY_API static PerezYxyCoefficients GetPerezCoefficientsForTurbidity(double turbidity);

			/**
			* Calculates the difference between the sun's position and the
			* position of the point we are determining the color for.
			*
			* @note The code in this function is from boliva on gamedev.net and
			*		obtained from http://www.gamedev.net/user/222007-boliva/
			*
			* @param zenith The zenith of the point we are calculating the
			*			color for.
			*
			* @param azimuth The azimuth of the point we are calculating the
			*			color for.
			*
			* @param solarzenith The zenith of the sun.
			*
			* @param solarazimuth The azimuth of the sun.
			*
			* @return Returns the angle between the point on the skydome we are
			*			calculating the color for and the sun's position.
			*/
			BIOSKY_API static double GetPerezGamma(float zenith, float azimuth, float solarzenith, float solarazimuth);

			/**
			* This calculates the luminance at a specific point.
			*
			* @note The code in this function is from boliva on gamedev.net and
			*		obtained from http://www.gamedev.net/user/222007-boliva/
			*
			* @param zenith The Zenith of the point we are calculating.
			*
			* @param gamma The difference in angle between sun position and the
			*			position we are calculating luminance for.
			*
			* @param coeff The coefficients to start with.
			*
			* @return Returns a double with the amount of luminance at this
			*			point in the sky.
			*/
			BIOSKY_API static double GetPerezLuminance(double zenith, double gamma, PerezCoefficient coeff);

			/**
			* Convert an YxyColor into an RGB color. The RGBColor hase the red,
			* green, and blue components of the color.
			*
			* @note The code in this function is from boliva on gamedev.net and
			*		obtained from http://www.gamedev.net/user/222007-boliva/
			*
			* @param YyxColor The color in Yxy format.
			*
			* @return Returns an RGBColor struct with the converted color info.
			*/
			BIOSKY_API static RGBColor GetRGBColorFromYxy(YyxColor YyxColor);

			/**
			* This function gets the Color of the zenith color values for the
			* perez equation.
			*
			* @note The code in this function is from boliva on gamedev.net and
			*		obtained from http://www.gamedev.net/user/222007-boliva/
			*
			* @param zenith The zenith of the sun.
			*
			* @param turbidity The turbidity of the air.
			*
			* @retunr Returns an YyxColor structure with the zenith color.
			*/
			BIOSKY_API static YyxColor GetYyxColorForZenithAndTurbidity(double zenith, double turbidity);
		public:
			/**
			* Constructor
			*/
			BIOSKY_API Sky(IDomeGeometry * skydome);

			/**
			* Destructor
			*/
			BIOSKY_API virtual ~Sky();

			/**
			* Calculate the Color of the skylights. This function depends on
			* the position of the sun and moon to determin the color of the
			* lights due to sky phenomena.
			*
			* @NOTE This function depends on sun and moon positions.
			*
			* @return Returns a
			*/
			BIOSKY_API virtual LightData CalculateSkyLights();

			/**
			* Converts a cartesian coordinate (x,y,z) into a sky coordinate
			* (azimuth, zenith).
			*
			* @param x The x cartesian coordinate.
			*
			* @param y The y cartesian coordinate.
			*
			* @param z The z cartesian coordinate.
			*
			* @return Returns a 'SkyPosition' struct with the azimuth and
			*			zenith values of the cartesian coordinate.
			*/
			BIOSKY_API static SkyPosition CartesianToSky(float x, float y, float z);

			/**
			* Get the error code for this class. If the return value is 0 or ==
			* BIO::SKY::OK then there is no error. If the value is != 0 then 
			* there was an error. Use BIO::SKY::ErrorCodeToString function to
			* get string version of the error.
			*
			* @return Returns the error code.
			*/
			BIOSKY_API virtual ErrorType GetErrorCode();

			/**
			* Set the phase of the moon and simultaneously update the moon
			* texture.
			*
			* @param phase The phase of the moon. This value must be within the
			*			range of [0,360] inclusive. 0 and 360 corrosponds to
			*			the new moon, 45 to Waxing cresent, 90 first quarter,
			*			..., 180 to full moon, 135 to waning gibbous, ect.
			*
			* @note See https://www.youtube.com/watch?v=XX4mRAfTkTE for
			*		detailed explination. See
			*		http://xnelo.com/images/LunarPhase.png for an image
			*		explaining the phase angle.
			*/
			BIOSKY_API virtual void SetMoonPhase(float phase);

			/**
			* Set the position in the sky of the moon.
			*
			* @param pos A struct containing the position of the moon.
			*/
			BIOSKY_API virtual void SetMoonPosition(SkyPosition pos);

			/**
			* Set the position in the sky of the moon.
			*
			* @param lunarAzimuth The angle from north.
			*
			* @param lunarZenith The angle from the zenith position.
			*/
			BIOSKY_API virtual void SetMoonPosition(float lunarAzimuth, float lunarZenith);

			/**
			* Set the visibility of the moon.
			*
			* @param visibility A value between [0, 1] for the visibility of
			*			the moon. 0 the moon is completely invisible, and 1
			*			the moon is completely visible.
			*/
			BIOSKY_API virtual void SetMoonVisibility(float visibility);

			/**
			* Set SkyLight Data.
			*/
			BIOSKY_API virtual void SetSkyLights(LightData & lightData);

			/**
			* Set the position of the Stars.
			*
			* @param zenith The zenith is the only thing that changes on the
			*			position of the stars. The azimuth is always pointing
			*			north and the zenith depends on latitude. The equation
			*			is (PI/2) - latitude Radians (90 - latitude Degrees).
			*			This function takes the zenith in Radians.
			*/
			BIOSKY_API virtual void SetStarPosition(float zenith, float rotation);

			/**
			* Set the position in the sky of the sun.
			*
			* @param pos A struct containing the position of the sun.
			*/
			BIOSKY_API virtual void SetSunPosition(SkyPosition pos);

			/**
			* Set the position in the sky of the sun.
			*
			* @param SolarAzimuth The angle from north.
			*
			* @param SolarZenith The angle from the zenith position.
			*/
			BIOSKY_API virtual void SetSunPosition(float SolarAzimuth, float SolarZenith);

			/**
			* Update the sky simulation according to the currently stored time
			* and location in this object.
			*
			* @note By calling this function you are responsible for updating
			*		the DateTime object on your own.
			*/
			BIOSKY_API virtual void Update() = 0;

			/**
			* Update the sky simulation according to the passed in time. The
			* DateTime object will be updated first... then the sky will be
			* updated according to the current time and location stored in this
			* object.
			*
			* @param deltaTime The time passed since the last call to this
			*			Update function.
			*/
			BIOSKY_API virtual void Update(float deltaTime) = 0;

			/**
			* Update All the sky objects. Calling this function is equivalent
			* to calling all of the UpdateMoonPosition, UpdateStarPosition,
			* UpdateStarRotation, UpdateSkyColor, and UpdateSunPosition. This
			* function is however slightly optimized so that calculations do
			* not need to be made twice.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateAllSkyObjects() = 0;

			/**
			* Update the Moon's position with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateMoonPosition() = 0;

			BIOSKY_API virtual void UpdateSkyLights();

			/**
			* Update the position of the North Star according to the current
			* latitude of the observer.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateStarPosition() = 0;

			/**
			* Update the rotation of the stars in the sky. This depends on the
			* current time and longitude.
			*
			* @note If an Update functin is called then this function should
			*		NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateStarRotation() = 0;

			/**
			* Update the sky color.
			*
			* @note In classes that derive from BIO::SKY::IBIOSkyStatic this
			*		function will not be automatically updated. If you change
			*		the sun's position then you need to call this function if
			*		you want the sky colors to be updated to the suns position.
			*		In sky classes that derive from BIO::SKY::BIOSkyDynamic then
			*		the updating of the sky is automatically taken care of in
			*		when either of the IBIOSky::Update functions are called.
			*/
			BIOSKY_API virtual void UpdateSkyColor();

			/**
			* Update the SunPosition with the current parameters.
			*
			* @note This function should NOT be called every frame.
			*/
			BIOSKY_API virtual void UpdateSunPosition() = 0;

#if BIOSKY_TESTING == 1
			BIOSKY_API static bool Tests(XNELO::TESTING::Test * test);
#endif
		};
	}//end namespace SKY
}//end namespace BIO

inline BIO::SKY::SkyPosition BIO::SKY::Sky::CartesianToSky(float x, float y, float z)
{
	//float radius = sqrt((x*x) + (y*y) + (z*z));

	SkyPosition rtn;
	rtn.Azimuth = atan2(x, z);
	rtn.Zenith = atan2(sqrt((x*x) + (z*z)), y);

	return rtn;
}

inline BIO::ErrorType BIO::SKY::Sky::GetErrorCode()
{
	return _error;
}

inline void BIO::SKY::Sky::SetMoonPosition(SkyPosition pos)
{
	SetMoonPosition(pos.Azimuth, pos.Zenith);
}

inline void BIO::SKY::Sky::SetSkyLights(LightData & lightData)
{
	_skydome->SetSkyLight(lightData);
}

inline void BIO::SKY::Sky::SetStarPosition(float zenith, float rotation)
{
	_skydome->SetStarRotation(zenith, rotation, 0);
}

inline void BIO::SKY::Sky::SetSunPosition(SkyPosition pos)
{
	SetSunPosition(pos.Azimuth, pos.Zenith);
}

inline void BIO::SKY::Sky::UpdateSkyLights()
{
	LightData data = CalculateSkyLights();
	SetSkyLights(data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//							PRivate Functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
inline double BIO::SKY::Sky::GetPerezGamma(float zenith, float azimuth, float solarzenith, float solarazimuth)
{
	double a = sin(zenith) * sin(solarzenith) * cos(solarazimuth - azimuth) + cos(zenith) * cos(solarzenith);
	if (a > 1)
		return 0;
	if (a < -1)
		return MATH::PI;
	return acos(a);
}

inline double BIO::SKY::Sky::GetPerezLuminance(double zenith, double gamma, PerezCoefficient coeff)
{
	double cosZenith = (zenith == MATH::PId2 ? .0000001 : cos(zenith));
	return (1 + coeff.A * exp(coeff.B / cosZenith)) * (1 + coeff.C * exp(coeff.D * gamma) + coeff.E * pow(cos(gamma), 2));
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//						End Private Functions
///////////////////////////////////////////////////////////////////////////////

#endif //___BIOSKY_SKY_HPP__2015___