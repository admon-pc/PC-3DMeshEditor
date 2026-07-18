#include "editor.h"
#include <Windows.h>

Application* g_app = 0;
alMat4 g_emptyMatrix;

AppGSShaderCallback_LineModel3D::AppGSShaderCallback_LineModel3D()
{
}

AppGSShaderCallback_LineModel3D::~AppGSShaderCallback_LineModel3D()
{
	AL_DESTROY(m_shader);
}

void AppGSShaderCallback_LineModel3D::OnSetShader()
{
}

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
	inf.m_vertexType = alMeshVertexType::Line;
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

void PrintLogFunction(const char* s)
{
	g_app->PrintLog(s);
}

Application::Application()
{
	g_app = this;
	m_colorThemeCurr = &m_colorTheme;
}

Application::~Application()
{
	AL_DESTROY(m_blackTexture);
	AL_DESTROY(m_transparentTexture);
	AL_DESTROY(m_shaderLineModel);
	AL_DESTROY(m_gs);
	AL_DESTROY(m_windowCallback);

	if (m_fileLog)
		fclose(m_fileLog);
}

bool Application::OnCreate(const char* videoDriver)
{
	alLog::SetPrintFunction(PrintLogFunction);
	alLog::PrintInfo("%s : %s\n", __DATE__, __TIME__);


	for (uint32_t i = 0; i < (uint32_t)AppCursorType::_count; ++i)
	{
		switch ((AppCursorType)i)
		{
		case AppCursorType::Arrow: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/arrow.cur"); break;
		case AppCursorType::Cross: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::Hand: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/link.cur"); break;
		case AppCursorType::Help: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/helpsel.cur"); break;
		case AppCursorType::IBeam: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/select.cur"); break;
		case AppCursorType::No: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/unavail.cur"); break;
		case AppCursorType::Size: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/move.cur"); break;
		case AppCursorType::SizeNESW: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/nesw.cur"); break;
		case AppCursorType::SizeNS: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/ns.cur"); break;
		case AppCursorType::SizeNWSE: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/nwse.cur"); break;
		case AppCursorType::SizeWE: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/ew.cur"); break;
		case AppCursorType::UpArrow: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/up.cur"); break;
		case AppCursorType::Wait: m_cursors[i] = alLib::CreateCursor((alCursorType)i, "../data/cursors/aero-no-tail/working.ani"); break;
		case AppCursorType::SelectObject: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::SelectVertex: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/aero-no-tail/prec.cur"); break;
		case AppCursorType::Rotate: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/rotate.cur"); break;
		default:break;
		}

		if (i < (uint32_t)alCursorType::_count)
			alLib::SetCursor((alCursorType)i, m_cursors[i]);
	}
	alLib::SetCursor(alCursorType::Arrow, m_cursors[(uint32_t)AppCursorType::Arrow]);

	m_windowCallback = alCreate<SystemWindowCallback>(this);
	m_mainWindow = alLib::CreateSystemWindow(m_windowCallback);
	if (!m_mainWindow)
		return false;
	m_mainWindow->Show();

	m_gs = alLib::CreateGS(alVideoDriverType::Direct3D11);
	if (!m_gs->Init(m_mainWindow))
		return false;
	{
		m_shaderLineModel = alCreate<AppGSShaderCallback_LineModel3D>();
		if (!m_shaderLineModel->Create(m_gs))
			return false;
	}

	m_gs->GetDepthRange(&m_gpuDepthRange);
	{
		alImage img;
		img.Create(2, 2);
		img.Fill(ColorBlack);
		m_blackTexture = m_gs->CreateTexturePoint(&img);
		img.Fill(ColorTransparent);
		m_transparentTexture = m_gs->CreateTexturePoint(&img);
	}
	UpdateWindowTitle();
	_initGridMesh();
	_initViewports();

	return true;
}

void Application::UpdateWindowTitle()
{
	alStringA text;

	text += "3DEditor ";
	//text += m_version;

	text += " - ";
	text += m_gs->GetVideoDriverName();

	text += " - ";
	text += __DATE__;

	/*if (m_workingSceneFile.size())
	{
		text += " - ";
		text += m_workingSceneFile.data();
	}*/

	/*if (m_needSave)
		text += " *";*/
	m_mainWindow->SetTitle(text.data());
}

void Application::MainLoop()
{
	auto dt = alLib::GetDeltaTime();
	auto currThread = GetCurrentThread();
	while (m_run)
	{
		alLib::Update();
		m_dt = *dt;
		auto input = alLib::GetInput();
		m_isCursorMove = (input->m_mouseDelta.x != 0.f) || (input->m_mouseDelta.y != 0.f);

		WaitForSingleObject(currThread, 10);

		UpdateViewports();

		m_gs->BeginDraw();
		m_gs->ClearAll();

		DrawViewports3D();

		m_gs->BeginDrawGUI();
		DrawViewports();
		//m_gs->BeginDrawGUI(false);

		//m_GUI->m_context->DrawAll();
		m_gs->EndDrawGUI();

		m_gs->EndDraw();
		m_gs->SwapBuffers();
	}
}

void Application::PrintLog(const char* s)
{
	if (!m_fileLog)
	{
		fopen_s(&m_fileLog, "log.txt", "wb");
	}

	printf("%s", s);

	if (m_fileLog)
	{
		fprintf(m_fileLog, "%s", s);
	}
}

void Application::GetRayFromScreen(alRay* ray, const alVec2f& coords, const alVec4f& viewportRect, const alMat4& VPInvert)
{
	AL_ASSERT_ST(ray);
	alVec2f point;
	point.x = coords.x - viewportRect.x;
	point.y = coords.y - viewportRect.y;
	float pt_x = ((float)point.x / (viewportRect.z - viewportRect.x)) * 2.f - 1.f;
	float pt_y = -((float)point.y / (viewportRect.w - viewportRect.y)) * 2.f + 1.f;

	alMat4 VPI;
	auto VPIptr = VPI.GetPtr();
	auto VPInvertptr = VPInvert.GetPtrConst();
	for (int i = 0; i < 16; ++i) {
		VPIptr[i] = VPInvertptr[i];
	}
	auto O = alMath::Mul(alVec4(pt_x, pt_y, m_gpuDepthRange.x, 1.f), VPI);
	auto E = alMath::Mul(alVec4(pt_x, pt_y, m_gpuDepthRange.y, 1.f), VPI);

	ray->m_origin.Set(O.x, O.y, O.z, O.w);
	ray->m_end.Set(E.x, E.y, E.z, E.w);

	ray->m_origin.w = 1.0f / ray->m_origin.w;
	ray->m_origin.x *= ray->m_origin.w;
	ray->m_origin.y *= ray->m_origin.w;
	ray->m_origin.z *= ray->m_origin.w;

	ray->m_end.w = 1.0f / ray->m_end.w;
	ray->m_end.x *= ray->m_end.w;
	ray->m_end.y *= ray->m_end.w;
	ray->m_end.z *= ray->m_end.w;

	ray->Update();
}

void Application::_initGridMesh()
{
	auto _get_mesh = [&](int32_t linesNum)->alMesh*
	{
		alMesh* mesh = alCreate<alMesh>();
		mesh->m_stride = sizeof(alVertexLine);
		mesh->m_vertexType = alMeshVertexType::Line;
		mesh->m_vCount = linesNum * 2;
		mesh->m_vertices = (uint8_t*)alMemory::Malloc(mesh->m_vCount * mesh->m_stride);
		mesh->m_iCount = linesNum * 2;
		mesh->m_indices = (uint8_t*)alMemory::Malloc(mesh->m_iCount * sizeof(uint16_t));
		return mesh;
	};

	alColor colorBase(180, 180, 180, 255);
	alVec4f colorRed(1.f, 0.f, 0.f, 1.f);
	alVec4f colorGreen(0.f, 1.f, 0.f, 1.f);

	auto _build = [&](alMesh* mesh, float32_t minimum, int32_t linesNum, float32_t step,
		const alColor& half1Color, const alColor& half2Color, const alColor& specColor1, const alColor& specColor2, int32_t specColorIndex,
		AppViewportCameraType ct
		)
	{
		auto vertex = (alVertexLine*)mesh->m_vertices;
		float32_t pos = minimum;

		const int32_t halfNum = linesNum / 2;

		for (int32_t i = 0; i < halfNum; ++i)
		{
			alVec4f color = half1Color.GetV4f();

			if (i == specColorIndex)
				color = specColor1.GetV4f();

			switch (ct)
			{
			case AppViewportCameraType::Left:
			case AppViewportCameraType::Right:
				vertex->Position.Set(0.f, minimum, pos);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(0.f, std::abs(minimum), pos);
				vertex->Color = color;
				vertex++;
				break;
			case AppViewportCameraType::Perspective:
			case AppViewportCameraType::Top:
			case AppViewportCameraType::Bottom:
				vertex->Position.Set(pos, 0.f, minimum);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(pos, 0.f, std::abs(minimum));
				vertex->Color = color;
				vertex++;
				break;
			case AppViewportCameraType::Front:
			case AppViewportCameraType::Back:
				vertex->Position.Set(pos, minimum, 0.f);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(pos, std::abs(minimum), 0.f);
				vertex->Color = color;
				vertex++;
				break;
			default:
				break;
			}

			pos += step;
		}

		pos = minimum;
		for (int32_t i = 0; i < halfNum; ++i)
		{
			alVec4f color = half2Color.GetV4f();

			if (i == specColorIndex)
				color = specColor2.GetV4f();

			switch (ct)
			{
			case AppViewportCameraType::Left:
			case AppViewportCameraType::Right:
				vertex->Position.Set(0.f, pos, -minimum);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(0.f, pos, minimum);
				vertex->Color = color;
				vertex++;
				break;
			case AppViewportCameraType::Perspective:
			case AppViewportCameraType::Top:
			case AppViewportCameraType::Bottom:
				vertex->Position.Set(-minimum, 0.f, pos);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(minimum, 0.f, pos);
				vertex->Color = color;
				vertex++;
				break;
			case AppViewportCameraType::Front:
			case AppViewportCameraType::Back:
				vertex->Position.Set(-minimum, pos, 0.f);
				vertex->Color = color;
				vertex++;
				vertex->Position.Set(minimum, pos, 0.f);
				vertex->Color = color;
				vertex++;
				break;
			default:
				break;
			}

			pos += step;
		}

		uint16_t* index = (uint16_t*)mesh->m_indices;
		for (uint32_t i = 0; i < mesh->m_iCount; ++i)
		{
			*index = (uint16_t)i;
			index++;
		}
	};

	{
		const int32_t linesNum = 22;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -5.f, linesNum, 1.f, colorBase, colorBase, ColorLime, ColorRed, 5, AppViewportCameraType::Perspective);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_perspective1 = m_gs->CreateMesh(&mi);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -5.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 5, AppViewportCameraType::Perspective);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_perspective2 = m_gs->CreateMesh(&mi);
		}
		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorLime, ColorRed, 251, AppViewportCameraType::Top);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_top1 = m_gs->CreateMesh(&mi);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Top);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_top2 = m_gs->CreateMesh(&mi);
		}
		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorBlue, ColorRed, 251, AppViewportCameraType::Front);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_front1 = m_gs->CreateMesh(&mi);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Front);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_front2 = m_gs->CreateMesh(&mi);
		}

		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorBlue, ColorLime, 251, AppViewportCameraType::Left);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_left1 = m_gs->CreateMesh(&mi);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Left);

		{
			alGSMeshInfo mi;
			mi.m_meshPtr = mesh;
			m_gridModel_left2 = m_gs->CreateMesh(&mi);
		}

		AL_DESTROY(mesh);
	}
}

void Application::SetMouseMode(AppMouseMode mm)
{
	m_mouseMode = mm;
	switch (m_mouseMode)
	{
	case AppMouseMode::CommonMode:
	default:
		alLib::SetCursorClip(0, 0, 0);
		//miSetCursorDisableAutoChange(false);
		//miShowCursor(true);
		//miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
		//m_cursors[(u32)miCursorType::Arrow]->Activate();
		break;
	case AppMouseMode::ClickAndDrag:
		alLib::SetCursorClip(0, 0, 0);
		//miSetCursorDisableAutoChange(false);
		//miShowCursor(true);
		//miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Cross]);
		//m_cursors[(u32)miCursorType::Cross]->Activate();
		break;
	case AppMouseMode::HideCursor:
		//miSetCursorDisableAutoChange(true);
		//miShowCursor(false);
		break;
	case AppMouseMode::SelectObject:
		break;
	case AppMouseMode::SelectVertex:
		break;
	case AppMouseMode::Other:
		break;
	}
}

void Application::OnWindowSizeChanged()
{
	if (m_gs)
	{
		m_gs->UpdateWindowData();
	}
	if (m_activeViewportLayout)
	{
		for (size_t i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
		{
			auto viewport = m_activeViewportLayout->m_viewports[i];
			viewport->OnWindowSize();
		}
	}

}
