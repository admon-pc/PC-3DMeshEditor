#include "editor.h"
#include "AppGraphicsObjectImpl.h"

extern Application* g_app;

AppGraphicsObjectImpl::AppGraphicsObjectImpl(alGS*gs, alMeshVertexType vt)
	:
	m_gs(gs)
{
	switch (vt){
	case alMeshVertexType::AnimatedTriangle:
		m_primitiveType = alGSPrimitiveType::Triangle;
		break;
	case alMeshVertexType::AnimatedLine:
		m_primitiveType = alGSPrimitiveType::Line;
		break;
	case alMeshVertexType::AnimatedPoint:
		m_primitiveType = alGSPrimitiveType::Point;
		break;
	default:
		break;
	}
}

AppGraphicsObjectImpl::~AppGraphicsObjectImpl()
{
	AL_DESTROY(m_GPUMesh);
}

void AppGraphicsObjectImpl::Draw(AppViewportData* viewport, AppSceneObject* object)
{
	AppViewportCamera* activeCamera = (AppViewportCamera*)viewport->m_activeCamera;

	alMat4 W, V, P;
	W = Application::AppMatToAlMat(*object->GetWMatrix());
	V = activeCamera->m_viewMatrix;
	P = activeCamera->m_projectionMatrix;

	alVec3 cameraTarget(0, 0., 0);
	alVec3 cameraPos(0., 3., 3.0);
	alVec3 cameraDir = cameraTarget - cameraPos;
	cameraDir.Normalize2();

	switch (m_primitiveType)
	{
	case alGSPrimitiveType::Triangle:
		g_app->m_shaderDefaultTriangle->m_cbVertexData.W = W;
		g_app->m_shaderDefaultTriangle->m_cbVertexData.WVP = P * V * W;
		g_app->m_shaderDefaultTriangle->m_cbVertexData.ViewDir.x = (float32_t)cameraDir.x;
		g_app->m_shaderDefaultTriangle->m_cbVertexData.ViewDir.y = (float32_t)cameraDir.y;
		g_app->m_shaderDefaultTriangle->m_cbVertexData.ViewDir.z = (float32_t)cameraDir.z;
		m_gs->SetShader(g_app->m_shaderDefaultTriangle->m_shader);
	
		g_app->m_shaderDefaultTriangle->OnSetConstants();
		g_app->m_shaderDefaultTriangle->m_texture1 = m_gs->GetWhiteTexture();
		break;
	case alGSPrimitiveType::Line:
		g_app->m_shaderLineModel->m_cbVertexData.WVP = P * V * W;
		g_app->m_shaderLineModel->m_cbPixelData.BaseColor = ColorWhite;
		m_gs->SetShader(g_app->m_shaderLineModel->m_shader);
		g_app->m_shaderLineModel->OnSetConstants();
		break;
	case alGSPrimitiveType::Point:
		g_app->m_shaderPointModel->m_cbVertexData.WVP = P * V * W;
		g_app->m_shaderPointModel->m_cbVertexData.Eye.Set(cameraPos.x, cameraPos.y, cameraPos.z, 0.f);
		g_app->m_shaderPointModel->m_cbVertexData.Viewport.Set(800.f, 600.f, 0.f, 0.f);
		m_gs->SetShader(g_app->m_shaderPointModel->m_shader);
		g_app->m_shaderPointModel->OnSetConstants();
		break;
	}


	m_gs->SetRasterizationType(alGSRasterizationType::Solid);
	m_gs->SetPrimitiveType(m_primitiveType);
	m_gs->EnableDepth();
	m_gs->SetMesh(m_GPUMesh);
	m_gs->Draw();
}
