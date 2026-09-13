#pragma once
#ifndef _PCEII_H_
#define _PCEII_H_

class AppPluginInterfaceImpl : public AppPluginInterface
{
	Application* m_app = 0;
public:
	AppPluginInterfaceImpl(Application*);
	virtual ~AppPluginInterfaceImpl();

	virtual void* MemAlloc(size_t) override;
	virtual void MemFree(void*) override;
	virtual bool GUIDIsEqual(const alGUID& g1, const alGUID& g2) override;
	virtual size_t strlen(const char32_t* s) override;
	virtual int32_t strcmp(const char32_t* s1, const char32_t* s2) override;
	virtual uint32_t sprintf(char32_t* str, const char32_t* format, ...) override;
	virtual uint32_t snprintf(char32_t* str, size_t n, const char32_t* format, ...) override;
	
	virtual AppGraphicsObject* CreateGraphicsObject(AppMesh* desc) override;
	virtual void Destroy(AppGraphicsObject*) override;
};

#endif

