#pragma once
#ifndef _PCSCENE_H_
#define _PCSCENE_H_

#include "SceneObject.h"


class AppScene
{
	class _root_t : public AppSceneObject
	{
	public:
		_root_t();
		virtual ~_root_t();
	};

	AppSceneObject* m_rootObject = 0;
	//AppSceneObjectInternal* m_rootObject = 0;

	PluginArray<AppSceneObject*>* m_getAllObjectArrayPtr = 0;
	void _onGetAllObjects(AppSceneObject*);

public:
	AppScene();
	~AppScene();
	AL_DECLARE_DEFAULT_ALLOCATOR(AppScene);

	void DeleteObject(AppSceneObject*);
	// Delete all objects
	void ClearScene();

	void AddObject(AppSceneObject*);
	void GetAllObjects(PluginArray<AppSceneObject*>*);

	bool IsNameFree(AppSceneObject* , alUnicodeString*);
	void GetFreeName(alUnicodeString*);

};

#endif

