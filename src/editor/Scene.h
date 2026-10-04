#pragma once
#ifndef _PCAPPSCENEIMPL_H_
#define _PCAPPSCENEIMPL_H_

class AppSceneImpl : public AppScene
{
	friend class Application;
	friend class AppViewport;
	friend class AppGizmo;

	class _root_t : public AppSceneObject
	{
	public:
		_root_t();
		virtual ~_root_t();
		virtual void Draw(AppViewportData*, AppPluginInterface*) override {}
		virtual bool OnSelect(AppSelectionFrust*, AppRay*, bool selectByRectangle, AppEditMode) override { return false; }
		virtual bool IsCanSelect(AppSelectionFrust*, AppRay*) override { return false; }
	};

	AppSceneObject* m_rootObject = 0;
	//AppSceneObjectInternal* m_rootObject = 0;

	AppArray<AppSceneObject*>* m_getAllObjectArrayPtr = 0;
	AppArray<AppSceneObject*> m_allObjectsOnScene;
	AppArray<AppSceneObject*> m_selectedObjects;
	void _onGetAllObjectsIntoArray();
	void _onGetAllObjects(AppSceneObject*);
	void _updateOnSelectObject();

public:
	AppSceneImpl();
	virtual ~AppSceneImpl();

	virtual void DeleteObject(AppSceneObject*) override;
	// Delete all objects
	virtual void ClearScene() override;

	virtual void AddObject(AppSceneObject*) override;
	virtual void GetAllObjects(AppArray<AppSceneObject*>*) override;

	virtual bool IsNameFree(AppSceneObject*, alUnicodeString*) override;
	virtual void GetFreeName(alUnicodeString*) override;

	//void Update(float32_t dt);
	//void Draw(float32_t dt);

	virtual AppSceneObject* GetRootObject() override;
	virtual void DeselectAll() override;
	virtual void SelectObject(AppSceneObject*) override;
};

#endif

