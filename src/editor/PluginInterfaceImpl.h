#pragma once
#ifndef _PCEII_H_
#define _PCEII_H_

class PluginInterfaceImpl : public PluginInterface
{
public:
	PluginInterfaceImpl();
	virtual ~PluginInterfaceImpl();

	virtual void* MemAlloc(size_t) override;
	virtual void MemFree(void*) override;
	virtual bool GUIDIsEqual(const alGUID& g1, const alGUID& g2) override;
};

#endif

