#pragma once
#ifndef _PCSCENE_H_
#define _PCSCENE_H_

#include "AppSceneObject.h"


class AppScene
{
	class _root_t : public AppSceneObject
	{
	public:
		_root_t();
		virtual ~_root_t();
		virtual void Draw(AppViewportData*, AppPluginInterface*) override {}
	};

	AppSceneObject* m_rootObject = 0;
	//AppSceneObjectInternal* m_rootObject = 0;

	AppArray<AppSceneObject*>* m_getAllObjectArrayPtr = 0;
	AppArray<AppSceneObject*> m_allObjectsOnScene;
	void _onGetAllObjectsIntoArray();
	void _onGetAllObjects(AppSceneObject*);

public:
	AppScene();
	~AppScene();
	AL_DECLARE_DEFAULT_ALLOCATOR(AppScene);

	void DeleteObject(AppSceneObject*);
	// Delete all objects
	void ClearScene();

	void AddObject(AppSceneObject*);
	void GetAllObjects(AppArray<AppSceneObject*>*);

	bool IsNameFree(AppSceneObject* , alUnicodeString*);
	void GetFreeName(alUnicodeString*);

	//void Update(float32_t dt);
	//void Draw(float32_t dt);

	AppSceneObject* GetRootObject() { return m_rootObject; }
};

#endif

