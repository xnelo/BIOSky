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

float DegreesToRadians(float degrees)
{
	return degrees * ((float)BIO::SKY::PI / 180.0f);
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
	MOON_ZENITH_EDIT
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
	sky = new BIO::SKY::SkyStaticManual(dome, _staticSunPos.Azimuth, _staticSunPos.Zenith, _staticMoonPos.Azimuth, _staticMoonPos.Zenith);
}

void enableDynamicSky()
{
	enableTimer(true);
	enableCalculations(true);
	enableStatic(false);
	staticSky->setChecked(false);
	calcSky->setChecked(false);
}

void enableCalculatedSky()
{
	enableTimer(false);
	enableCalculations(true);
	enableStatic(false);
	dynamicSky->setChecked(false);
	staticSky->setChecked(false);
}

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
	}
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
	case irr::gui::EGET_EDITBOX_CHANGED:
		switch (id)
		{
		case YEAR_EDIT:
			if (!checkNumber(year->getText()))
				year->setText(L"2003");
			setDate();
			return true;
		case LAT_EDIT:
			if (!checkFloatNumber(lat->getText()))
				lat->setText(L"41");
			if (wcstof(lat->getText(), NULL) > 90.0f)
				lat->setText(L"90");
			setGPS();
			return true;
		case LONGITUDE_EDIT:
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
		default:
			return false;
		}
	case irr::gui::EGET_COMBO_BOX_CHANGED:
		std::cout << "COmbo Box changed" << std::endl;
		switch (id)
		{
		case MONTH_CBO_BOX:
			setDate();
			return true;
		case DAY_CBO_BOX:
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
			return true;
		case STOP_TIME_BUTTON:
			std::cout << "Stop Timer" << std::endl;
			return true;
		default:
			return false;
		}
	case irr::gui::EGET_CHECKBOX_CHANGED:
		switch (id)
		{
		case STATIC_SKY_CHECK:
			if ((calcSky->isChecked() == false) && (dynamicSky->isChecked() == false))
				staticSky->setChecked(true);
			enableStaticSky();
			return true;
		case CALC_SKY_CHECK:
			if ((staticSky->isChecked() == false) && (dynamicSky->isChecked() == false))
				calcSky->setChecked(true);
			enableCalculatedSky();
			return true;
		case DYNAMIC_SKY_CHECK:
			if ((calcSky->isChecked() == false) && (staticSky->isChecked() == false))
				dynamicSky->setChecked(true);
			enableDynamicSky();
			return true;
		case SOUTH_CHECK:
			setGPS();
			return true;
		case WEST_CHECK:
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

	irr::scene::ILightSceneNode * sun = sm->addLightSceneNode();

	irr::video::SLight & l = sun->getLightData();
	l.Direction = irr::core::vector3df(0, 0, 0);
	l.Type = irr::video::ELT_DIRECTIONAL;
	l.AmbientColor = irr::video::SColorf(0.3f, 0.3f, 0.3f, 1.0f);
	l.SpecularColor = irr::video::SColorf(0.4f, 0.4f, 0.4f, 1);
	l.DiffuseColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
	l.CastShadows = false;

	sun->setPosition(irr::core::vector3df(0, 0, 0));
	sun->setRotation(irr::core::vector3df(65.0f,129.0f,12.0f));

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

	startTime = gui->addButton(irr::core::recti(15, 75, 85, 95), main, START_TIME_BUTTON, L"Start Time");
	stopTime = gui->addButton(irr::core::recti(90, 75, 160, 95), main, STOP_TIME_BUTTON, L"Stop Time");

	gui->addStaticText(L"Sky Type: ", irr::core::recti(5, 100, 100, 120), false, false, main);

	staticSky = gui->addCheckBox(true, irr::core::recti(5, 125, 100, 145), main, STATIC_SKY_CHECK, L"Static Sky");
	calcSky = gui->addCheckBox(false, irr::core::recti(105, 125, 200, 145), main, CALC_SKY_CHECK, L"Calculated Sky");
	dynamicSky = gui->addCheckBox(false, irr::core::recti(205, 125, 300, 145), main, DYNAMIC_SKY_CHECK, L"Dynamic Sky");

	gui->addStaticText(L"Azimuth: ", irr::core::recti(5, 150, 50, 170), false, false, main);
	azimuth = gui->addEditBox(L"45", irr::core::recti(55, 150, 105, 170), true, main, AZIMUTH_EDIT);
	gui->addStaticText(L"Moon Azimuth: ", irr::core::recti(115, 150, 170, 170), false, false, main);
	moonAzimuth = gui->addEditBox(L"100", irr::core::recti(180, 150, 230, 170), true, main, MOON_AZIMUTH_EDIT);
	gui->addStaticText(L"Zenith: ", irr::core::recti(5, 175, 50, 195), false, false, main);
	zenith = gui->addEditBox(L"45", irr::core::recti(55, 175, 105, 195), true, main, ZENITH_EDIT);
	gui->addStaticText(L"Moon Zenith: ", irr::core::recti(115, 175, 170, 195), false, false, main);
	moonZenith = gui->addEditBox(L"100", irr::core::recti(180, 175, 230, 195), true, main, MOON_ZENITH_EDIT);
	//irr::gui::IGUITabControl * tabs = gui->addTabControl(irr::core::recti(5, 20, 100, 100), main);

	//irr::gui::IGUITab * staticTab = tabs->addTab(L"Static");
	//irr::gui::IGUITab * dynamicTab = tabs->addTab(L"Dynamic");
	
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

	delete eventReciever;
	eventReciever = NULL;

	_dev->drop();
	_dev = NULL;

	return returnErrorValue;
}