#pragma once
#ifndef _PCSCENEO_H_
#define _PCSCENEO_H_

enum class AppSceneObjectType
{
	Polygonal
};

class AppPluginObject;
class AppSceneObject
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

	AppString m_name;
	std::wstring m_nameW;

	AppSceneObject* m_parent = 0;
	AppList<AppSceneObject*> m_children;
	
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
	AppSceneObject(AppPluginObject* po) : m_pluginObject(po) {}
	virtual ~AppSceneObject() {}

	virtual void SetParent(AppSceneObject* parent)
	{
		if (m_parent)
		{
			m_parent->m_children.erase_first(this);
		}

		m_parent = parent;

		if (parent)
			parent->m_children.push_back(this);
	}
	virtual const AppList<AppSceneObject*>& GetChildren() const { return m_children; }
	virtual void ClearChildrenList() { m_children.clear(); }
	
	virtual bool IsSelected() { return m_isSelected; }

	// m_aabb хранит оригинальное значение. Он не изменяет своего значения если
	// изменили масштаб объекта или покрутили его.
	// Для того, чтобы получить Aabb в соответствии с масштабов и вращением
	// нужно получить m_aabbTransformed. Это метод GetAABBTransformed().
	virtual AppAabb* GetAABB() { return &m_aabb; }
	virtual AppAabb* GetAABBTransformed() { return &m_aabbTransformed; }

	virtual AppVec4* GetLocalPosition() { return &m_localPosition; }
	virtual AppVec4* GetGlobalPosition() { return &m_globalPosition; }
	virtual AppMat4* GetRotationScaleMatrix() { return &m_rotationScaleMatrix; }
	virtual AppMat4* GetWVPMatrix() { return &m_worldViewProjection; }
	virtual AppMat4* GetWMatrix() { return &m_worldMatrix; }

	virtual void UpdateAabb() 
	{
		// В старой версии делал вот это.
		// Думаю - нужно ли?
		// m_aabb и есть тот AABB который устанавливался при создании объекта.
		// имея базовое значение, делаем его модифицированную версию - m_aabbTransformed
		// и дальше, используем m_aabbTransformed.
		// Возможно, может потребоваться изменять m_aabb. В каких случаях? В моменте редактирования?
		//  Тогда, просто нужно будет брать, и обновлять m_aabb вручную там где происходит редактирование.
		// Здесь же, происходит обновление m_aabbTransformed в соответствии с матрицей 
		// хранящей вращение и масштаб. Вся эта штука работает для корректного frustum cull.
		// Так-же вероятно как я помню, выбор объекта мышкой так-же происходит с учётом
		//  попал ли луч в m_aabbTransformed.
		// Ну и получается что надо вызывать этот метод после вращения или масштабирования объекта.
		//
		/*m_aabb.reset();
		for (int i = 0, sz = GetVisualObjectCount(); i < sz; ++i)
		{
			m_aabb.add(GetVisualObject(i)->GetAabb());
		}*/

		//m_aabb.m_min += m_globalPosition;//no
		//m_aabb.m_max += m_globalPosition;//no
		// m_aabb must be in space center

		AppMat4 m = m_rotationScaleMatrix;

		m_aabbTransformed = m_aabb;
		m_aabbTransformed.Transform(&m_aabb, &m, &m_globalPosition);
	}
	
	virtual AppColor* GetEdgeColor() { return &m_edgeColor; }
	virtual void SetEdgeColor(const AppColor& c) { m_edgeColor = c; }

	virtual AppSceneObject* GetParent() { return m_parent; }
	virtual const char32_t* GetName() { return m_name.Data(); }
	virtual const wchar_t* GetNameW() { return m_nameW.c_str(); }
	virtual void SetName(const char32_t* s) { if (s) { m_name.Assign(s); } m_name.ToUTF16(m_nameW); }
	/*virtual void SetName(const char32_t* name)
	{
		m_pi->snprintf(m_name, 100, U"%s", name);
	}*/

	virtual const AppPluginClassID& GetClassID() { return m_classID; }
	virtual AppPluginObject* GetPluginObject() { return m_pluginObject; }

	AppSceneObjectType GetSceneObjectType() { return m_sceneObjectType; }

	virtual void Draw(AppViewportData*, AppPluginInterface*) = 0;

	// This must be called inside plugin.
	// Return true if need to select this object.
	// User can select object by clicking on it, or using rectangle,
	// when you click LMB and drag mouse.
	virtual bool OnSelect(AppSelectionFrust&, AppRay&, bool selectByRectangle, AppEditMode) = 0;
	virtual bool IsCanSelect(AppSelectionFrust&, AppRay&) = 0;
};

// Will be used only in .exe
//class AppSceneObjectInternal
//{
//public:
//	AppSceneObjectInternal() {}
//	virtual ~AppSceneObjectInternal() {}
//};

#endif

