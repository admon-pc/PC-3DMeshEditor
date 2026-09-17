#pragma once
#ifndef _PLUGINSTDo_H_
#define _PLUGINSTDo_H_

class AppPlugin_STD : public AppPlugin
{
	AppPluginObject* m_objects[1];
	uint32_t m_objectsNum = 1;
public:
	AppPlugin_STD(AppPluginInterface* i);
	virtual ~AppPlugin_STD();
	virtual const char32_t* Name() override;
	virtual const char32_t* Desc() override;
	virtual const char32_t* Copyright() override;
	virtual uint32_t Version() override;
	virtual uint32_t SDKVersion() override;
	virtual AppPluginClassID PluginID() override;
	virtual uint32_t GetObjectPluginNum() override;
	virtual uint32_t GetImportPluginNum() override;
	virtual uint32_t GetExportPluginNum() override;
	virtual AppPluginObject* GetObjectPlugin(uint32_t) override;
	virtual AppPluginImport* GetImportPlugin(uint32_t) override;
	virtual AppPluginExport* GetExportPlugin(uint32_t) override;
};

class AppSceneObject_plane : public AppSceneObject
{
public:
	AppSceneObject_plane(AppPluginObject* po);
	virtual ~AppSceneObject_plane();
	
	virtual void Draw(AppViewportData*, AppPluginInterface*) override;

	virtual bool OnSelect(AppSelectionFrust&, AppRay&, bool selectByRectangle, AppEditMode) override;
	virtual bool IsCanSelect(AppSelectionFrust&, AppRay&) override;

	AppGraphicsObject* m_testGO_triangle = 0;
	AppGraphicsObject* m_testGO_line = 0;
	AppGraphicsObject* m_testGO_point = 0;
};

class AppPluginObject_plane : public AppPluginObject
{
public:
	AppPluginObject_plane(AppPlugin* plugin);
	virtual ~AppPluginObject_plane();

	/// PluginObject
	virtual EObjectType ObjectType() override;
	virtual const char32_t* Category() override;
	virtual const char32_t* TitleName() override;
	virtual AppPluginClassID ClassID() override;
	virtual AppSceneObject* CreateObject() override;
	virtual void DestroyObject(AppSceneObject*) override;
};

#endif

