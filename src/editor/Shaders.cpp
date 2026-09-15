#include "editor.h"

AppGSShaderCallback_PointModel::AppGSShaderCallback_PointModel() {}
AppGSShaderCallback_PointModel::~AppGSShaderCallback_PointModel() {
	AL_DESTROY(m_shader);
}
void AppGSShaderCallback_PointModel::OnSetShader() {}
void AppGSShaderCallback_PointModel::OnSetConstants() 
{
	m_cbV->MapData(&m_cbVertexData, sizeof(m_cbVertexData));
	m_cbV->VSSetConstantBuffers(0);
	m_cbV->GSSetConstantBuffers(0);
}
bool AppGSShaderCallback_PointModel::Create(alGS* gs)
{
	alGSShaderCreationInfo inf;
	inf.m_callback = this;
	inf.m_vertexType = alMeshVertexType::AnimatedPoint;
	//inf.m_saveShaderToFile_VS = "../data/shaders/d3d11/ScreenQuad.vs";
	//inf.m_saveShaderToFile_PS = "../data/shaders/d3d11/ScreenQuad.ps";
	//inf.m_saveShaderToFile_GS = "../data/shaders/d3d11/ScreenQuad.gs";
	inf.m_shaderEntry_VS = "VSMain";
	inf.m_shaderEntry_PS = "PSMain";
	inf.m_shaderEntry_GS = "GSMain";
	inf.m_shaderFile_VS = "../data/shaders/d3d11/PointModel.hlsl";
	inf.m_shaderFile_PS = "../data/shaders/d3d11/PointModel.hlsl";
	inf.m_shaderFile_GS = "../data/shaders/d3d11/PointModel.hlsl";
	inf.m_shaderModel_VS = "vs_5_0";
	inf.m_shaderModel_PS = "ps_5_0";
	inf.m_shaderModel_GS = "gs_5_0";

	m_shader = gs->CreateShader(inf);
	if (!m_shader)
		return false;

	m_cbV = m_shader->CreateConstantBuffer(sizeof(m_cbVertexData));

	return true;
}

AppGSShaderCallback_LineModel3D::AppGSShaderCallback_LineModel3D(){}
AppGSShaderCallback_LineModel3D::~AppGSShaderCallback_LineModel3D()
{
	AL_DESTROY(m_shader);
}

void AppGSShaderCallback_LineModel3D::OnSetShader(){}
void AppGSShaderCallback_LineModel3D::OnSetConstants()
{
	m_cbV->MapData(&m_cbVertexData, sizeof(m_cbVertexData));
	m_cbP->MapData(&m_cbPixelData, sizeof(m_cbPixelData));
	m_cbV->VSSetConstantBuffers(0);
	m_cbP->PSSetConstantBuffers(0);
}

bool AppGSShaderCallback_LineModel3D::Create(alGS* gs)
{
	alGSShaderCreationInfo inf;
	inf.m_callback = this;
	inf.m_vertexType = alMeshVertexType::AnimatedLine;
	//inf.m_vertexType = alMeshVertexType::Point;
	//inf.m_saveShaderToFile_VS = "../data/shaders/d3d11/ScreenQuad.vs";
	//inf.m_saveShaderToFile_PS = "../data/shaders/d3d11/ScreenQuad.ps";
	//inf.m_saveShaderToFile_GS = "../data/shaders/d3d11/ScreenQuad.gs";
	inf.m_shaderEntry_VS = "VSMain";
	inf.m_shaderEntry_PS = "PSMain";
	inf.m_shaderFile_VS = "../data/shaders/d3d11/LineModel.hlsl";
	inf.m_shaderFile_PS = "../data/shaders/d3d11/LineModel.hlsl";
	inf.m_shaderModel_VS = "vs_5_0";
	inf.m_shaderModel_PS = "ps_5_0";

	m_shader = gs->CreateShader(inf);
	if (!m_shader)
		return false;

	m_cbV = m_shader->CreateConstantBuffer(sizeof(m_cbVertexData));
	m_cbP = m_shader->CreateConstantBuffer(sizeof(m_cbPixelData));

	return true;
}

AppGSShaderCallback_DefaultTriangle::AppGSShaderCallback_DefaultTriangle(){}
AppGSShaderCallback_DefaultTriangle::~AppGSShaderCallback_DefaultTriangle()
{
	AL_DESTROY(m_shader);
}

void AppGSShaderCallback_DefaultTriangle::OnSetShader(){}
void AppGSShaderCallback_DefaultTriangle::OnSetConstants()
{
	m_constantBuffer->MapData(&m_cbVertexData, sizeof(m_cbVertexData));
	m_constantBuffer->VSSetConstantBuffers(0);
	m_constantBuffer->PSSetConstantBuffers(0);
	if (m_texture1)
		m_shader->SetTexture(m_texture1, 0);
}

bool AppGSShaderCallback_DefaultTriangle::Create(alGS* gs)
{
	alGSShaderCreationInfo inf;
	inf.m_callback = this;
	inf.m_vertexType = alMeshVertexType::AnimatedTriangle;
	//inf.m_vertexType = alMeshVertexType::Point;
	//inf.m_saveShaderToFile_VS = "../data/shaders/d3d11/ScreenQuad.vs";
	//inf.m_saveShaderToFile_PS = "../data/shaders/d3d11/ScreenQuad.ps";
	//inf.m_saveShaderToFile_GS = "../data/shaders/d3d11/ScreenQuad.gs";
	inf.m_shaderEntry_VS = "VSMain";
	inf.m_shaderEntry_PS = "PSMain";
	inf.m_shaderFile_VS = "../data/shaders/d3d11/DefaultTriangle.hlsl";
	inf.m_shaderFile_PS = "../data/shaders/d3d11/DefaultTriangle.hlsl";
	inf.m_shaderModel_VS = "vs_5_0";
	inf.m_shaderModel_PS = "ps_5_0";

	m_shader = gs->CreateShader(inf);
	if (!m_shader)
		return false;

	m_constantBuffer = m_shader->CreateConstantBuffer(sizeof(m_cbVertexData));

	return true;
}


