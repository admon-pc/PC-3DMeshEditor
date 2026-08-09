#pragma once
#ifndef _PLUGINSTDo_H_
#define _PLUGINSTDo_H_

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
	virtual const char32_t* Category() override;
	virtual const char32_t* SubCategory() override;
	virtual const char32_t* TitleName() override;
	virtual PluginClassID ClassID() override;
};

#endif

