#pragma once
#ifndef _PCEII_H_
#define _PCEII_H_

class EditorInterfaceImpl : public EditorInterface
{
public:
	EditorInterfaceImpl();
	virtual ~EditorInterfaceImpl();

	virtual void* MemAlloc(size_t) override;
	virtual void MemFree(void*) override;
};

#endif

