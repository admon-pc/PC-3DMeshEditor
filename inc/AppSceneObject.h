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

	PluginString m_name;
	std::wstring m_nameW;

	AppSceneObject* m_parent = 0;
	PluginList<AppSceneObject*> m_children;

	AppPluginObject* m_plugin = 0;

	AppSceneObjectType m_sceneObjectType = AppSceneObjectType::Polygonal;
public:
	AppSceneObject(AppPluginObject* po) : m_plugin(po) {}
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
	virtual const PluginList<AppSceneObject*>& GetChildren() const { return m_children; }
	virtual void ClearChildrenList() { m_children.clear(); }

	virtual AppSceneObject* GetParent() { return m_parent; }
	virtual const char32_t* GetName() { return m_name.Data(); }
	virtual const wchar_t* GetNameW() { return m_nameW.c_str(); }
	virtual void SetName(const char32_t* s) { if (s) { m_name.Assign(s); } m_name.ToUTF16(m_nameW); }
	/*virtual void SetName(const char32_t* name)
	{
		m_pi->snprintf(m_name, 100, U"%s", name);
	}*/

	virtual const AppPluginClassID& GetClassID() { return m_classID; }
	virtual AppPluginObject* GetPlugin() { return m_plugin; }

	AppSceneObjectType GetSceneObjectType() { return m_sceneObjectType; }
};

// Will be used only in .exe
//class AppSceneObjectInternal
//{
//public:
//	AppSceneObjectInternal() {}
//	virtual ~AppSceneObjectInternal() {}
//};

#endif

