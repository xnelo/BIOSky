/**
* @file IrrBIOSkyDome.cpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Implementation of the IrrBIOSkyDome class.
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

#include "IrrBIOSkyDome.hpp"

#include <iostream>

IrrBIOSkyDome::IrrBIOSkyDome(irr::scene::ISceneNode * parent, irr::scene::ISceneManager * sm, irr::s32 id) : 
irr::scene::ISceneNode(parent, sm, id),
_bBox(),
_geometry(NULL),
_radius(200),
_sun(NULL),
_sunTexture(NULL),
_moon(NULL),
_moonTexture(NULL),
_material(),
_vertData(NULL),
_nightGeometry(NULL),
_nightMaterial(),
_nightTexture(NULL)
{
	
	_material.Wireframe = false;
	_material.Lighting = false;
	_material.MaterialType = irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL;

	//create the geometry
	BIO::SKY::RawGeometry * dome = BIO::SKY::CreateSkyDomeGeometry(_radius, 12, 12);
	
	irr::scene::SMeshBuffer * buffer = new irr::scene::SMeshBuffer();

	buffer->Indices.clear();
	buffer->Indices.set_used(dome->numIndecies);
	
	for (int i = 0; i < dome->numIndecies; ++i)
	{
		buffer->Indices[i] = dome->indecies[i];
	}

	// Create vertices
	irr::video::SColor clr(255, 255, 255, 255);

	buffer->Vertices.clear();
	buffer->Vertices.reallocate(dome->numVertecies);
	
	for (int i = 0; i < dome->numVertecies; i++)
	{
		buffer->Vertices.push_back(irr::video::S3DVertex(dome->vertecies[i].X, dome->vertecies[i].Y, dome->vertecies[i].Z, -1, -1, -1, clr, dome->UVTextureCoordinates[i].X, dome->UVTextureCoordinates[i].Y));
	}

	// Recalculate bounding box
	buffer->BoundingBox.reset(0, 0, 0);
	
	for (int i = 0; i < dome->numVertecies; ++i)
	{
		buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
	}

	_geometry = buffer;
	buffer = NULL;

	delete dome;
	dome = NULL;

	
	BIO::SKY::RawGeometry * night = BIO::SKY::CreateNightSkyDomeGeometry(_radius);
	
	irr::scene::SMeshBuffer * bufferNight = new irr::scene::SMeshBuffer();
	bufferNight->Indices.clear();
	bufferNight->Indices.set_used(night->numIndecies);
	for (int i = 0; i < night->numIndecies; ++i)
	{
		bufferNight->Indices[i] = night->indecies[i];
	}
	bufferNight->Vertices.clear();
	bufferNight->Vertices.reallocate(night->numVertecies);
	for (int i = 0; i < night->numVertecies; i++)
	{
		bufferNight->Vertices.push_back(irr::video::S3DVertex(night->vertecies[i].X, night->vertecies[i].Y, night->vertecies[i].Z, -1, -1, -1, clr, night->UVTextureCoordinates[i].X, night->UVTextureCoordinates[i].Y));
	}
	bufferNight->BoundingBox.reset(0, 0, 0);
	for (int i = 0; i < night->numVertecies; ++i)
	{
		bufferNight->BoundingBox.addInternalPoint(bufferNight->Vertices[i].Pos);
	}
	_nightGeometry = bufferNight;
	delete night;

	//Create the sun
	const float _domeRadiusToSunBillboardRatio = 0.15f;
	_sun = sm->addBillboardSceneNode(this, irr::core::dimension2d<irr::f32>(_radius * _domeRadiusToSunBillboardRatio, _radius *_domeRadiusToSunBillboardRatio));
	SetSunPosition(0, 1, 0);

	int sizeOfTexture = 128;//50;
	_sunTexture = sm->getVideoDriver()->addTexture(irr::core::dimension2du(sizeOfTexture, sizeOfTexture), "_internal/Sky/SunTexture");
	unsigned char * tmptex = BIO::SKY::CreateSunTexture(sizeOfTexture);
	unsigned char * locked = (unsigned char *)_sunTexture->lock();
	for (int i = 0; i < sizeOfTexture * sizeOfTexture * 4; i++)
	{
		locked[i] = tmptex[i];
	}
	_sunTexture->unlock();
	delete[] tmptex;

	_sun->setMaterialTexture(0, _sunTexture);
	_sun->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	_sun->setMaterialFlag(irr::video::EMF_USE_MIP_MAPS, false);
	_sun->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	//-----------------------------------------------------

	//Create the Moon -------------------------------------
	const float _domeRadiusToMoonBillboardRatio = 0.15f;
	_moon = sm->addBillboardSceneNode(this, irr::core::dimension2d<irr::f32>(_radius * _domeRadiusToMoonBillboardRatio, _radius *_domeRadiusToMoonBillboardRatio));
	SetMoonPosition(0, 1, 0);

	int moonTextureWidth = 0;
	int moonTextureHeight = 0;

	tmptex = BIO::SKY::CreateMoonTexture(&moonTextureWidth, &moonTextureHeight);

	_moonTexture = sm->getVideoDriver()->addTexture(irr::core::dimension2du(moonTextureWidth, moonTextureHeight), "_internal/Sky/MoonTexture");

	locked = (unsigned char *)_moonTexture->lock();
	for (int i = 0; i < moonTextureWidth * moonTextureHeight * 4; i++)
	{
		locked[i] = tmptex[i];
	}
	_moonTexture->unlock();
	delete[] tmptex;

	_moon->setMaterialTexture(0, _moonTexture);
	_moon->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	_moon->setMaterialFlag(irr::video::EMF_USE_MIP_MAPS, false);
	_moon->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	//-----------------------------------------------------

	//Setup Night material --------------------------------
	int nightTextureWidth = 0;
	int nightTextureHeight = 0;

	tmptex = BIO::SKY::CreateNightSkyTexture(&nightTextureWidth, &nightTextureHeight);

	_nightTexture = sm->getVideoDriver()->addTexture(irr::core::dimension2du(nightTextureWidth, nightTextureHeight), "_internal/Sky/NightSkyTexture");

	

	locked = (unsigned char *)_nightTexture->lock();
	/*for (int i = 0; i < nightTextureWidth * nightTextureHeight * 4; i++)
	{
		locked[i] = tmptex[i];
	}*/
	memcpy((void*)locked, (void*)tmptex, sizeof(unsigned char) * nightTextureWidth * nightTextureHeight * 4);
	_nightTexture->unlock();
	delete[] tmptex;

	if (_nightTexture == NULL)
	{
		std::cout << "ERROR GETTING NIGHT SKY TEXTURE" << std::endl;
	}

	_nightMaterial.setTexture(0, _nightTexture);
	_nightMaterial.Wireframe = false;
	_nightMaterial.Lighting = false;
	_nightMaterial.UseMipMaps = false;
	_nightMaterial.MaterialType = irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL;
	//-----------------------------------------------------
}

IrrBIOSkyDome::~IrrBIOSkyDome()
{
	if (_vertData)
	{
		delete _vertData;
		_vertData = NULL;
	}

	if (_sun)
	{
		_sun->removeAll();
		_sun = NULL;
	}

	if (_sunTexture)
	{
		SceneManager->getVideoDriver()->removeTexture(_sunTexture);
		_sunTexture = NULL;
	}

	if (_moon)
	{
		_moon->removeAll();
		_moon = NULL;
	}

	if (_moonTexture)
	{
		SceneManager->getVideoDriver()->removeTexture(_moonTexture);
		_moonTexture = NULL;
	}

	if (_geometry)
	{
		_geometry->drop();
		_geometry = NULL;
	}

	if (_nightTexture)
	{
		SceneManager->getVideoDriver()->removeTexture(_nightTexture);
		_nightTexture = NULL;
	}

	if (_nightGeometry)
	{
		_nightGeometry->drop();
		_nightGeometry = NULL;
	}
}

void IrrBIOSkyDome::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void IrrBIOSkyDome::render()
{
	irr::video::IVideoDriver * driver = SceneManager->getVideoDriver();

	//set the transform
	driver->setTransform(irr::video::ETS_WORLD, _starTransformations);

	if (_nightGeometry)
	{
		//draw night
		driver->setMaterial(_nightMaterial);
		driver->drawMeshBuffer(_nightGeometry);
	}

	driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
	if (_geometry)
	{
		//draw day
		driver->setMaterial(_material);
		driver->drawMeshBuffer(_geometry);
	}
	
	//render the sun
	_sun->render();

	//render the moon
	_moon->render();
}

const irr::core::aabbox3d<irr::f32> & IrrBIOSkyDome::getBoundingBox() const
{
	return _geometry->getBoundingBox();
}

irr::u32 IrrBIOSkyDome::getMaterialCount() const
{
	return 1;
}

irr::video::SMaterial& IrrBIOSkyDome::getMaterial(irr::u32 i)
{
	return _material;
}

BIO::SKY::IDomeVertecies * IrrBIOSkyDome::GetVertecies()
{
	//this is allocated on the heap. It will then be
	//deleted in the UnlockGeometry function.
	_vertData = new IrrBIOSkyDomeVertecies(
		(irr::video::S3DVertex *)_geometry->getVertices(), 
		_geometry->getVertexCount());

	return _vertData;
}

void IrrBIOSkyDome::LockGeometry()
{
	//In this implementation we do not need to do anything.
}

void IrrBIOSkyDome::SetMoonPosition(float unitX, float unitY, float unitZ)
{
	_moon->setPosition(
		irr::core::vector3df(
		unitX * _radius,
		unitY * _radius,
		unitZ * _radius
		)
		);
}

void IrrBIOSkyDome::SetStarRotation(float x, float y, float z)
{
	//_starTransformations.setRotationRadians(irr::core::vector3df(x, y, z));
	irr::core::matrix4 tmp;
	tmp.setRotationRadians(irr::core::vector3df(0, y, 0));
	_starTransformations.setRotationRadians(irr::core::vector3df(x, 0, 0));
	_starTransformations *= tmp;
}

void IrrBIOSkyDome::SetSunPosition(float unitX, float unitY, float unitZ)
{
	_sun->setPosition(
		irr::core::vector3df(
		unitX * _radius,
		unitY * _radius,
		unitZ * _radius
		)
		);
}

void IrrBIOSkyDome::UnlockGeometry()
{
	if (_vertData)
	{
		delete _vertData;
		_vertData = NULL;
	}
}





