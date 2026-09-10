#pragma once
#ifndef _PCSCENEO_H_
#define _PCSCENEO_H_

class AppSceneObject
{
protected:
	//PluginInterface* m_pi = 0;
	//char32_t m_name[100];
	PluginClassID m_classID; // object type

	PluginString m_name;

	//AppSceneObject* m_parent = 0;
	//alList<AppSceneObject*> m_children;
public:
	AppSceneObject(/*PluginInterface* pi*/) {}// : m_pi(pi) { m_name[0] = 0; }
	virtual ~AppSceneObject() {}

	virtual void Destroy() = 0;

	/*virtual void SetParent(AppSceneObject* parent)
	{
		if (m_parent)
		{
			m_parent->m_children.erase_first(this);
		}

		m_parent = parent;

		if (parent)
			parent->m_children.push_back(this);
	}*/


	virtual const char32_t* GetName() { return m_name.Data(); }
	/*virtual void SetName(const char32_t* name)
	{
		m_pi->snprintf(m_name, 100, U"%s", name);
	}*/

	virtual const PluginClassID& GetClassID() { return m_classID; }
};

// Will be used only in .exe
class AppSceneObjectInternal
{
public:
	AppSceneObjectInternal() {}
	virtual ~AppSceneObjectInternal() {}
};

#endif

