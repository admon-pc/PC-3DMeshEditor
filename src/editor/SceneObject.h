#pragma once
#ifndef _PCSCENEO_H_
#define _PCSCENEO_H_
class AppSceneObject
{
protected:
//	alAabb m_aabb;
//	alUnicodeString m_name;
	const char32_t* m_name = 0;
	PluginClassID m_classID; // object type
public:
	AppSceneObject() {}
	virtual ~AppSceneObject() {}

	virtual const char32_t* GetName() { return m_name; }
//	virtual const alAabb& GetAABB() { return m_aabb; }
	virtual const PluginClassID& GetClassID() { return m_classID; }
};

#endif

