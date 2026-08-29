#pragma once
#ifndef _PCEDITOR_PLUGININTERFACE_H_
#define _PCEDITOR_PLUGININTERFACE_H_

#include "al.h"
#include "Common/alGUID.h"

#define PLUGIN_SDK_VERSION 1

#define PLUGIN_DEFINE_GUID AL_DEFINE_GUID

// Default Class IDs
// 
// Plugin Types
// {D1B539F5-B468-4AB6-80C5-FF63291C1938}
PLUGIN_DEFINE_GUID(PLUGIN_CLASS_ID_PLUGIN_TYPE_OBJECT,
	0xd1b539f5, 0xb468, 0x4ab6, 0x80, 0xc5, 0xff, 0x63, 0x29, 0x1c, 0x19, 0x38);
// {43B5A545-0B15-4223-A28F-96DB139525F2}
PLUGIN_DEFINE_GUID(PLUGIN_CLASS_ID_PLUGIN_TYPE_IMPORT,
	0x43b5a545, 0xb15, 0x4223, 0xa2, 0x8f, 0x96, 0xdb, 0x13, 0x95, 0x25, 0xf2);
// {16DB0562-80CC-4810-8D6E-2545DD92CCE5}
PLUGIN_DEFINE_GUID(PLUGIN_CLASS_ID_PLUGIN_TYPE_EXPORT,
	0x16db0562, 0x80cc, 0x4810, 0x8d, 0x6e, 0x25, 0x45, 0xdd, 0x92, 0xcc, 0xe5);
// 
// Objects
// {4FB9983C-D263-48B5-A149-8D15AB5AAD43}
PLUGIN_DEFINE_GUID(PLUGIN_CLASS_ID_OBJECT_PLANE,
	0x4fb9983c, 0xd263, 0x48b5, 0xa1, 0x49, 0x8d, 0x15, 0xab, 0x5a, 0xad, 0x43);

#define PluginClassID alGUID

#include "Scene.h"

class PluginInterface
{
public:
	PluginInterface() {}
	virtual ~PluginInterface() {}

	virtual void* MemAlloc(size_t) = 0;
	virtual void MemFree(void*) = 0;
	virtual bool GUIDIsEqual(const alGUID&, const alGUID&) = 0;
};

class Plugin
{
	PluginInterface* m_interface = 0;
public:
	Plugin(PluginInterface* i) : m_interface(i) {}
	virtual ~Plugin() {}
	virtual const char32_t* Name() = 0;
	virtual const char32_t* Desc() = 0;
	virtual const char32_t* Author() = 0;
	virtual const char32_t* Copyright() = 0;
	virtual uint32_t Version() = 0;
	virtual uint32_t SDKVersion() = 0;
	virtual PluginClassID PluginType() = 0;

};

class PluginObject : public Plugin
{
public:
	PluginObject(PluginInterface* i):Plugin(i){}
	virtual ~PluginObject() {}

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
	virtual PluginClassID ClassID() = 0;
};

class PluginImport : public Plugin
{
public:
	PluginImport(PluginInterface* i) :Plugin(i) {}
	virtual ~PluginImport() {}
};

class PluginExport : public Plugin
{
public:
	PluginExport(PluginInterface* i) :Plugin(i) {}
	virtual ~PluginExport() {}
};



typedef Plugin* (AL_CDECL* PluginLoad_t)(PluginInterface*);
typedef void (AL_CDECL* PluginUnload_t)();

#endif

