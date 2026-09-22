#pragma once
#ifndef _PCAPPSOI_H_
#define _PCAPPSOI_H_

class AppSceneObjectImpl : public AppSceneObject
{
	friend class AppScene;

	uint32_t m_flags = 0;
	enum
	{
		// this flag will be used in app
		// it will be used to know that this object was added on scene only once
		flag_addedToScene = 0x1,
	};

protected:
	//PluginInterface* m_pi = 0;
	//char32_t m_name[100];
	AppPluginClassID m_classID; // object type

	alUnicodeString m_name;
	std::wstring m_nameW;

	AppSceneObjectImpl* m_parent = 0;
	AppList<AppSceneObjectImpl*> m_children;

	AppAabb m_aabb;
	AppAabb m_aabbTransformed;

	AppVec4 m_localPosition;
	AppVec4 m_globalPosition;

	// матрица хранит только ориентацию, без позиции.
	AppMat4 m_rotationScaleMatrix;

	AppMat4 m_worldViewProjection;
	AppMat4 m_worldMatrix;

	AppColor m_edgeColor;

	AppPluginObject* m_pluginObject = 0;

	AppSceneObjectType m_sceneObjectType = AppSceneObjectType::Polygonal;
	bool m_isSelected = false;
public:
	AppSceneObjectImpl(AppPluginObject* po);
	virtual ~AppSceneObjectImpl();

	virtual void SetParent(AppSceneObject* parent) override;
	virtual void GetChildren(AppList<AppSceneObject*>&) override;
	virtual void ClearChildrenList()  override;

	virtual bool IsSelected()  override;

	// m_aabb хранит оригинальное значение. Он не изменяет своего значения если
	// изменили масштаб объекта или покрутили его.
	// Для того, чтобы получить Aabb в соответствии с масштабов и вращением
	// нужно получить m_aabbTransformed. Это метод GetAABBTransformed().
	virtual AppAabb* GetAABB() override;
	virtual AppAabb* GetAABBTransformed() override;

	virtual AppVec4* GetLocalPosition() override;
	virtual AppVec4* GetGlobalPosition() override;
	virtual AppMat4* GetRotationScaleMatrix() override;
	virtual AppMat4* GetWVPMatrix() override;
	virtual AppMat4* GetWMatrix() override;

	virtual void UpdateAabb() override;

	virtual AppColor* GetEdgeColor() override;
	virtual void SetEdgeColor(const AppColor& c) override;

	virtual AppSceneObject* GetParent() override;
	virtual const char32_t* GetName() override;
	virtual const wchar_t* GetNameW() override;
	virtual void SetName(const char32_t* s) override;

	virtual const AppPluginClassID& GetClassID() override;
	virtual AppPluginObject* GetPluginObject() override;

	virtual AppSceneObjectType GetSceneObjectType() override;

	virtual void Draw(AppViewportData*, AppPluginInterface*) = 0;

	// This must be called inside plugin.
	// Return true if need to select this object.
	// User can select object by clicking on it, or using rectangle,
	// when you click LMB and drag mouse.
	virtual bool OnSelect(AppSelectionFrust*, AppRay*, bool selectByRectangle, AppEditMode) = 0;
	virtual bool IsCanSelect(AppSelectionFrust*, AppRay*) = 0;
};
#endif

