#include "editor.h"
extern Application* g_app;
extern alMat4 g_emptyMatrix;

void AppViewport_onClick_viewport(alGUIElement* elem)
{
	AppViewport* vp = (AppViewport*)elem->GetUserData();
//	g_app->m_popupViewport = vp;
//	g_app->ShowPopupAtCursor(g_app->m_popup_ViewportParameters);
}

AppViewport::AppViewport(AppViewportType vt, AppViewportCameraType vct, const alVec4& rect1_0) {
	m_viewportType = vt;
	m_creationRect = rect1_0;
	m_isDrawAabbs = false;
	m_isOnLeftBorder = false;
	m_isOnRightBorder = false;
	m_isOnTopBorder = false;
	m_isOnBottomBorder = false;

	if (m_creationRect.x == 0.f) m_isOnLeftBorder = true;
	if (m_creationRect.y == 0.f) m_isOnTopBorder = true;
	if (m_creationRect.z == 1.f) m_isOnRightBorder = true;
	if (m_creationRect.w == 1.f) m_isOnBottomBorder = true;

	m_gs = g_app->m_gs;
	//									old
	//m_gui_group = 0;
	//m_gui_text_vpName = 0;
	//m_gui_button_resetCamera = 0;
	//m_gui_button_viewport = 0;

	m_camera[Camera_Perspective] = new AppViewportCamera(this, AppViewportCameraType::Perspective);
	m_camera[Camera_Top] = new AppViewportCamera(this, AppViewportCameraType::Top);
	m_camera[Camera_Bottom] = new AppViewportCamera(this, AppViewportCameraType::Bottom);
	m_camera[Camera_Front] = new AppViewportCamera(this, AppViewportCameraType::Front);
	m_camera[Camera_Back] = new AppViewportCamera(this, AppViewportCameraType::Back);
	m_camera[Camera_Left] = new AppViewportCamera(this, AppViewportCameraType::Left);
	m_camera[Camera_Right] = new AppViewportCamera(this, AppViewportCameraType::Right);

	switch (vct)
	{
	default:
	case AppViewportCameraType::Perspective:
		m_activeCamera = m_camera[Camera_Perspective];
		break;
	case AppViewportCameraType::Left:
		m_activeCamera = m_camera[Camera_Left];
		break;
	case AppViewportCameraType::Right:
		m_activeCamera = m_camera[Camera_Right];
		break;
	case AppViewportCameraType::Top:
		m_activeCamera = m_camera[Camera_Top];
		break;
	case AppViewportCameraType::Bottom:
		m_activeCamera = m_camera[Camera_Bottom];
		break;
	case AppViewportCameraType::Front:
		m_activeCamera = m_camera[Camera_Front];
		break;
	case AppViewportCameraType::Back:
		m_activeCamera = m_camera[Camera_Back];
		break;
	}
	m_activeCamera->Reset();

	switch (vct)
	{
	case AppViewportCameraType::Perspective:
	case AppViewportCameraType::Top:
		m_rayTestTiangles[0].v1 = alVec4f(-999999.f, 0.f, -999999.f, 0.f);
		m_rayTestTiangles[0].v2 = alVec4f(-999999.f, 0.f, 999999.f, 0.f);
		m_rayTestTiangles[0].v3 = alVec4f(999999.f, 0.f, 999999.f, 0.f);
		m_rayTestTiangles[0].Update();
		m_rayTestTiangles[1].v1 = alVec4f(-999999.f, 0.f, -999999.f, 0.f);
		m_rayTestTiangles[1].v2 = alVec4f(999999.f, 0.f, 999999.f, 0.f);
		m_rayTestTiangles[1].v3 = alVec4f(999999.f, 0.f, -999999.f, 0.f);
		m_rayTestTiangles[1].Update();
		break;
	case AppViewportCameraType::Left:
		break;
	case AppViewportCameraType::Right:
		break;
	case AppViewportCameraType::Bottom:
		break;
	case AppViewportCameraType::Front:
		break;
	case AppViewportCameraType::Back:
		break;
	default:
		break;
	}

	m_currentRect = m_creationRect;
	m_currentRectSize = alVec2f(800.f, 600.f);

	/*m_GUI_panel = g_app->m_GUI->m_context->CreatePanel(alVec2f(), alVec2f());
	m_GUI_panel->m_ignoreInput = true;
	m_GUI_panel->m_draw = false;*/

	//float32_t vpNamePosX = 10.f;
	//m_gui_text_vpName = g_app->m_GUI->m_context->CreateText(alVec2f(vpNamePosX, 2.f),
	//	g_app->m_GUI->m_fontDefault,
	//	L"T");
	//m_gui_text_vpName->m_alignment = miGUIElementAlignment::LeftTop;
	//m_gui_text_vpName->m_ignoreInput = false;
	////m_gui_text_vpName->m_ignoreSetCursorInGUI = false;
	//m_gui_text_vpName->m_userData = this;
	//m_gui_text_vpName->m_onLMBDown = miViewport_onClick_viewport;
	//m_gui_text_vpName->SetParent(m_GUI_panel);

#ifdef AL_DEBUG
	static uint32_t clorid = 0;
	/*switch (clorid)
	{
	default:
	case 0:
		m_GUI_panel->m_color = ColorRed;
		break;
	case 1:
		m_GUI_panel->m_color = ColorLime;
		break;
	case 2:
		m_GUI_panel->m_color = ColorBlue;
		break;
	case 3:
		m_GUI_panel->m_color = ColorYellow;
		break;
	}
	++clorid;
	m_GUI_panel->m_color.setAlpha(0.03f);*/
#endif

	m_cameraType = vct;
	Init();
	OnWindowSize();

}

AppViewport::~AppViewport() {
	//if (m_GUI_panel)
	//	g_app->m_GUI->m_context->DeleteElement(m_GUI_panel);


	for (int32_t i = 0; i < Camera_count_; ++i)
	{
		if (m_camera[i]) delete m_camera[i];
	}
}

void AppViewport::Copy(AppViewport* other) {
	this->SetCameraType(other->m_cameraType);
	this->SetDrawGrid(other->m_drawGrid);
	this->SetDrawMode(other->m_drawMode);
	m_activeCamera->Copy(other->m_activeCamera);
}

void AppViewport::Init() 
{

	SetCameraType(m_cameraType);
}

void AppViewport::SetViewportName(const char32_t* name)
{
	static alUnicodeString n;
	n = name;
	if (m_gui_text_vpName)
		m_gui_text_vpName->SetText(name);
}

void AppViewport::SetCameraType(AppViewportCameraType ct) {
	m_cameraType = ct;
	switch (ct)
	{
	case AppViewportCameraType::Perspective:
		SetViewportName(U"Perspective");
		break;
	case AppViewportCameraType::Left:
		SetViewportName(U"Left");
		break;
	case AppViewportCameraType::Right:
		SetViewportName(U"Right");
		break;
	case AppViewportCameraType::Top:
		SetViewportName(U"Top");
		break;
	case AppViewportCameraType::Bottom:
		SetViewportName(U"Bottom");
		break;
	case AppViewportCameraType::Front:
		SetViewportName(U"Front");
		break;
	case AppViewportCameraType::Back:
		SetViewportName(U"Back");
		break;
	default:
		SetViewportName(U"Orthogonal");
		break;
	}

	switch (ct)
	{
	default:
	case AppViewportCameraType::Perspective:
		m_activeCamera = m_camera[Camera_Perspective];
		break;
	case AppViewportCameraType::Left:
		m_activeCamera = m_camera[Camera_Left];
		break;
	case AppViewportCameraType::Right:
		m_activeCamera = m_camera[Camera_Right];
		break;
	case AppViewportCameraType::Top:
		m_activeCamera = m_camera[Camera_Top];
		break;
	case AppViewportCameraType::Bottom:
		m_activeCamera = m_camera[Camera_Bottom];
		break;
	case AppViewportCameraType::Front:
		m_activeCamera = m_camera[Camera_Front];
		break;
	case AppViewportCameraType::Back:
		m_activeCamera = m_camera[Camera_Back];
		break;
	}
}

void AppViewport::HideGUI() 
{
	//m_GUI_panel->SetVisible(false);
}

void AppViewport::ShowGUI() 
{
	//m_GUI_panel->SetVisible(true);
}

void AppViewport::OnWindowSize()
{
	float32_t windowSizeX_1 = 1.f / g_app->m_mainWindow->m_clientSize.x;
	float32_t windowSizeY_1 = 1.f / g_app->m_mainWindow->m_clientSize.y;

	m_currentRect = m_creationRect;

	if (m_creationRect.x > 0.f) m_currentRect.x = m_creationRect.x / windowSizeX_1;
	if (m_creationRect.z > 0.f) m_currentRect.z = m_creationRect.z / windowSizeX_1;
	if (m_creationRect.y > 0.f) m_currentRect.y = m_creationRect.y / windowSizeY_1;
	if (m_creationRect.w > 0.f) m_currentRect.w = m_creationRect.w / windowSizeY_1;

	m_currentRect.x += AppViewportBorderSize;
	m_currentRect.y += AppViewportBorderSize;
	m_currentRect.z -= AppViewportBorderSize;
	m_currentRect.w -= AppViewportBorderSize;

	if (m_isOnLeftBorder) m_currentRect.x += g_leftPanelWidth;
	if (m_isOnRightBorder) m_currentRect.z -= g_rightPanelWidth;
	if (m_isOnTopBorder) m_currentRect.y += g_topPanelHeight;
	if (m_isOnBottomBorder) m_currentRect.w -= g_bottomPanelHeight;

	m_currentRectSize.x = m_currentRect.z - m_currentRect.x;
	m_currentRectSize.y = m_currentRect.w - m_currentRect.y;

	/*m_GUI_panel->m_buildArea = m_currentRect;
	m_GUI_panel->m_clipArea = m_currentRect;
	m_GUI_panel->m_sensorArea = m_currentRect;
	m_GUI_panel->UpdateTransform();*/
	UpdateAspect();

	/*printf("%f %f %f %f - %f\n",
		m_currentRect.x, m_currentRect.y, m_currentRect.z, m_currentRect.w, windowSizeX_1);*/
}

void AppViewport::UpdateAspect() {
	if (m_activeCamera)
		m_activeCamera->m_aspect = m_currentRectSize.x / m_currentRectSize.y;
}

void AppViewport::_frustum_cull(AppSceneObject* o)
{
	/*if (o != g_app->m_rootObject)
	{
		alVec4f center;
		o->GetAABBTransformed()->center(center);
		auto r = o->GetAABBTransformed()->radius();
		if (m_activeCamera->m_frust.PointInFrustum(*o->GetGlobalPosition()) || m_activeCamera->m_frust.PointInFrustum(center))
		{
			m_visibleObjects.push_back(o);
		}
		else if (m_activeCamera->m_frust.SphereInFrustum(r + r, center))
		{
			m_visibleObjects.push_back(o);
		}
	}


	auto node = o->GetChildren()->m_head;
	if (node)
	{
		auto last = node->m_left;
		while (true) {
			_frustum_cull(node->m_data);

			if (node == last)
				break;

			node = node->m_right;
		}
	}*/
}

void AppViewport::OnDrawUV()
{
	//static miMaterial m;
	//m.m_colorDiffuse.set(1.f, 0.f, 1.f, 1.f);
	//m.m_type = miMaterialType::Standart;
	//miSetMaterial(0);

	//g_app->m_gs->SetTexture(0, g_app->m_UVPlaneTexture);
	//if (g_app->m_selectedObjects.m_size)
	//{
	//	if (g_app->m_selectedObjects.m_data[0]->m_material)
	//	{
	//		if (g_app->m_selectedObjects.m_data[0]->m_material->m_maps[0].m_GPUTexture)
	//		{
	//			g_app->m_gs->SetTexture(0, (miGPUTexture*)g_app->m_selectedObjects.m_data[0]->m_material->m_maps[0].m_GPUTexture);
	//		}
	//	}
	//}

	//m_gs->UseDepth(false);
	//auto W = alMat4();
	//auto WVP = m_activeCamera->m_projectionMatrix * m_activeCamera->m_viewMatrix * W;
	//alLib::SetMatrix(alMatrixType::WorldViewProjection, &WVP);
	//alLib::SetMatrix(alMatrixType::World, &W);
	//m_gs->SetMesh(g_app->m_UVPlaneModel);
	//m_gs->Draw();

	//m_gs->UseDepth(false);
	//m_gs->DrawLine3D(alVec4f(), alVec4f(1.f, 0.f, 0.f, 0.f), ColorRed);
	//m_gs->DrawLine3D(alVec4f(), alVec4f(0.f, 0.f, 1.f, 0.f), ColorLime);

	//m_gs->UseDepth(true);

	//bool isUVGizmo = false;
	//for (uint32_t i = 0; i < g_app->m_selectedObjects.m_size; ++i)
	//{
	//	auto object = g_app->m_selectedObjects.m_data[i];

	//	object->OnDrawUV();

	//	if (object->IsUVSelected())
	//		isUVGizmo = true;
	//}

	//m_gs->UseDepth(false);
	//if (isUVGizmo)
	//{
	//	float32_t zoom = 1.f * m_activeCamera->m_positionPlatform.w;
	//	float32_t indent = 0.018f * zoom;

	//	//m_UVAabbMoveOffset
	//	alVec4f aabbMin = g_app->m_UVAabbOnClick.m_min;
	//	alVec4f aabbMax = g_app->m_UVAabbOnClick.m_max;

	//	auto p1 = alVec4f(aabbMin.x - indent, 0.f, aabbMin.z - indent, 0.f); // LT
	//	auto p2 = alVec4f(aabbMax.x + indent, 0.f, aabbMin.z - indent, 0.f); // RT
	//	auto p3 = alVec4f(aabbMin.x - indent, 0.f, aabbMax.z + indent, 0.f); // LB
	//	auto p4 = alVec4f(aabbMax.x + indent, 0.f, aabbMax.z + indent, 0.f); // RB

	//	auto p1_2 = (p1 + p2) * 0.5f; // Top
	//	auto p1_3 = (p1 + p3) * 0.5f; // Left
	//	auto p3_4 = (p3 + p4) * 0.5f; // Bottom
	//	auto p2_4 = (p2 + p4) * 0.5f; // Right
	//	auto pCenter = (p1 + p4) * 0.5f;

	//	auto dp1 = alVec4f(g_app->m_UVAabb.m_min.x - indent, 0.f, g_app->m_UVAabb.m_min.z - indent, 0.f); // LT
	//	auto dp2 = alVec4f(g_app->m_UVAabb.m_max.x + indent, 0.f, g_app->m_UVAabb.m_min.z - indent, 0.f); // RT
	//	auto dp3 = alVec4f(g_app->m_UVAabb.m_min.x - indent, 0.f, g_app->m_UVAabb.m_max.z + indent, 0.f); // LB
	//	auto dp4 = alVec4f(g_app->m_UVAabb.m_max.x + indent, 0.f, g_app->m_UVAabb.m_max.z + indent, 0.f); // RB

	//	auto dp1_2 = (dp1 + dp2) * 0.5f; // Top
	//	auto dp1_3 = (dp1 + dp3) * 0.5f; // Left
	//	auto dp3_4 = (dp3 + dp4) * 0.5f; // Bottom
	//	auto dp2_4 = (dp2 + dp4) * 0.5f; // Right
	//	auto dpCenter = (dp1 + dp4) * 0.5f;

	//	alVec4f moveOffset(g_app->m_UVAabbMoveOffset.x, 0.f, g_app->m_UVAabbMoveOffset.y, 0.f);

	//	m_gs->DrawLine3D(dp1 + moveOffset, dp2 + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1 + moveOffset, dp3 + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp3 + moveOffset, dp4 + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2 + moveOffset, dp4 + moveOffset, ColorRed);


	//	float32_t cornersSize = 0.003f * zoom;
	//	m_gs->DrawLine3D(dp1 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp1 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp1_2 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_2 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_2 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_2 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_2 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_2 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_2 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp1_2 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp2 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp2 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp3 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp3 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp1_3 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_3 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_3 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_3 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_3 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp1_3 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp1_3 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp1_3 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp3_4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3_4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3_4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3_4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3_4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp3_4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp3_4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp3_4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dp2_4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2_4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2_4 - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2_4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2_4 + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dp2_4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dp2_4 + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dp2_4 + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	m_gs->DrawLine3D(dpCenter - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dpCenter + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dpCenter - alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dpCenter + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dpCenter + alVec4f(-cornersSize, 0.f, cornersSize, 0.f) + moveOffset, dpCenter + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);
	//	m_gs->DrawLine3D(dpCenter + alVec4f(cornersSize, 0.f, -cornersSize, 0.f) + moveOffset, dpCenter + alVec4f(cornersSize, 0.f, cornersSize, 0.f) + moveOffset, ColorRed);

	//	if (g_app->m_viewportUnderCursor == this)
	//	{

	//		if (g_app->m_selectionFrust->PointInFrust(p1))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::SizeNWSE]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::LeftTop;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p2))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::SizeNESW]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::RightTop;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p3))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::SizeNESW]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::LeftBottom;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p4))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::SizeNWSE]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::RightBottom;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p1_2))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Rotate]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::Top;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p1_3))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Rotate]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::Left;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p3_4))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Rotate]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::Bottom;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(p2_4))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Rotate]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::Right;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else if (g_app->m_selectionFrust->PointInFrust(pCenter))
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Size]);
	//			if (g_app->m_inputContext->m_isLMBDown && g_app->m_activeViewportLayout->m_activeViewport == this)
	//			{
	//				g_app->m_gizmoModeUV = miGizmoUVMode::Center;
	//				g_app->OnGizmoUVClick();
	//			}
	//		}
	//		else
	//		{
	//			miSetCursor(miCursorType::Arrow, g_app->m_cursors[(uint32_t)miCursorType::Arrow]);
	//			g_app->m_gizmoModeUV = miGizmoUVMode::NoTransform;
	//		}
	//	}

	//	if (g_app->m_gizmoModeUV != miGizmoUVMode::NoTransform)
	//	{
	//		g_app->m_gizmoMode = miGizmoMode::Other;
	//	}
	//}
}

void AppViewport::OnDraw()
{
	//m_activeCamera->Update();
	//miSetEyePosition(&m_activeCamera->m_positionCamera);


	//g_app->m_gs->UseDepth(false);
	//m_gs->DrawRectangle(m_currentRect, g_app->m_color_windowClearColor, g_app->m_color_viewportColor);

	//m_gs->SetScissorRect(m_currentRect, g_app->m_mainWindow, 0);
	//m_gs->SetViewport(m_currentRect.x, m_currentRect.y, m_currentRectSize.x, m_currentRectSize.y, g_app->m_mainWindow, 0);

	//alLib::SetMatrix(alMatrixType::View, &m_activeCamera->m_viewMatrix);
	//alLib::SetMatrix(alMatrixType::Projection, &m_activeCamera->m_projectionMatrix);
	//alLib::SetMatrix(alMatrixType::ViewProjection, &m_activeCamera->m_viewProjectionMatrix);

	//// Прежде чем рисовать скорее всего лучше сделать сортировку и всё сохранить в массив
	////g_app->m_currentViewportDrawCamera = m_activeCamera;
	//g_app->m_currentViewportDraw = this;

	//if (m_viewportType == AppViewportType::UV)
	//{
	//	OnDrawUV();
	//}
	//else
	//{
	//	if (m_drawGrid)
	//		_drawGrid();

	//	g_app->m_gs->UseDepth(true);

	//	m_visibleObjects.clear();
	//	_frustum_cull(g_app->m_rootObject);

	//	//printf("%i\n", (int32_t)m_visibleObjects.m_size);

	//	g_app->m_gs->UseDepth(true);


	//	if (m_visibleObjects.m_size)
	//	{
	//		_drawSelectedObjectFrame();
	//		_drawScene();
	//		/*if (m_isDrawAabbs)
	//			g_app->DrawAabb(g_app->m_sceneAabb, alVec4f(1.f), v3f());*/
	//	}

	//	if (g_app->m_isClickAndDrag)
	//	{
	//		g_app->m_gs->UseDepth(false);
	//		m_gs->DrawLine3D(g_app->m_cursorLMBClickPosition3D, g_app->m_cursorPosition3D, ColorWhite);
	//		g_app->m_gs->UseDepth(true);
	//		//	printf("%f %f : %f %f\n", g_app->m_cursorLMBClickPosition3D.x, g_app->m_cursorLMBClickPosition3D.y,
	//	//		g_app->m_cursorPosition3D.x, g_app->m_cursorPosition3D.y);
	//	}
	//}
}

void AppViewport::ToggleDrawAABB() 
{
	m_isDrawAabbs = m_isDrawAabbs ? false : true;
}

void AppViewport::_drawScene() 
{
	auto dm = m_drawMode;

	for (uint32_t i = 0; i < m_visibleObjects.m_size; ++i)
	{
		/*auto object = m_visibleObjects.m_data[i];
		auto object_position = object->GetGlobalPosition();
		auto object_position_v4f = *object_position;

		object->m_worldViewProjection = m_activeCamera->m_projectionMatrix * m_activeCamera->m_viewMatrix * object->m_worldMatrix;
		object->OnUpdate(g_app->m_dt);

		m_gs->UseDepth(true);


		object->OnDraw(dm, g_app->m_editMode, g_app->m_dt);

		if (object->IsSelected())
		{
			if (m_isDrawAabbs)
				g_app->DrawAabb(*object->GetAABBTransformed(), *object->GetEdgeColor(), v3f());
		}*/
	}

	switch (g_app->m_editMode)
	{
	case AppEditMode::Object:
		if (g_app->m_selectedObjects.m_size)
			g_app->m_gizmo->Draw(this);
		break;
	default:
		/*if (g_app->m_isVertexEdgePolygonSelected)
			g_app->m_gizmo->Draw(this);*/
		break;
	}

}

void AppViewport::SetDrawMode(AppViewportDrawMode dm) 
{
	m_drawMode = dm;
}

void AppViewport::SetDrawGrid(bool v) {
	m_drawGrid = v;
}

void AppViewport::ToggleDrawModeMaterial() {
	static bool is_materail_mode = false;
	if (m_drawMode == AppViewportDrawMode::Wireframe)
	{
		if (is_materail_mode)
		{
			m_drawMode = AppViewportDrawMode::Material;
			is_materail_mode = false;
		}
		else
			m_drawMode = AppViewportDrawMode::MaterialWireframe;
	}
	else if (m_drawMode == AppViewportDrawMode::MaterialWireframe)
		m_drawMode = AppViewportDrawMode::Wireframe;
	else if (m_drawMode == AppViewportDrawMode::Material)
	{
		m_drawMode = AppViewportDrawMode::Wireframe;
		is_materail_mode = true;
	}
}
void AppViewport::ToggleDrawModeWireframe() {
	if (m_drawMode == AppViewportDrawMode::Material)
		m_drawMode = AppViewportDrawMode::MaterialWireframe;
	else if (m_drawMode == AppViewportDrawMode::MaterialWireframe)
		m_drawMode = AppViewportDrawMode::Material;
}

alVec4 AppViewport::GetCursorRayHitPosition(const alVec2f& cursorPosition) 
{
	alRay ray;

	g_app->GetRayFromScreen(&ray,
		cursorPosition,
		m_currentRect,
		m_activeCamera->m_viewProjectionInvertMatrix);

	alVec4 ip;

	bool isObject = false;

	/*
		check objects on scene here
	*/

	if (!isObject)
	{
		float64_t T = 0.f;
		float32_t U = 0.f;
		float32_t V = 0.f;
		float32_t W = 0.f;
		switch (m_cameraType)
		{
		default:
		case AppViewportCameraType::Top:
		case AppViewportCameraType::Perspective:
			// another way
			if (!m_rayTestTiangles[0].RayTest_MT(ray, true, T, U, V, W))
			{
				m_rayTestTiangles[1].RayTest_MT(ray, true, T, U, V, W);
			}
			break;
		case AppViewportCameraType::Bottom:
			ray.PlaneIntersection(alVec4(0.f, 0.f, 0.f, 1.f), alVec4f(0.f, -1.f, 0.f, 1.f), T);
			break;
		case AppViewportCameraType::Left:
			ray.PlaneIntersection(alVec4(0.f, 0.f, 0.f, 1.f), alVec4f(-1.f, 0.f, 0.f, 1.f), T);
			break;
		case AppViewportCameraType::Right:
			ray.PlaneIntersection(alVec4(0.f, 0.f, 0.f, 1.f), alVec4f(1.f, 0.f, 0.f, 1.f), T);
			break;
		case AppViewportCameraType::Front:
			ray.PlaneIntersection(alVec4(0.f, 0.f, 0.f, 1.f), alVec4f(0.f, 0.f, 1.f, 1.f), T);
			break;
		case AppViewportCameraType::Back:
			ray.PlaneIntersection(alVec4(0.f, 0.f, 0.f, 1.f), alVec4f(0.f, 0.f, -1.f, 1.f), T);
			break;
		}
		ray.GetIntersectionPoint(T, ip);

	}

	return ip;
}

void AppViewport::_drawGrid() 
{
	alLib::SetMatrix(alMatrixType::World, &g_emptyMatrix);

	static alMat4 WVP;
	WVP = m_activeCamera->m_projectionMatrix * m_activeCamera->m_viewMatrix * g_emptyMatrix;
	alLib::SetMatrix(alMatrixType::WorldViewProjection, &WVP);

	bool isCameraLowerThanWorld = false;
	if (m_activeCamera->m_positionCamera.y < 0.f)
		isCameraLowerThanWorld = true;

	switch (m_cameraType)
	{
	default:
	case AppViewportCameraType::Perspective:
		if (isCameraLowerThanWorld)
			m_gs->SetMesh(g_app->m_gridModel_perspective2);
		else
			m_gs->SetMesh(g_app->m_gridModel_perspective1);
		break;
	case AppViewportCameraType::Left: {
		bool front = ((m_activeCamera->m_rotationPlatform.y < PIPI) &&
			(m_activeCamera->m_rotationPlatform.y > PI));
		//if (m_activeCamera->m_positionPlatform.w < 40.f)
		front ? m_gs->SetMesh(g_app->m_gridModel_left1) : m_gs->SetMesh(g_app->m_gridModel_left2);
	}break;
	case AppViewportCameraType::Right: {
		bool front = ((m_activeCamera->m_rotationPlatform.y > 0.f) &&
			(m_activeCamera->m_rotationPlatform.y < PI));

		//if (m_activeCamera->m_positionPlatform.w < 40.f)
		front ? m_gs->SetMesh(g_app->m_gridModel_left1) : m_gs->SetMesh(g_app->m_gridModel_left2);
	}break;
	case AppViewportCameraType::Bottom:
	case AppViewportCameraType::Top:
		//if (m_activeCamera->m_positionPlatform.w < 40.f)
		isCameraLowerThanWorld ? m_gs->SetMesh(g_app->m_gridModel_top2) : m_gs->SetMesh(g_app->m_gridModel_top1);
		break;
	case AppViewportCameraType::Front: {
		bool front = ((m_activeCamera->m_rotationPlatform.y < PIHalf) &&
			(m_activeCamera->m_rotationPlatform.y > -PIPlusHalf));

		//if (m_activeCamera->m_positionPlatform.w < 40.f)
		front ? m_gs->SetMesh(g_app->m_gridModel_front1) : m_gs->SetMesh(g_app->m_gridModel_front2);
	}break;
	case AppViewportCameraType::Back: {
		//printf("%f\n", m_activeCamera->m_rotationPlatform.y);
		bool front = ((m_activeCamera->m_rotationPlatform.y > PIHalf) &&
			(m_activeCamera->m_rotationPlatform.y < PI + PIHalf));

		//if (m_activeCamera->m_positionPlatform.w < 40.f)
		front ? m_gs->SetMesh(g_app->m_gridModel_front1) : m_gs->SetMesh(g_app->m_gridModel_front2);
	}break;
	}

	////m_gs->DrawLine3D(alVec4f(), alVec4f(99999.f, 0.f, 0.f, 0.f), ColorRed);
	////m_gs->DrawLine3D(alVec4f(), alVec4f(0.f, 99999.f, 0.f, 0.f), ColorBlue);
	////m_gs->DrawLine3D(alVec4f(), alVec4f(0.f, 0.f, 99999.f, 0.f), ColorLime);

//	miSetMaterial(&g_app->m_gridModelMaterial);
	m_gs->EnableDepth();
	m_gs->Draw();
}

void AppViewport::_drawSelectedObjectFrame()
{
	float32_t frameSizeX = 0.f;
	float32_t frameSizeY = 0.f;
	float32_t frameSizeZ = 0.f;
	float32_t frameIndentX = 0.f;
	float32_t frameIndentY = 0.f;
	float32_t frameIndentZ = 0.f;

	//for (uint32_t i = 0; i < g_app->m_selectedObjects.m_size; ++i)
	//{
	//	auto obj = g_app->m_selectedObjects.m_data[i];

	//	auto aabb = *obj->GetAABBTransformed();
	//	aabb.m_max.w = 0.f;
	//	aabb.m_min.w = 0.f;

	//	if (g_app->m_editMode == miEditMode::Object)
	//	{
	//		aabb.m_max += g_app->m_gizmo->m_var_move;
	//		aabb.m_min += g_app->m_gizmo->m_var_move;
	//	}

	//	frameSizeX = (aabb.m_max.x - aabb.m_min.x) * 0.2f;
	//	frameSizeY = (aabb.m_max.y - aabb.m_min.y) * 0.2f;
	//	frameSizeZ = (aabb.m_max.z - aabb.m_min.z) * 0.2f;
	//	//frameSize /= 12.f;
	//	frameIndentX = frameSizeX * 0.2f;
	//	frameIndentY = frameSizeY * 0.2f;
	//	frameIndentZ = frameSizeZ * 0.2f;

	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z + frameSizeZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y + frameSizeY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x + frameSizeX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z - frameSizeZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y - frameSizeY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x - frameSizeX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z + frameSizeZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y - frameSizeY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x - frameSizeX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y + frameSizeY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x + frameSizeX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z - frameSizeZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x + frameSizeX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y - frameSizeY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_min.z + frameSizeZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x + frameSizeX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y - frameSizeY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_min.x - frameIndentX, aabb.m_max.y + frameIndentY, aabb.m_max.z - frameSizeZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x - frameSizeX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y + frameSizeY, aabb.m_min.z - frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z - frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_min.z + frameSizeZ, 0.f), ColorLightGray);

	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x - frameSizeX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y + frameSizeY, aabb.m_max.z + frameIndentZ, 0.f), ColorLightGray);
	//	m_gs->DrawLine3D(alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z + frameIndentZ, 0.f), alVec4f(aabb.m_max.x + frameIndentX, aabb.m_min.y - frameIndentY, aabb.m_max.z - frameSizeZ, 0.f), ColorLightGray);
	//}
}


void AppViewport::PanMove() {
	m_activeCamera->PanMove();
}

void AppViewport::Rotate(float32_t x, float32_t y) {
	if (m_viewportType != AppViewportType::Scene)
		return;
	m_activeCamera->Rotate(x, y);
}

void AppViewport::RotateZ() {
	if (m_viewportType != AppViewportType::Scene)
		return;
	m_activeCamera->RotateZ();
}

void AppViewport::Zoom() {
	m_activeCamera->Zoom();
}

void AppViewport::ChangeFOV() {
	if (m_viewportType != AppViewportType::Scene)
		return;
	m_activeCamera->ChangeFOV();
}

void AppViewport::ResetCamera()
{
	for (uint32_t i = 0; i < Camera_count_; ++i)
	{
		m_camera[i]->Reset();
	}
}

