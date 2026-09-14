#pragma once
#ifndef _PLUGINSTDo_H_
#define _PLUGINSTDo_H_

class AppSceneObject_plane : public AppSceneObject
{
public:
	AppSceneObject_plane(AppPluginObject* po);
	virtual ~AppSceneObject_plane();
	
	virtual void Draw(AppViewportDrawMode, AppPluginInterface*) override;

	AppGraphicsObject* m_testGO_triangle = 0;
	AppGraphicsObject* m_testGO_line = 0;
	AppGraphicsObject* m_testGO_point = 0;
};

class AppPluginObject_plane : public AppPluginObject
{
public:
	AppPluginObject_plane(AppPluginInterface* i);
	virtual ~AppPluginObject_plane();

	/// Plugin
	virtual const char32_t* Name() override;
	virtual const char32_t* Desc() override;
	virtual const char32_t* Author() override;
	virtual const char32_t* Copyright() override;
	virtual uint32_t Version() override;
	virtual uint32_t SDKVersion() override;
	virtual AppPluginClassID PluginType() override;

	/// PluginObject
	virtual EObjectType ObjectType() override;
	virtual const char32_t* Category() override;
	virtual const char32_t* TitleName() override;
	virtual AppPluginClassID ClassID() override;
	virtual AppSceneObject* CreateObject() override;
	virtual void DestroyObject(AppSceneObject*) override;
};

#endif

