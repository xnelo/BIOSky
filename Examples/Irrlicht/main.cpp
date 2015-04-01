/**
* @file main.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Example program using Irrlicht.
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

//visual leak detector
//#include <vld.h>
//#pragma comment(lib, "vld")

#include <iostream>
#include <cwchar>//needed for wcstol, wcstof, wsclen

#include "irrlicht-1.8.1\include\irrlicht.h"
#include "BIOSky.hpp"
#include "IrrBIOSkyDome.hpp"

#define WINDOW_CAPTION L"BIOSKY: Irrlicht Example"
#define GROUND_NAME "BIOSKYTestGround"
#define SKYDOME_ID 1324

BIO::SKY::ISky * sky = NULL;
IrrBIOSkyDome * dome = NULL;

BIO::SKY::SkyPosition _staticSunPos;
BIO::SKY::SkyPosition _staticMoonPos;

BIO::DateTime _dateTime;
BIO::GPS _gps;

bool runTime = false;
float timeSpeed = 1.0f;

float DegreesToRadians(float degrees)
{
	return degrees * (BIO::MATH::PIf / 180.0f);
}

enum GUI_IDS
{
	MAIN_WINDOW = 401,
	MONTH_CBO_BOX,
	DAY_CBO_BOX,
	YEAR_EDIT,
	HOUR_EDIT,
	MIN_EDIT,
	SEC_EDIT,
	LAT_EDIT,
	SOUTH_CHECK,
	LONGITUDE_EDIT,
	WEST_CHECK,
	START_TIME_BUTTON,
	STOP_TIME_BUTTON,
	STATIC_SKY_CHECK,
	CALC_SKY_CHECK,
	DYNAMIC_SKY_CHECK,
	AZIMUTH_EDIT,
	ZENITH_EDIT,
	MOON_AZIMUTH_EDIT,
	MOON_ZENITH_EDIT,
	SIM_SPEED_SCROLL,
	SIM_SPEED_TEXT,
	SEC_TO_HOURS_CHECK
};

irr::gui::IGUIComboBox * month;
irr::gui::IGUIComboBox * day;
irr::gui::IGUIEditBox * year;
irr::gui::IGUIEditBox * hour;
irr::gui::IGUIEditBox * min;
irr::gui::IGUIEditBox * sec;
irr::gui::IGUIEditBox * lat;
irr::gui::IGUICheckBox * south;
irr::gui::IGUIEditBox * longitude;
irr::gui::IGUICheckBox * west;
irr::gui::IGUIButton * startTime;
irr::gui::IGUIButton * stopTime;
irr::gui::IGUICheckBox * staticSky;
irr::gui::IGUICheckBox * calcSky;
irr::gui::IGUICheckBox * dynamicSky;
irr::gui::IGUIEditBox * azimuth;
irr::gui::IGUIEditBox * zenith;
irr::gui::IGUIEditBox * moonAzimuth;
irr::gui::IGUIEditBox * moonZenith;
irr::gui::IGUIScrollBar * simSpeed;
irr::gui::IGUIStaticText * simSpeedText;
irr::gui::IGUIStaticText * simSpeedUnits;
irr::gui::IGUICheckBox * secToMin;


bool isLeapYear(unsigned int year)
{
	if ((year % 4) == 0)
	{//it is divisible by 4
		if ((year % 100) == 0)
		{//it is divisible by 100
			if ((year % 400) == 0)
			{//it is divisible by 400
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

int daysInMonth(int month, unsigned int year)
{
	switch (month)
	{
	case 2:
		if (isLeapYear(year))
			return 29;
		return 28;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return 31;
	}
}

void setDate()
{
	//get the date information
	int monthVal = month->getItemData(month->getSelected());
	int dayVal = day->getItemData(day->getSelected());
	int yearVal = (int)wcstol(year->getText(), NULL, 10);

	if (dayVal > daysInMonth(monthVal, yearVal))
	{
		dayVal = daysInMonth(monthVal, yearVal);
		day->setSelected(dayVal - 1);
	}
	std::cout << "Setting Date: " << monthVal << "/" << dayVal << "/" << yearVal << std::endl;

	_dateTime.SetMonth(BIO::Date::IntToMonth(monthVal));
	_dateTime.SetDay(dayVal);
	_dateTime.SetYear(yearVal);

	if (sky)
	{
		//sky->UpdateSunPosition();
		//sky->UpdateMoonPosition();
		//sky->UpdateSkyColor();
		sky->UpdateAllSkyObjects();
	}
}

void setTime()
{
	//get the information
	int hourVal = (int)wcstol(hour->getText(), NULL, 10);
	int minVal = (int)wcstol(min->getText(), NULL, 10);
	int secVal = (int)wcstol(sec->getText(), NULL, 10);

	if (secVal >= 60)
	{
		secVal = 0;
		sec->setText(L"00");
	}

	if (minVal >= 60)
	{
		minVal = 0;
		min->setText(L"00");
	}

	if (hourVal >= 24)
	{
		hourVal = 0;
		hour->setText(L"00");
	}

	float hours = hourVal + ((float)minVal / 60.0f) + ((float)secVal / 3600);
	_dateTime.SetTimeHours(hours);

	if (sky)
	{
		//sky->UpdateSunPosition();
		//sky->UpdateMoonPosition();
		//sky->UpdateSkyColor();
		sky->UpdateAllSkyObjects();
	}
}

void setGPS()
{
	float latitude = wcstof(lat->getText(), NULL);
	float longitud = wcstof(longitude->getText(), NULL);

	if (south->isChecked())
		latitude *= -1;

	if (west->isChecked())
		longitud *= -1;

	std::cout << "Setting GPS: " << latitude << " | " << longitud << std::endl;

	_gps.SetLatitude(latitude);
	_gps.SetLongitude(longitud);

	if (sky)
	{
		//sky->UpdateSunPosition();
		//sky->UpdateMoonPosition();
		//sky->UpdateSkyColor();
		sky->UpdateAllSkyObjects();
	}
}

void deleteSky()
{
	if (sky != NULL)
	{
		delete sky;
		sky = NULL;
	}
}

void enableCalculations(bool enabled)
{
	month->setEnabled(enabled);
	day->setEnabled(enabled);
	year->setEnabled(enabled);
	hour->setEnabled(enabled);
	min->setEnabled(enabled);
	sec->setEnabled(enabled);
	lat->setEnabled(enabled);
	longitude->setEnabled(enabled);
	south->setEnabled(enabled);
	west->setEnabled(enabled);
	
}

void enableTimer(bool enabled)
{
	startTime->setEnabled(enabled);
	stopTime->setEnabled(enabled);
	simSpeed->setEnabled(enabled);
	simSpeedText->setEnabled(enabled);
	simSpeedUnits->setEnabled(enabled);
	secToMin->setEnabled(enabled);
}

void enableStatic(bool enabled)
{
	azimuth->setEnabled(enabled);
	zenith->setEnabled(enabled);
	moonAzimuth->setEnabled(enabled);
	moonZenith->setEnabled(enabled);
}

void enableStaticSky()
{
	enableTimer(false);
	enableCalculations(false);
	enableStatic(true);
	dynamicSky->setChecked(false);
	calcSky->setChecked(false);

	//setup a static sky
	deleteSky();
	sky = new BIO::SKY::SkyManual(dome, _staticSunPos.Azimuth, _staticSunPos.Zenith, _staticMoonPos.Azimuth, _staticMoonPos.Zenith);
}

void enableDynamicSky()
{
	enableTimer(true);
	enableCalculations(true);
	enableStatic(false);
	staticSky->setChecked(false);
	calcSky->setChecked(false);
	
	deleteSky();
	setTime();
	setDate();
	setGPS();
	sky = new BIO::SKY::SkyCalculatedDynamic(dome, &_dateTime, &_gps);
}

void enableCalculatedSky()
{
	enableTimer(false);
	enableCalculations(true);
	enableStatic(false);
	dynamicSky->setChecked(false);
	staticSky->setChecked(false);

	deleteSky();
	setTime();
	setDate();
	setGPS();
	sky = new BIO::SKY::SkyCalculatedStatic(dome, &_dateTime, &_gps);
}



bool checkFloatNumber(const wchar_t * str)
{
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		if ((str[i] != L'0') &&
			(str[i] != L'1') &&
			(str[i] != L'2') &&
			(str[i] != L'3') &&
			(str[i] != L'4') &&
			(str[i] != L'5') &&
			(str[i] != L'6') &&
			(str[i] != L'7') &&
			(str[i] != L'8') &&
			(str[i] != L'9') &&
			(str[i] != L'.'))
			return false;
	}
	return true;
}

bool checkNumber(const wchar_t * str)
{
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		if ((str[i] != L'0') &&
			(str[i] != L'1') &&
			(str[i] != L'2') &&
			(str[i] != L'3') &&
			(str[i] != L'4') &&
			(str[i] != L'5') &&
			(str[i] != L'6') &&
			(str[i] != L'7') &&
			(str[i] != L'8') &&
			(str[i] != L'9'))
			return false;
	}
	return true;
}

void setStaticSky()
{
	_staticSunPos.Azimuth = DegreesToRadians(wcstof(azimuth->getText(), NULL));
	_staticSunPos.Zenith = DegreesToRadians(wcstof(zenith->getText(), NULL));
	_staticMoonPos.Azimuth = DegreesToRadians(wcstof(moonAzimuth->getText(), NULL));
	_staticMoonPos.Zenith = DegreesToRadians(wcstof(moonZenith->getText(), NULL));

	if (sky)
	{
		sky->SetSunPosition(_staticSunPos.Azimuth, _staticSunPos.Zenith);
		sky->SetMoonPosition(_staticMoonPos.Azimuth, _staticMoonPos.Zenith);
		sky->UpdateSkyColor();
		sky->UpdateSkyLights();
	}
}

void updateTime(float deltaSeconds)
{
	_dateTime.AddTime(deltaSeconds * timeSpeed);

	//set all the time things again.
	month->setSelected(BIO::Date::MonthToInt(_dateTime.GetMonth()) - 1);
	day->setSelected(_dateTime.GetDay() - 1);

	year->setText(std::to_wstring(_dateTime.GetYear()).c_str());

	float timeHours = _dateTime.GetTimeHours();
	int outHours = (int)timeHours;
	timeHours -= outHours;
	timeHours *= 60;//num of minutes
	int outMin = (int)timeHours;
	timeHours -= outMin;
	timeHours *= 60;//num of seconds
	int outSec = (int)timeHours;

	hour->setText(std::to_wstring(outHours).c_str());
	min->setText(std::to_wstring(outMin).c_str());
	sec->setText(std::to_wstring(outSec).c_str());
}

void changeSimulationSpeed()
{
	float tmpVal = (float)simSpeed->getPos();
	tmpVal /= 10.0f;

	std::wstring output = std::to_wstring(tmpVal);

	simSpeedText->setText(output.c_str());

	timeSpeed = tmpVal;
	if (secToMin->isChecked())
		timeSpeed *= 60;
}










class DefaultEventReceiver : public irr::IEventReceiver
{
private:
	irr::gui::ICursorControl * _cursorControl;
	irr::scene::ICameraSceneNode * _camera;
	bool _moveCameraForward;
	bool _moveCameraBackward;
	bool _strafeLeft;
	bool _strafeRight;

	bool _leftMouseDown;
	irr::core::vector2df _mouseStartPos;
	irr::core::vector2df _mouseDelta;
public:
	/**
	* Constructor
	*/
	DefaultEventReceiver(irr::gui::ICursorControl * cursorControl);

	/**
	* Destructor
	*/
	virtual ~DefaultEventReceiver();

	/**
	* @override
	*/
	virtual bool OnEvent(const irr::SEvent& event);

	/**
	* Set the Camera that this event reciever will manipulate.
	*
	* @param camera A pointer to a camera objects.
	*/
	void SetCamera(irr::scene::ICameraSceneNode * camera);

	bool handleGUI(const irr::SEvent& event);

	void Update(float deltaTime);
};

inline DefaultEventReceiver::DefaultEventReceiver(irr::gui::ICursorControl * cursorControl) :
_cursorControl(cursorControl),
_camera(NULL),
_moveCameraForward(false),
_moveCameraBackward(false),
_strafeLeft(false),
_strafeRight(false),
_leftMouseDown(false),
_mouseStartPos(0.0f, 0.0f),
_mouseDelta(0.0f, 0.0f)
{
	if (cursorControl)
		_cursorControl->grab();
}

DefaultEventReceiver::~DefaultEventReceiver()
{
	if (_cursorControl)
		_cursorControl->drop();

	_cursorControl = NULL;

	_camera = NULL;
}

bool DefaultEventReceiver::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_GUI_EVENT)
	{
		return handleGUI(event);
	}

	if (_camera == NULL)
		return false;

	if (event.KeyInput.Key == irr::EKEY_CODE::KEY_KEY_W)
	{
		if (event.KeyInput.PressedDown)
		{
			_moveCameraForward = true;
		}
		else
		{
			_moveCameraForward = false;
		}
	}
	else if (event.KeyInput.Key == irr::EKEY_CODE::KEY_KEY_S)
	{
		if (event.KeyInput.PressedDown)
		{
			_moveCameraBackward = true;
		}
		else
		{
			_moveCameraBackward = false;
		}
	}
	else if (event.KeyInput.Key == irr::EKEY_CODE::KEY_KEY_A)
	{
		if (event.KeyInput.PressedDown)
		{
			_strafeLeft = true;
		}
		else
		{
			_strafeLeft = false;
		}
	}
	else if (event.KeyInput.Key == irr::EKEY_CODE::KEY_KEY_D)
	{
		if (event.KeyInput.PressedDown)
		{
			_strafeRight = true;
		}
		else
		{
			_strafeRight = false;
		}
	}

	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		const irr::core::vector2df mouseCenter = irr::core::vector2df(0.5f, 0.5f);

		switch (event.MouseInput.Event)
		{
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			_leftMouseDown = true;
			_mouseStartPos = _cursorControl->getRelativePosition();
			_cursorControl->setPosition(mouseCenter);
			break;

		case irr::EMIE_RMOUSE_LEFT_UP:
			_leftMouseDown = false;
			_cursorControl->setPosition(_mouseStartPos);
			break;

		case irr::EMIE_MOUSE_MOVED:
			if (_leftMouseDown)
			{
				irr::core::vector2df mPos = _cursorControl->getRelativePosition();
				if (mPos != mouseCenter)
				{
					_mouseDelta.X = mPos.X - mouseCenter.X;
					_mouseDelta.Y = mPos.Y - mouseCenter.Y;

					_cursorControl->setPosition(mouseCenter);
				}
			}
			break;

		default:
			// We won't use the wheel
			break;
		}
	}


	return false;//
}

void DefaultEventReceiver::SetCamera(irr::scene::ICameraSceneNode * camera)
{
	_camera = camera;
}

void DefaultEventReceiver::Update(float deltaTime)
{
	const irr::core::vector2df Zero(0.0f, 0.0f);

	if (_camera == NULL)
		return;

	const float distFromLookAt = 5.0f;
	const float cameraMoveSpeed = 5.0f;
	const float cameraRotationSpeed = 10.0f;

	if (_moveCameraForward)
	{
		_camera->setPosition(_camera->getPosition() + (_camera->getRotation().rotationToDirection() *deltaTime*cameraMoveSpeed));
	}
	else if (_moveCameraBackward)
	{
		_camera->setPosition(_camera->getPosition() + ((_camera->getRotation().rotationToDirection() * -1)*deltaTime*cameraMoveSpeed));
	}

	if (_strafeLeft)
	{
		_camera->setPosition(_camera->getPosition() + (_camera->getRotation().rotationToDirection(irr::core::vector3df(-1, 0, 0))*deltaTime*cameraMoveSpeed));
	}
	else if (_strafeRight)
	{
		_camera->setPosition(_camera->getPosition() + (_camera->getRotation().rotationToDirection(irr::core::vector3df(1, 0, 0))*deltaTime*cameraMoveSpeed));
	}

	if (_mouseDelta != Zero)
	{
		//rotate the camera
		irr::core::vector3df camRot = _camera->getRotation();
		camRot += irr::core::vector3df(_mouseDelta.Y * cameraRotationSpeed, _mouseDelta.X * cameraRotationSpeed, 0.0f);

		if (camRot.X > 89)
			camRot.X = 89;
		else if (camRot.X < -89)
			camRot.X = -89;

		if (camRot.Y > 360)
			camRot.Y -= 360;
		else if (camRot.Y < 0)
			camRot.Y += 360;

		_camera->setRotation(camRot);
	}

	_camera->setTarget(_camera->getPosition() + (_camera->getRotation().rotationToDirection()*distFromLookAt));

	_mouseDelta.X = 0.0f;
	_mouseDelta.Y = 0.0f;
}

bool DefaultEventReceiver::handleGUI(const irr::SEvent& event)
{
	irr::s32 id = event.GUIEvent.Caller->getID();

	switch (event.GUIEvent.EventType)
	{
	case irr::gui::EGET_SCROLL_BAR_CHANGED:
		if (id == SIM_SPEED_SCROLL)
		{
			changeSimulationSpeed();
			return true;
		}
	case irr::gui::EGET_EDITBOX_CHANGED:
		switch (id)
		{
		case YEAR_EDIT:
			runTime = false;
			if (!checkNumber(year->getText()))
				year->setText(L"2003");
			setDate();
			return true;
		case LAT_EDIT:
			runTime = false;
			if (!checkFloatNumber(lat->getText()))
				lat->setText(L"41");
			if (wcstof(lat->getText(), NULL) > 90.0f)
				lat->setText(L"90");
			setGPS();
			return true;
		case LONGITUDE_EDIT:
			runTime = false;
			if (!checkFloatNumber(longitude->getText()))
				longitude->setText(L"112");
			if (wcstof(longitude->getText(), NULL) > 180.0f)
				longitude->setText(L"180");
			setGPS();
			return true;
		case AZIMUTH_EDIT:
			if (!checkFloatNumber(azimuth->getText()))
				azimuth->setText(L"45");
			if (wcstof(azimuth->getText(), NULL) > 360.0f)
				azimuth->setText(L"45");
			setStaticSky();
			return true;
		case ZENITH_EDIT:
			if (!checkFloatNumber(zenith->getText()))
				zenith->setText(L"45");
			if (wcstof(zenith->getText(), NULL) > 180.0f)
				zenith->setText(L"45");
			setStaticSky();
			return true;
		case MOON_AZIMUTH_EDIT:
			if (!checkFloatNumber(moonAzimuth->getText()))
				moonAzimuth->setText(L"100");
			if (wcstof(moonAzimuth->getText(), NULL) > 360.0f)
				moonAzimuth->setText(L"100");
			setStaticSky();
			return true;
		case MOON_ZENITH_EDIT:
			if (!checkFloatNumber(moonZenith->getText()))
				moonZenith->setText(L"100");
			if (wcstof(moonZenith->getText(), NULL) > 180.0f)
				moonZenith->setText(L"100");
			setStaticSky();
			return true;
		case HOUR_EDIT:
			runTime = false;
			if (!checkNumber(hour->getText()))
				hour->setText(L"4");
			setTime();
			return true;
		case MIN_EDIT:
			runTime = false;
			if (!checkNumber(min->getText()))
				min->setText(L"30");
			setTime();
			return true;
		case SEC_EDIT:
			runTime = false;
			if (!checkNumber(sec->getText()))
				sec->setText(L"00");
			setTime();
			return true;
		default:
			return false;
		}
	case irr::gui::EGET_COMBO_BOX_CHANGED:
		std::cout << "COmbo Box changed" << std::endl;
		switch (id)
		{
		case MONTH_CBO_BOX:
			runTime = false;
			setDate();
			return true;
		case DAY_CBO_BOX:
			runTime = false;
			setDate();
			return true;
		default:
			return false;
		}
	case irr::gui::EGET_BUTTON_CLICKED:
		switch (id)
		{
		case START_TIME_BUTTON:
			std::cout << "Start Timer" << std::endl;
			runTime = true;
			return true;
		case STOP_TIME_BUTTON:
			std::cout << "Stop Timer" << std::endl;
			runTime = false;
			return true;
		default:
			return false;
		}
	case irr::gui::EGET_CHECKBOX_CHANGED:
		switch (id)
		{
		case SEC_TO_HOURS_CHECK:
			if (secToMin->isChecked())
			{
				simSpeedUnits->setText(L"1 Sec =             mins");
			}
			else
			{
				simSpeedUnits->setText(L"1 Sec =             seconds");
			}
			changeSimulationSpeed();
			return true;
		case STATIC_SKY_CHECK:
			runTime = false;
			if ((calcSky->isChecked() == false) && (dynamicSky->isChecked() == false))
				staticSky->setChecked(true);
			enableStaticSky();
			return true;
		case CALC_SKY_CHECK:
			runTime = false;
			if ((staticSky->isChecked() == false) && (dynamicSky->isChecked() == false))
				calcSky->setChecked(true);
			enableCalculatedSky();
			return true;
		case DYNAMIC_SKY_CHECK:
			runTime = false;
			if ((calcSky->isChecked() == false) && (staticSky->isChecked() == false))
				dynamicSky->setChecked(true);
			enableDynamicSky();
			return true;
		case SOUTH_CHECK:
			runTime = false;
			setGPS();
			return true;
		case WEST_CHECK:
			runTime = false;
			setGPS();
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
}





























int Init(irr::IrrlichtDevice * dev, DefaultEventReceiver * eventReciever)
{
	irr::scene::ISceneManager * sm = dev->getSceneManager();

	irr::scene::ICameraSceneNode * camera;

	camera = sm->addCameraSceneNode(
		(irr::scene::ISceneNode*)0,
		irr::core::vector3df(0, 10, 0),
		irr::core::vector3df(0, 0, 100),
		-1,
		false
		);
	camera->setNearValue(0.1f);
	camera->setFarValue(200.0f);

	sm->setActiveCamera(camera);

	eventReciever->SetCamera(camera);

	irr::scene::IMesh * mesh = sm->getMesh("mainScreen.obj");

	if (mesh == NULL)
		return -1;

	irr::scene::IMeshSceneNode * ground = sm->addMeshSceneNode(mesh);

	ground->setMaterialFlag(irr::video::EMF_LIGHTING, true);

	//Setup the skydome
	dome = new IrrBIOSkyDome(sm->getRootSceneNode(), sm, SKYDOME_ID);
	//dome->setPosition(irr::core::vector3df(0,0,100));
	//dome->drop();//I don't need it anymore
	//dome = NULL;

	//set up gui
	irr::gui::IGUIEnvironment * gui = dev->getGUIEnvironment();

	// disable alpha

	for (irr::s32 i = 0; i<irr::gui::EGDC_COUNT; ++i)
	{
		irr::video::SColor col = gui->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		gui->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);
	}
	
	irr::gui::IGUIWindow * main = gui->addWindow(irr::core::recti(5, 350, 300, 575), false, L"Sky Controls", NULL, MAIN_WINDOW);
	main->getCloseButton()->setVisible(false);

	month = gui->addComboBox(irr::core::recti(5, 20, 75, 40), main,MONTH_CBO_BOX);
	month->addItem(L"January", 1);
	month->addItem(L"February", 2);
	month->addItem(L"March", 3);
	month->addItem(L"April", 4);
	month->addItem(L"May", 5);
	month->addItem(L"June", 6);
	month->addItem(L"July", 7);
	month->addItem(L"August", 8);
	month->addItem(L"September", 9);
	month->addItem(L"October", 10);
	month->addItem(L"November", 11);
	month->addItem(L"December", 12);
	month->setSelected(0);

	day = gui->addComboBox(irr::core::recti(80, 20, 120, 40), main,DAY_CBO_BOX);
	day->addItem(L"1", 1);
	day->addItem(L"2", 2);
	day->addItem(L"3", 3);
	day->addItem(L"4", 4);
	day->addItem(L"5", 5);
	day->addItem(L"6", 6);
	day->addItem(L"7", 7);
	day->addItem(L"8", 8);
	day->addItem(L"9", 9);
	day->addItem(L"10", 10);
	day->addItem(L"11", 11);
	day->addItem(L"12", 12);
	day->addItem(L"13", 13);
	day->addItem(L"14", 14);
	day->addItem(L"15", 15);
	day->addItem(L"16", 16);
	day->addItem(L"17", 17);
	day->addItem(L"18", 18);
	day->addItem(L"19", 19);
	day->addItem(L"20", 20);
	day->addItem(L"21", 21);
	day->addItem(L"22", 22);
	day->addItem(L"23", 23);
	day->addItem(L"24", 24);
	day->addItem(L"25", 25);
	day->addItem(L"26", 26);
	day->addItem(L"27", 27);
	day->addItem(L"28", 28);
	day->addItem(L"29", 29);
	day->addItem(L"30", 30);
	day->addItem(L"31", 31);
	day->setSelected(0);

	year = gui->addEditBox(L"2003", irr::core::recti(125, 20, 190, 40), true, main,YEAR_EDIT);

	hour = gui->addEditBox(L"4", irr::core::recti(195, 20, 215, 40), true, main,HOUR_EDIT);
	gui->addStaticText(L":", irr::core::recti(220, 25, 225, 40), false, false, main);
	min = gui->addEditBox(L"30", irr::core::recti(230, 20, 250, 40), true, main, MIN_EDIT);
	gui->addStaticText(L":", irr::core::recti(255, 25, 260, 40), false, false, main);
	sec = gui->addEditBox(L"00", irr::core::recti(265, 20, 285, 40), true, main, SEC_EDIT);

	gui->addStaticText(L"Latitude: ", irr::core::recti(5, 55, 50, 70), false, false, main);
	lat = gui->addEditBox(L"41", irr::core::recti(45, 50, 75, 70), true, main, LAT_EDIT);
	south = gui->addCheckBox(false, irr::core::recti(80, 50, 135, 70), main, SOUTH_CHECK, L"South");

	gui->addStaticText(L"Longitude: ", irr::core::recti(140, 55, 185, 70), false, false, main);
	longitude = gui->addEditBox(L"112", irr::core::recti(180, 50, 210, 70), true, main, LONGITUDE_EDIT);
	west = gui->addCheckBox(true, irr::core::recti(215, 50, 270, 70), main, WEST_CHECK, L"West");

	startTime = gui->addButton(irr::core::recti(5, 75, 65, 95), main, START_TIME_BUTTON, L"Start Time");
	stopTime = gui->addButton(irr::core::recti(70, 75, 130, 95), main, STOP_TIME_BUTTON, L"Stop Time");

	gui->addStaticText(L"Sim Speed:", irr::core::recti(135, 75, 170, 95), false, true, main);
	simSpeed = gui->addScrollBar(true, irr::core::recti(165, 75, 285, 95), main, SIM_SPEED_SCROLL);
	simSpeed->setMin(10);
	simSpeed->setMax(10000);
	simSpeedUnits = gui->addStaticText(L"1 Sec =             seconds", irr::core::recti(135, 105, 220, 130), false, false, main);
	simSpeedText = gui->addStaticText(L"1.0", irr::core::recti(160, 105, 185, 130), false, false, main, SIM_SPEED_TEXT);

	secToMin = gui->addCheckBox(false, irr::core::recti(225, 100, 280, 120), main, SEC_TO_HOURS_CHECK, L"Minutes");

	int offset = 25;

	gui->addStaticText(L"Sky Type: ", irr::core::recti(5, 100 + offset, 100, 120 + offset), false, false, main);

	staticSky = gui->addCheckBox(true, irr::core::recti(5, 125 + offset, 100, 145 + offset), main, STATIC_SKY_CHECK, L"Static Sky");
	calcSky = gui->addCheckBox(false, irr::core::recti(105, 125 + offset, 200, 145 + offset), main, CALC_SKY_CHECK, L"Calculated Sky");
	dynamicSky = gui->addCheckBox(false, irr::core::recti(205, 125 + offset, 300, 145 + offset), main, DYNAMIC_SKY_CHECK, L"Dynamic Sky");

	gui->addStaticText(L"Azimuth: ", irr::core::recti(5, 150 + offset, 50, 170 + offset), false, false, main);
	azimuth = gui->addEditBox(L"45", irr::core::recti(55, 150 + offset, 105, 170 + offset), true, main, AZIMUTH_EDIT);
	gui->addStaticText(L"Moon Azimuth: ", irr::core::recti(115, 150 + offset, 170, 170 + offset), false, false, main);
	moonAzimuth = gui->addEditBox(L"100", irr::core::recti(180, 150 + offset, 230, 170 + offset), true, main, MOON_AZIMUTH_EDIT);
	gui->addStaticText(L"Zenith: ", irr::core::recti(5, 175 + offset, 50, 195 + offset), false, false, main);
	zenith = gui->addEditBox(L"45", irr::core::recti(55, 175 + offset, 105, 195 + offset), true, main, ZENITH_EDIT);
	gui->addStaticText(L"Moon Zenith: ", irr::core::recti(115, 175 + offset, 170, 195 + offset), false, false, main);
	moonZenith = gui->addEditBox(L"100", irr::core::recti(180, 175 + offset, 230, 195 + offset), true, main, MOON_ZENITH_EDIT);
	
	
	//-----------------------------
	setStaticSky();
	enableStaticSky();

	return 0;
}

int MainLoop(irr::IrrlichtDevice * dev, DefaultEventReceiver * eventReciever)
{
	irr::video::IVideoDriver * video = dev->getVideoDriver();
	irr::scene::ISceneManager * sm = dev->getSceneManager();
	irr::gui::IGUIEnvironment * env = dev->getGUIEnvironment();
	irr::ITimer * timer = dev->getTimer();

	//timing stuff
	unsigned int lastTime;
	float deltaTime = 0.0f;

	lastTime = timer->getTime();

	while (dev->run())
	{
		//update timer
		//update deltatime
		deltaTime = (float)(timer->getTime() - lastTime);
		lastTime = timer->getTime();//set the new last time

		//convert delta time to seconds
		deltaTime = (float)deltaTime / 1000.0f;
		//-------------

		//move the camera
		eventReciever->Update(deltaTime);
		//update the sky
		if (sky)
			sky->Update();

		//update time
		if (runTime)
			updateTime(deltaTime);

		//loop stuff
		video->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));

		sm->drawAll();

		env->drawAll();

		video->endScene();
	}

	return 0;
}

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		//execute tests
		BIO::SKY::BIOSkyTests();
		return 0;
	}

	irr::IrrlichtDevice * _dev = NULL;
	

	 _dev = irr::createDevice(
		irr::video::EDT_DIRECT3D9,
		irr::core::dimension2d<irr::u32>(800,600),
		32,
		false,
		false,
		true,
		NULL);

	//Did the device create correctly.
	if (!_dev)
	{
		std::cout << "Rendering Device Failed To Init." << std::endl;
		return -1;
	}

	DefaultEventReceiver* eventReciever = new DefaultEventReceiver(_dev->getCursorControl());
	_dev->setEventReceiver(eventReciever);

	_dev->setWindowCaption(WINDOW_CAPTION);


	//main loop
	int returnErrorValue = 0;

	returnErrorValue = Init(_dev, eventReciever);

	if (returnErrorValue == 0)
	{
		//it passed... run main loop
		returnErrorValue = MainLoop(_dev, eventReciever);
	}
	else
	{
		std::cout << "Error Initializing Simulation.\nError: " << returnErrorValue << std::endl;
	}

	//close this crap
	std::cout << "Shutting down Irrlicht Example: Goodbye" << std::endl;

	//close everything
	deleteSky();

	//remove all scene nodes
	dome->remove();
	_dev->getSceneManager()->getRootSceneNode()->removeAll();

	delete eventReciever;
	eventReciever = NULL;

	_dev->drop();
	_dev = NULL;

	return returnErrorValue;
}