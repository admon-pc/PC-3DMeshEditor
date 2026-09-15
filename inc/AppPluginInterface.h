#pragma once
#ifndef _PCEDITOR_PLUGININTERFACE_H_
#define _PCEDITOR_PLUGININTERFACE_H_

#include "al.h"
#include "Common/alGUID.h"

#include "editor.lib.h"

// Viewport can draw triangles or lines or all together
enum class AppViewportDrawMode : uint32_t
{
	// draw filled triangles with material
	Material,

	// draw only line-model
	Wireframe,

	// draw all
	MaterialWireframe
};

struct AppViewportData
{
	AppViewportDrawMode m_drawMode = AppViewportDrawMode::Material;
	AppMat4 m_viewMatrix;
	AppMat4 m_projectMatrix;

	void* m_activeCamera = 0;
};


#include "AppGraphicsObject.h"

#define APP_SDK_VERSION 1

#define APP_DEFINE_GUID AL_DEFINE_GUID

// Default Class IDs
// 
// Plugins
// {D1B539F5-B468-4AB6-80C5-FF63291C1938}
APP_DEFINE_GUID(APP_CLASS_ID_PLUGIN_STD,
	0xd1b539f5, 0xb468, 0x4ab6, 0x80, 0xc5, 0xff, 0x63, 0x29, 0x1c, 0x19, 0x38);
// {43B5A545-0B15-4223-A28F-96DB139525F2}
//APP_DEFINE_GUID(APP_CLASS_ID_PLUGIN_TYPE_IMPORT,
//	0x43b5a545, 0xb15, 0x4223, 0xa2, 0x8f, 0x96, 0xdb, 0x13, 0x95, 0x25, 0xf2);
// {16DB0562-80CC-4810-8D6E-2545DD92CCE5}
//APP_DEFINE_GUID(APP_CLASS_ID_PLUGIN_TYPE_EXPORT,
//	0x16db0562, 0x80cc, 0x4810, 0x8d, 0x6e, 0x25, 0x45, 0xdd, 0x92, 0xcc, 0xe5);

// 
// Objects
// {4FB9983C-D263-48B5-A149-8D15AB5AAD43}
APP_DEFINE_GUID(APP_CLASS_ID_OBJECT_PLANE,
	0x4fb9983c, 0xd263, 0x48b5, 0xa1, 0x49, 0x8d, 0x15, 0xab, 0x5a, 0xad, 0x43);

#define AppPluginClassID alGUID


class AppPluginInterface
{
public:
	AppPluginInterface() {}
	virtual ~AppPluginInterface() {}

	virtual void* MemAlloc(size_t) = 0;
	virtual void MemFree(void*) = 0;
	virtual bool GUIDIsEqual(const alGUID&, const alGUID&) = 0;
	virtual size_t strlen(const char32_t* s) = 0;
	virtual int32_t strcmp(const char32_t* s1, const char32_t* s2) = 0;
	virtual uint32_t sprintf(char32_t* str, const char32_t* format, ...) = 0;
	virtual uint32_t snprintf(char32_t* str, size_t n, const char32_t* format, ...) = 0;
	
	virtual AppGraphicsObject* CreateGraphicsObject(AppMesh* m) = 0;
	virtual void Destroy(AppGraphicsObject*) = 0;
};

#include "AppScene.h"

class AppPluginObject;
class AppPluginImport;
class AppPluginExport;
class AppPlugin
{
protected:
	AppPluginInterface* m_interface = 0;
public:
	AppPlugin(AppPluginInterface* i) : m_interface(i) {}
	virtual ~AppPlugin() {}
	virtual const char32_t* Name() = 0;
	virtual const char32_t* Desc() = 0;
	//virtual const char32_t* Author() = 0;
	virtual const char32_t* Copyright() = 0;
	virtual uint32_t Version() = 0;
	virtual uint32_t SDKVersion() = 0;
//	virtual AppPluginClassID PluginType() = 0;
	virtual AppPluginClassID PluginID() = 0;
	virtual uint32_t GetObjectPluginNum() = 0;
	virtual uint32_t GetImportPluginNum() = 0;
	virtual uint32_t GetExportPluginNum() = 0;
	virtual AppPluginObject* GetObjectPlugin(uint32_t) = 0;
	virtual AppPluginImport* GetImportPlugin(uint32_t) = 0;
	virtual AppPluginExport* GetExportPlugin(uint32_t) = 0;

	AppPluginInterface* GetPluginInterface() { return m_interface; }
};

class AppPluginObject/* : public AppPlugin*/
{
protected:
	AppPlugin* m_plugin = 0;
public:
	//AppPluginObject(AppPluginInterface* i) :AppPlugin(i) {}
	AppPluginObject(AppPlugin* p):m_plugin(p){}
	virtual ~AppPluginObject() {}

	// This will set where creation button will be located.
	// All objects on the scene are one of these types.
	// When you need to create new object, you click on Create button,
	// then on specific button that represent this EObjectType.
	// After this, below, there must appear a ComboBox.
	// This combobox will have categories. Category is a plugin thing.
	// When you create PluginObject, you return 'Category' as a string.
	// I don't know how to call it. Let's call it category. It's your category.
	// When you select category, you know, from which plugin all this coming.
	// Each plugin contains only one object. If they all have same category,
	// buttons for creating these objects will be shown together.
	// Each Button will have title (method TitleName should return this title).
	// All objects should have unique PluginClassID.
	//
	enum EObjectType
	{
		EObjectType_Polygonal,
		EObjectType_Helper,

		EObjectType__end,
	};

	virtual EObjectType ObjectType() = 0;
	virtual const char32_t* Category() = 0;
	virtual const char32_t* TitleName() = 0;
	virtual AppPluginClassID ClassID() = 0;
	virtual AppSceneObject* CreateObject() = 0;
	virtual void DestroyObject(AppSceneObject*) = 0;

	AppPlugin* GetPlugin() { return m_plugin; }
};

class AppPluginImport/* : public AppPlugin*/
{
public:
	//AppPluginImport(AppPluginInterface* i) :AppPlugin(i) {}
	virtual ~AppPluginImport() {}
};

class AppPluginExport /*: public AppPlugin*/
{
public:
	//AppPluginExport(AppPluginInterface* i) :AppPlugin(i) {}
	virtual ~AppPluginExport() {}
};



typedef AppPlugin* (AL_CDECL* AppPluginLoad_t)(AppPluginInterface*);
typedef void (AL_CDECL* AppPluginUnload_t)();

#endif

