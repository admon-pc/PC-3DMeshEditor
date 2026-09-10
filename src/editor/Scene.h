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
		virtual void Destroy() override;
	};

	//AppSceneObject* m_rootObject = 0;
	AppSceneObjectInternal* m_rootObject = 0;

public:
	AppScene();
	~AppScene();
	AL_DECLARE_DEFAULT_ALLOCATOR(AppScene);

	// Delete all objects
	void ClearScene();

	void AddObject(AppSceneObject*);

	void GetFreeName(alUnicodeString*);

};

#endif

