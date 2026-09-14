#pragma once
#ifndef _PCAPPGOI_H_
#define _PCAPPGOI_H_

class AppGraphicsObjectImpl : public AppGraphicsObject
{
	alGS* m_gs = 0;
	//alMeshVertexType m_vertexType = alMeshVertexType::AnimatedTriangle;
	alGSPrimitiveType m_primitiveType = alGSPrimitiveType::Triangle;
public:
	AppGraphicsObjectImpl(alGS*, alMeshVertexType);
	virtual ~AppGraphicsObjectImpl();
	virtual void Draw(AppViewportData*, AppSceneObject*) override;

	alGSMesh* m_GPUMesh = 0;
};

#endif

