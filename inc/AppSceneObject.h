#pragma once
#ifndef _PCSCENEO_H_
#define _PCSCENEO_H_

enum class AppSceneObjectType
{
	Polygonal
};

class AppSelectionFrust;
class AppPluginObject;
class AppSceneObject : public AppBaseObject
{
public:
	virtual void SetParent(AppSceneObject* parent) = 0;
	virtual void GetChildren(AppList<AppSceneObject*>&) = 0;
	virtual void ClearChildrenList() = 0;	
	virtual bool IsSelected() = 0;

	// m_aabb хранит оригинальное значение. Он не изменяет своего значения если
	// изменили масштаб объекта или покрутили его.
	// Для того, чтобы получить Aabb в соответствии с масштабов и вращением
	// нужно получить m_aabbTransformed. Это метод GetAABBTransformed().
	virtual AppAabb* GetAABB() = 0;
	virtual AppAabb* GetAABBTransformed() = 0;

	virtual AppVec4* GetLocalPosition() = 0;
	virtual AppVec4* GetGlobalPosition() = 0;
	virtual AppMat4* GetRotationScaleMatrix() = 0;
	virtual AppMat4* GetWVPMatrix() = 0;
	virtual AppMat4* GetWMatrix() = 0;

	virtual void UpdateAabb() = 0;
	
	virtual AppColor* GetEdgeColor() = 0;
	virtual void SetEdgeColor(const AppColor& c) = 0;

	virtual AppSceneObject* GetParent() = 0;
	virtual const char32_t* GetName() = 0;
	virtual const wchar_t* GetNameW() = 0;
	virtual void SetName(const char32_t* s) = 0;
	virtual const AppPluginClassID& GetClassID() = 0;
	virtual AppPluginObject* GetPluginObject() = 0;

	virtual AppSceneObjectType GetSceneObjectType() = 0;

	virtual void Draw(AppViewportData*, AppPluginInterface*) = 0;

	// This must be called inside plugin.
	// Return true if need to select this object.
	// User can select object by clicking on it, or using rectangle,
	// when you click LMB and drag mouse.
	virtual bool OnSelect(AppSelectionFrust*, AppRay*, bool selectByRectangle, AppEditMode) = 0;
	virtual bool IsCanSelect(AppSelectionFrust*, AppRay*) = 0;
};

// Will be used only in .exe
//class AppSceneObjectInternal
//{
//public:
//	AppSceneObjectInternal() {}
//	virtual ~AppSceneObjectInternal() {}
//};

#endif

