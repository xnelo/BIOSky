/**
* @file IrrBIOSkyDome.hpp
* @author Spencer Hoffa
*
* @copyright 2015 Spencer Hoffa
*
* Creates a Custom SceneNode for Irrlicht engine that will render the skydome 
* and all elements related to the sky.
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

#ifndef ___BIOSKY_IRRBIOSKYDOME_HPP__2015___
#define ___BIOSKY_IRRBIOSKYDOME_HPP__2015___

#include "irrlicht-1.8.1\include\irrlicht.h"
#include "BIOSky.hpp"

class IrrBIOSkyDomeVertecies : public BIO::SKY::IDomeVertecies
{
private:
	/**The variable that actually holds the vertecies.*/
	irr::video::S3DVertex * _vertecies;
	/**The number of vertecies stored.*/
	int _vertexCount;
public:
	/**
	* Constructor
	*
	* @param verts A pointer to the vertex data that will be stored in this
	*			class.
	*
	* @param vertCount The number of vertecies being stored in this class.
	*/
	IrrBIOSkyDomeVertecies(irr::video::S3DVertex * verts, int vertCount);

	/**
	* Destructor
	*/
	virtual ~IrrBIOSkyDomeVertecies();

	/**
	* This returns the number of vertecies that are stored.
	*
	* @return Returns an int with the number of vertecies.
	*/
	virtual int GetVertexCount();

	/**
	* Get the position of the vertex referenced by the index. The
	* passed in index number will always be between 0 and
	* GetVertexCount() - 1.
	*
	* @param index The index of the vertex we are requesting position
	*			data for. Will always be between 0 and
	*			GetVertexCount() - 1.
	*
	* @return A Vector3D with the point position contained in it.
	*/
	virtual BIO::Vector3D GetVertexPosition(int index);

	/**
	* Set the Color for the vertex specified by the index. The passed
	* in index will always be between 0 and GetVertexCounter() -1.
	*
	* @param index The index of the vertex we are requesting position
	*			data for. Will always be between 0 and
	*			GetVertexCount() - 1.
	*
	* @param A The Alpha value for the new color. This is within the
	*			range of [0,255].
	*
	* @param R The Red value for the new color. This is within the
	*			range of [0,255].
	*
	* @param G The Green value for the new color. This is within the
	*			range of [0,255].
	*
	* @param B The Blue value for the new color. This is within the
	*			range of [0,255].
	*/
	virtual void SetVertexColor(int index, int A, int R, int G, int B);
};

inline IrrBIOSkyDomeVertecies::IrrBIOSkyDomeVertecies(irr::video::S3DVertex * verts, int vertCount) : _vertecies(verts), _vertexCount(vertCount)
{}

inline IrrBIOSkyDomeVertecies::~IrrBIOSkyDomeVertecies()
{
	_vertecies = NULL;
}

inline int IrrBIOSkyDomeVertecies::GetVertexCount()
{
	return _vertexCount;
}

inline BIO::Vector3D IrrBIOSkyDomeVertecies::GetVertexPosition(int index)
{
	return BIO::Vector3D(
		_vertecies[index].Pos.X,
		_vertecies[index].Pos.Y,
		_vertecies[index].Pos.Z);
}

inline void IrrBIOSkyDomeVertecies::SetVertexColor(int index, int A, int R, int G, int B)
{
	_vertecies[index].Color.set(A, R, G, B);
}

class IrrBIOSkyDome : public irr::scene::ISceneNode, public BIO::SKY::IDomeGeometry
{
protected:
	/**The bounding box*/
	irr::core::aabbox3d<irr::f32> _bBox;
	/**Mesh buffer that holds the geometry*/
	irr::scene::IMeshBuffer * _geometry;
	/**The Skydomes radius*/
	float _radius;
	/**The sun. This is a billboard object.*/
	irr::scene::IBillboardSceneNode * _sun;
	/**The texture for the sun geometry*/
	irr::video::ITexture * _sunTexture;
	/**The moon. This is a billboard object.*/
	irr::scene::IBillboardSceneNode * _moon;
	/**The texture for the moon geometry*/
	irr::video::ITexture * _moonTexture;
	/**The material for the dome*/
	irr::video::SMaterial _material;

	/**A pointer to the vertex data needed by the library.*/
	IrrBIOSkyDomeVertecies * _vertData;
public: 
	/**
	* Constructor
	*/
	IrrBIOSkyDome(irr::scene::ISceneNode * parent, irr::scene::ISceneManager * sm, irr::s32 id);

	/**
	* Destructor
	*/
	virtual ~IrrBIOSkyDome();

	/**
	* Override from ISceneNode
	*/
	virtual void OnRegisterSceneNode();

	/**
	* Render the Node.
	*/
	virtual void render();

	virtual const irr::core::aabbox3d<irr::f32> & getBoundingBox() const;

	virtual irr::u32 getMaterialCount() const;

	virtual irr::video::SMaterial& IrrBIOSkyDome::getMaterial(irr::u32 i);

	/**
	* Return a class that inherits from IDomeVertecies. This class
	* should contain, or at least be able to access, the vertex data
	* for this geometry. You are required to implement the
	* IDomeVertecies interface and in the implementation of this
	* function return a pointer to the structure you created. You are
	* also required to manage the memory. The BIOSky library will not
	* delete the returned object when it is done with it, you are
	* required to do that. A perfect place would be in the
	* UnlockGeometry function.
	*
	* @return Returns an object that inherits from IDomeVertecies and
	*			has access to all the domes vertecies.
	*/
	virtual BIO::SKY::IDomeVertecies * GetVertecies();

	/**
	* This function is the request for the BIOSky to take control of
	* the geometry buffers. When this is called the BIOSky library will
	* be editing the Skydome geometry until the UnlockGeometry function
	* is called.
	*
	* @note You may or may not need to do anything or even keep track
	* of when these functions are called. This is for graphic drivers
	* and enginges that need to lock the geometry buffers. Some locks
	* will not happen until you return the vertex data. The BIOSky
	* library will call LockGeometry before it edits skydome geometry
	* and will call UnlockGeometry when it is done.
	*/
	virtual void LockGeometry();

	/**
	* Set Position of the moon.
	*
	* @param unitX The unit x position of the moon. This is the position
	*			on the unit circle(circle with radius of 1). When you
	*			implement this function you should multiply by the
	*			radius of the dome.
	*
	* @param unitY The unit y position of the moon. This is the position
	*			on the unit circle(circle with radius of 1). When you
	*			implement this function you should multiply by the
	*			radius of the dome.
	*
	* @param unitZ The unit z position of the moon. This is the position
	*			on the unit circle(circle with radius of 1). When you
	*			implement this function you should multiply by the
	*			radius of the dome.
	*/
	virtual void SetMoonPosition(float unitX, float unitY, float unitZ);

	/**
	* Set Position of the sun.
	*
	* @param unitX The unit x position of the sun. You should multiply
	*			by the domes radius to find the actual position.
	*
	* @param unitY The unit y position of the sun. You should multiply
	*			by the domes radius to find the actual position.
	*
	* @param unitZ The unit z position of the sun. You should multiply
	*			by the domes radius to find the actual position.
	*/
	virtual void SetSunPosition(float unitX, float unitY, float unitZ);

	/**
	* When this function is called this means that the BIOSky library
	* has finished editing the skydome geometry and has released all
	* pointers to the geometry and buffers.
	*
	* @note You may or may not need to do anything or even keep track
	* of when these functions are called. This is for graphic drivers
	* and enginges that need to lock the geometry buffers. Some locks
	* will not happen until you return the vertex data. The BIOSky
	* library will call LockGeometry before it edits skydome geometry
	* and will call UnlockGeometry when it is done.
	*/
	virtual void UnlockGeometry();
};

#endif //___BIOSKY_IRRBIOSKYDOME_HPP__2015___