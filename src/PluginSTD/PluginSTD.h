#pragma once
#ifndef _PLUGINSTD_H_
#define _PLUGINSTD_H_

#include "PluginSTD_import.h"
#include "PluginSTD_export.h"
#include "PluginSTD_object.h"

class EditorPluginSTD : public EditorPlugin
{
public:
	EditorPluginSTD(EditorInterface* i);
	virtual ~EditorPluginSTD();
	virtual const char32_t* Name() override;
	virtual const char32_t* Desc() override;
	virtual const char32_t* Author() override;
	virtual const char32_t* Copyright() override;
	virtual uint32_t Version() override;
	virtual uint32_t ExporterNum() override;
	virtual uint32_t ImporterNum() override;
	virtual uint32_t ObjectNum() override;
	virtual EditorPluginObject* GetPluginObject(uint32_t) override;
	virtual EditorPluginImport* GetPluginImport(uint32_t) override;
	virtual EditorPluginExport* GetPluginExport(uint32_t) override;
	virtual uint32_t SDKVersion() override;
};

#endif

