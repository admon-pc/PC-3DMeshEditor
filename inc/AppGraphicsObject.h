#pragma once
#ifndef _PCGO_H_
#define _PCGO_H_

class AppSceneObject;



struct AppViewportData;
// GPU mesh
class AppGraphicsObject
{
public:
	AppGraphicsObject() {}
	virtual ~AppGraphicsObject() {}

	virtual void Draw(AppViewportData*, AppSceneObject*) = 0;
	//AppAabb m_aabb;
};

#endif

