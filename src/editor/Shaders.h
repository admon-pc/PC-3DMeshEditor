#pragma once
#ifndef _PCAPPSHDERS_H_
#define _PCAPPSHDERS_H_

class AppGSShaderCallback_LineModel3D : public alGSShaderCallback
{
public:
	AppGSShaderCallback_LineModel3D();
	virtual ~AppGSShaderCallback_LineModel3D();

	virtual void OnSetShader() override;
	virtual void OnSetConstants() override;

	bool Create(alGS*);

	alGSShader* m_shader = 0;
	alGSShaderConstantBuffer* m_cbV = 0;
	alGSShaderConstantBuffer* m_cbP = 0;

	struct cbVertex
	{
		alMat4 WVP;
	}
	m_cbVertexData;

	struct cbPixel
	{
		alColor BaseColor;
	}
	m_cbPixelData;
};


class AppGSShaderCallback_DefaultTriangle : public alGSShaderCallback
{
public:
	AppGSShaderCallback_DefaultTriangle();
	virtual ~AppGSShaderCallback_DefaultTriangle();

	virtual void OnSetShader() override;
	virtual void OnSetConstants() override;

	bool Create(alGS*);
	alGSShaderConstantBuffer* m_constantBuffer = 0;
	alGSShader* m_shader = 0;

	struct cbVertex
	{
		alMat4 WVP;
		alMat4 W;
		alVec4f ViewDir;
	}
	m_cbVertexData;

	alGSTexture* m_texture1 = 0;
};


#endif

