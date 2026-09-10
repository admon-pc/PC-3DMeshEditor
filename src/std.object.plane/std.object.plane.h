#pragma once
#ifndef _PLUGINSTDo_H_
#define _PLUGINSTDo_H_

class AppSceneObject_plane : public AppSceneObject
{
public:
	AppSceneObject_plane(/*PluginInterface* pi*/);
	virtual ~AppSceneObject_plane();
	virtual void Destroy() override;
};

class PluginObject_plane : public PluginObject
{
public:
	PluginObject_plane(PluginInterface* i);
	virtual ~PluginObject_plane();

	/// Plugin
	virtual const char32_t* Name() override;
	virtual const char32_t* Desc() override;
	virtual const char32_t* Author() override;
	virtual const char32_t* Copyright() override;
	virtual uint32_t Version() override;
	virtual uint32_t SDKVersion() override;
	virtual PluginClassID PluginType() override;

	/// PluginObject
	virtual EObjectType ObjectType() override;
	virtual const char32_t* Category() override;
	virtual const char32_t* TitleName() override;
	virtual PluginClassID ClassID() override;
	virtual AppSceneObject* CreateObject() override;
};

#endif

