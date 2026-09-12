#pragma once
#ifndef _PCAPPGOI_H_
#define _PCAPPGOI_H_

class AppGraphicsObjectImpl : public AppGraphicsObject
{
public:
	AppGraphicsObjectImpl();
	virtual ~AppGraphicsObjectImpl();

	alGSMesh* m_GPUMesh = 0;
};

#endif

