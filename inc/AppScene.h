#pragma once
#ifndef _PCSCENE_H_
#define _PCSCENE_H_

#include "AppSceneObject.h"

class AppScene : public AppBaseObject
{
public:
	virtual void DeleteObject(AppSceneObject*) = 0;
	// Delete all objects
	virtual void ClearScene() = 0;

	virtual void AddObject(AppSceneObject*) = 0;
	virtual void GetAllObjects(AppArray<AppSceneObject*>*) = 0;

	virtual bool IsNameFree(AppSceneObject* , alUnicodeString*) = 0;
	virtual void GetFreeName(alUnicodeString*) = 0;

	virtual AppSceneObject* GetRootObject() = 0;
	virtual void DeselectAll() = 0;
	virtual void SelectObject(AppSceneObject*) = 0;
};

#endif

