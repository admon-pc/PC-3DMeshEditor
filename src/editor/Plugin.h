#pragma once
#ifndef _PCEDPLUG_H_
#define _PCEDPLUG_H_

class EditorInterface
{
public:
	EditorInterface() {}
	virtual ~EditorInterface() {}

	virtual void* MemAlloc(size_t) = 0;
	virtual void MemFree(void*) = 0;
};
class EditorPluginBase
{
public:
	EditorPluginBase() {}
	virtual ~EditorPluginBase() {}
	virtual const char32_t* Name() = 0;
	virtual const char32_t* Desc() = 0;
	virtual const char32_t* Author() = 0;
	virtual const char32_t* Copyright() = 0;
	virtual uint32_t Version() = 0;
};
class EditorPluginObject 
{
public:
	EditorPluginObject() {}
	virtual ~EditorPluginObject() {}
};
class EditorPluginImport
{
public:
	EditorPluginImport() {}
	virtual ~EditorPluginImport() {}
};
class EditorPluginExport
{
public:
	EditorPluginExport() {}
	virtual ~EditorPluginExport() {}
};

class EditorPlugin : public EditorPluginBase
{
protected:
	EditorInterface* m_interface = 0;
public:
	EditorPlugin(EditorInterface*i) : m_interface(i){}
	virtual ~EditorPlugin()
	{
	}

	virtual uint32_t ExporterNum() = 0;
	virtual uint32_t ImporterNum() = 0;
	virtual uint32_t ObjectNum() = 0;
	virtual EditorPluginObject* GetPluginObject(uint32_t) = 0;
	virtual EditorPluginImport* GetPluginImport(uint32_t) = 0;
	virtual EditorPluginExport* GetPluginExport(uint32_t) = 0;
	
	virtual uint32_t SDKVersion() = 0;
};
typedef EditorPlugin* (AL_CDECL* EditorLoadPlugin_t)(EditorInterface*);

#endif

