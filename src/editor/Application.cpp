#include "editor.h"
#include <Windows.h>
#include <commctrl.h>
#include <filesystem>

Application* g_app = 0;
alMat4 g_emptyMatrix;
TOOLINFO g_toolTipInfo;
INT_PTR CALLBACK DialogProcAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void AppGUIButton::OnMouseEnter()
{
	auto input = alLib::GetInput();
	Application::GUI* gui = (Application::GUI*)GetUserData();

	g_app->m_toolTipText = L"...";
	switch (GetID())
	{
	case Application::GUI::elementID_btnCreateTypePoly:
		g_app->m_toolTipText = L"Polygonal Objects";
		break;
	case Application::GUI::elementID_btnCreateTypeHelp:
		g_app->m_toolTipText = L"Helper Objects";
		break;
	}
	g_app->ShowToolTip();
}

void AppGUIButton::OnMouseLeave()
{
	g_app->HideToolTip();
}


void AppGUICombo::OnComboSelectItem(size_t index)
{
	m_selected = index;
	uint8_t* ptr = (uint8_t*)m_items;
	m_text = (char32_t*)(&ptr[index * m_stride] + m_textOffset);
}

void AppGUIButtonIcon::OnButtonToggleOn()
{
	switch (GetID())
	{
	case Application::GUI::elementID_btnGizmoSelect:
		g_app->SetTransformMode(AppTransformMode::NoTransform);
		break;
	case Application::GUI::elementID_btnGizmoMove:
		g_app->SetTransformMode(AppTransformMode::Move);
		break;
	case Application::GUI::elementID_btnGizmoRotate:
		g_app->SetTransformMode(AppTransformMode::Rotate);
		break;
	case Application::GUI::elementID_btnGizmoScale:
		g_app->SetTransformMode(AppTransformMode::Scale);
		break;

	case Application::GUI::elementID_btnCreateAdd:
		g_app->SetRightTabMode(Application::RightTabMode::Create);
		break;
	case Application::GUI::elementID_btnObjectEdit:
		g_app->SetRightTabMode(Application::RightTabMode::Edit);
		break;
	case Application::GUI::elementID_btnObjectParameters:
		g_app->SetRightTabMode(Application::RightTabMode::Parameters);
		break;
	}
}
void AppGUIButtonIcon::OnMouseEnter()
{
	auto input = alLib::GetInput();
	Application::GUI* gui = (Application::GUI*)GetUserData();

	g_app->m_toolTipText = L"...";
	switch (GetID())
	{
	case Application::GUI::elementID_btnGizmoSelect:
		g_app->m_toolTipText = L"Select mode";
		break;
	case Application::GUI::elementID_btnGizmoMove:
		g_app->m_toolTipText = L"Move";
		break;
	case Application::GUI::elementID_btnGizmoRotate:
		g_app->m_toolTipText = L"Rotate";
		break;
	case Application::GUI::elementID_btnGizmoScale:
		g_app->m_toolTipText = L"Scale";
		break;
	case Application::GUI::elementID_btnCreateAdd:
		g_app->m_toolTipText = L"Create object";
		break;
	case Application::GUI::elementID_btnObjectEdit:
		g_app->m_toolTipText = L"Edit object";
		break;
	case Application::GUI::elementID_btnObjectParameters:
		g_app->m_toolTipText = L"Change object parameters";
		break;
	}
	g_app->ShowToolTip();
}
void AppGUIButtonIcon::OnMouseLeave()
{
	g_app->HideToolTip();
}
void Application::GUI::CreatePanels()
{
	m_panelCreate = m_context->GetNewPanel(alVec2f(), alVec2f(0,0));
	g_app->m_colorThemeCurr->m_GUIColorTheme2 = *alLib::GetDefaultColorTheme();
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg1_enabled = ColorTransparent;
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg2_enabled = ColorTransparent;
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg1_mouseHover = ColorRed;
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg2_mouseHover = ColorTransparent;
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg1_press = ColorLime;
	g_app->m_colorThemeCurr->m_GUIColorTheme2.m_button_bg2_press = ColorTransparent;

	AppGUIButton* btn = new AppGUIButton(m_context,
		alVec2f(0, 0), alVec2f(32, 32));
	btn->SetFont(g_app->m_fontGUIIcons32);
	char32_t s[2] = { 0x1, 0 };
	btn->SetText(s);
	btn->SetID(GUI::elementID_btnCreateTypePoly);
	btn->m_toggleButton = true;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme2;
	btn->m_radioGroup = AppGUIRadioGroupID_CreateOBJTYPE;
	btn->m_radioButton = true;
	btn->RadioCheck();
	m_panelCreate->AddElement(btn, true);
	btn = new AppGUIButton(m_context,
		alVec2f(32, 0), alVec2f(32, 32));
	btn->SetFont(g_app->m_fontGUIIcons32);
	s[0] = 0x2;
	btn->SetText(s);
	btn->SetID(GUI::elementID_btnCreateTypeHelp);
	btn->m_radioGroup = AppGUIRadioGroupID_CreateOBJTYPE;
	btn->m_radioButton = true;
	btn->m_toggleButton = true;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme2;
	m_panelCreate->AddElement(btn, true);
	/*AppGUICombo* combo = new AppGUICombo(m_context,
		alVec2f(0,10), alVec2f(g_rightPanelWidth,10));
	combo->SetFont(g_app->m_fontGUI);
	combo->SetID(AppGUIID_Combo_Create_Category);
	m_panelCreate->AddElement(combo, true);*/

	m_panelCreate->Rebuild();
}
void Application::GUI::CreateButtons()
{
	m_panel = m_context->GetNewPanel(alVec2f(), alVec2f());

	auto iconIDSel1 = m_ta->AddUV(alVec2u(0, 0), alVec2u(32, 32));
	auto iconIDMov1 = m_ta->AddUV(alVec2u(32 *1, 0), alVec2u(32 , 32));
	auto iconIDRot1 = m_ta->AddUV(alVec2u(32 *2, 0), alVec2u(32, 32));
	auto iconIDSc1 = m_ta->AddUV(alVec2u(32 *3, 0), alVec2u(32 , 32));
	auto iconIDSel2 = m_ta->AddUV(alVec2u(0, 64), alVec2u(32, 32));
	auto iconIDMov2 = m_ta->AddUV(alVec2u(32 * 1, 64), alVec2u(32, 32));
	auto iconIDRot2 = m_ta->AddUV(alVec2u(32 * 2, 64), alVec2u(32, 32));
	auto iconIDSc2 = m_ta->AddUV(alVec2u(32 * 3, 64), alVec2u(32, 32));
	
	auto iconIDAdd1 = m_ta->AddUV(alVec2u(0, 96), alVec2u(32, 32));
	auto iconIDParam1 = m_ta->AddUV(alVec2u(32 * 1, 96), alVec2u(32, 32));
	auto iconIDEdit1 = m_ta->AddUV(alVec2u(32 * 2, 96), alVec2u(32, 32));
	auto iconIDAdd2 = m_ta->AddUV(alVec2u(0, 128), alVec2u(32, 32));
	auto iconIDParam2 = m_ta->AddUV(alVec2u(32 * 1, 128), alVec2u(32, 32));
	auto iconIDEdit2 = m_ta->AddUV(alVec2u(32 * 2, 128), alVec2u(32, 32));

	float32_t position = 0.f;
	AppGUIButtonIcon* btn = new AppGUIButtonIcon(m_context, m_ta, iconIDSel1, alVec2f(position,0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnGizmoSelect);
	btn->m_toggleButton = true;	
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_GIZMOMODE;
	btn->m_iconIndexPress = iconIDSel2;
	btn->RadioCheck();
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	m_panel->AddElement(btn, true);
	position += 32;

	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDMov1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnGizmoMove);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_GIZMOMODE;
	btn->m_iconIndexPress = iconIDMov2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	m_panel->AddElement(btn, true);
	position += 32;

	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDRot1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnGizmoRotate);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_GIZMOMODE;
	btn->m_iconIndexPress = iconIDRot2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	m_panel->AddElement(btn, true);
	position += 32;

	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDSc1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnGizmoScale);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_GIZMOMODE;
	btn->m_iconIndexPress = iconIDSc2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	m_panel->AddElement(btn, true);
	position += 32;

	position = 0;
	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDEdit1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnObjectEdit);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_RIGHTPANELMODE;
	btn->m_iconIndexPress = iconIDEdit2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	btn->m_alignment = alGUIElementAlignment::RightTop;
	m_panel->AddElement(btn, true);
	position += 32;

	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDParam1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnObjectParameters);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_RIGHTPANELMODE;
	btn->m_iconIndexPress = iconIDParam2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	btn->m_alignment = alGUIElementAlignment::RightTop;
	m_panel->AddElement(btn, true);
	position += 32;

	btn = new AppGUIButtonIcon(m_context, m_ta, iconIDAdd1, alVec2f(position, 0), alVec2f(32.f, 32.f));
	btn->SetUserData(this);
	btn->SetID(elementID_btnCreateAdd);
	btn->m_toggleButton = true;
	btn->m_radioButton = true;
	btn->m_radioGroup = AppGUIRadioGroupID_RIGHTPANELMODE;
	btn->m_iconIndexPress = iconIDAdd2;
	btn->m_colorTheme = &g_app->m_colorThemeCurr->m_GUIColorTheme;
	btn->m_alignment = alGUIElementAlignment::RightTop;
	m_panel->AddElement(btn, true);
	btn->RadioCheck();
	position += 32;

	m_panel->SetPositionAndSize(0.f, 0.f, (float32_t)g_app->m_mainWindow->m_clientSize.x,
		32.f);
	m_panel->Rebuild();

	auto bg1 = alLib::GetDefaultColorTheme()->m_panel_bg1;
	alLib::GetDefaultColorTheme()->m_panel_bg1 = alLib::GetDefaultColorTheme()->m_panel_bg2;
	alLib::GetDefaultColorTheme()->m_panel_bg2 = bg1;
}

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
	m_input = alLib::GetInput();
	m_colorThemeCurr = &m_colorTheme;
}

Application::~Application()
{
	for (size_t i = 0; i < m_plugins.m_size; ++i)
	{
		m_plugins.m_data[i].m_unloadFunc();
	}

	AL_DESTROY(m_gui);
	AL_DESTROY(m_fontGUI);
	AL_DESTROY(m_fontGUIIcons32);


	AL_DESTROY(m_shortcutManager);
	AL_DESTROY(m_blackTexture);
	AL_DESTROY(m_transparentTexture);
	AL_DESTROY(m_shaderLineModel);
	AL_DESTROY(m_gs);
	AL_DESTROY(m_windowCallback);
	AL_DESTROY(m_pluginInterface);

	DestroyWindow(m_hwndTT);

	if (m_fileLog)
		fclose(m_fileLog);
}
void Application::HideToolTip()
{
	SendMessage(m_hwndTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolTipInfo);
	m_showToolTip = false;
	m_tooltipTimer = 0.f;
}
void Application::ShowToolTip()
{
	m_showToolTip = true;
}
void Application::ShowToolTip(int x, int y, const wchar_t* text)
{
	m_toolTipBuffer.clear();
	if (text)
	{
		m_toolTipBuffer = text;
		alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();

		POINT pt = { x, y };
		ClientToScreen(w32->m_hwnd, &pt);
		g_toolTipInfo.lpszText = m_toolTipBuffer.data();

		SendMessage(m_hwndTT, TTM_UPDATETIPTEXTW, (WPARAM)TRUE, (LPARAM)&g_toolTipInfo);
		SendMessage(m_hwndTT, TTM_SETDELAYTIME, (WPARAM)TTDT_INITIAL, (LPARAM)1000);
		SendMessage(m_hwndTT, TTM_SETDELAYTIME, (WPARAM)TTDT_RESHOW, (LPARAM)1000);
		SendMessage(m_hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolTipInfo);
		SendMessage(m_hwndTT, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
	}
}

bool Application::OnCreate(const char* videoDriver)
{
	alLog::SetPrintFunction(PrintLogFunction);
	alLog::PrintInfo("%s : %s\n", __DATE__, __TIME__);

	m_pluginInterface = alCreate<PluginInterfaceImpl>();

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
		case AppCursorType::HandGrab: m_cursors[i] = alLib::CreateCursor(alCursorType::Arrow, "../data/cursors/xuldll/grab.cur"); break;
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
	{
		HMENU menu_file = CreateMenu();
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_NEW_SCENE, L"New Scene");
		AppendMenu(menu_file, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_OPEN_SCENE, L"Open");
		AppendMenu(menu_file, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_SAVE_SCENE, L"Save");
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_SAVEAS_SCENE, L"Save As...");
		AppendMenu(menu_file, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_IMPORT, L"Import");
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_EXPORT, L"Export");
		AppendMenu(menu_file, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_file, MF_STRING, AppMenuID_FILE_EXIT, L"E&xit");

		HMENU menu_edit = CreateMenu();
		AppendMenu(menu_edit, MF_STRING, AppMenuID_EDIT_SELECTALL, L"Select All");
		AppendMenu(menu_edit, MF_STRING, AppMenuID_EDIT_INVERTSELECT, L"Invert Selection");

		HMENU menu_view = CreateMenu();
		AppendMenu(menu_view, MF_STRING, AppMenuID_VIEW_TOGGLEFULLVIEW, L"Toggle Full View");

		HMENU menu_create = CreateMenu();
		AppendMenu(menu_create, MF_STRING, 0, L"Plane");
		AppendMenu(menu_create, MF_STRING, 0, L"Cube");
		AppendMenu(menu_create, MF_STRING, 0, L"Sphere");

		HMENU menu_help = CreateMenu();
		AppendMenu(menu_help, MF_STRING, AppMenuID_HELP_ABOUT, L"About");
		AppendMenu(menu_help, MF_STRING, 0, L"Send feedback");

		HMENU mMainMenu = CreateMenu();
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)menu_file, L"&File");
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)menu_edit, L"&Edit");
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)menu_view, L"View");
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)menu_create, L"Create");
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)menu_help, L"Help");
		alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();
		SetMenu(w32->m_hwnd, mMainMenu);

		m_hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 0, 0, 0, 0, w32->m_hwnd, NULL, 0, NULL);

		wchar_t text[] = L"alshlkaskhlaks lfasknlaksf asfas";

		memset(&g_toolTipInfo, 0, sizeof(g_toolTipInfo));
		g_toolTipInfo.cbSize = sizeof(TTTOOLINFO);
		g_toolTipInfo.uFlags = TTF_SUBCLASS;
		g_toolTipInfo.hwnd = 0;
		g_toolTipInfo.uId = 0;
		g_toolTipInfo.lpszText = text;
		GetClientRect(w32->m_hwnd, &g_toolTipInfo.rect);

		if (!SendMessage(m_hwndTT, TTM_ADDTOOL, 0, (LPARAM)&g_toolTipInfo))
			MessageBox(0, TEXT("Failed: TTM_ADDTOOL"), 0, 0);
	}
	m_mainWindow->Show();

	m_gs = alLib::CreateGS(alVideoDriverType::Direct3D11);
	if (!m_gs->Init(m_mainWindow))
		return false;
	{
		m_shaderLineModel = alCreate<AppGSShaderCallback_LineModel3D>();
		if (!m_shaderLineModel->Create(m_gs))
			return false;
	}

	alLib::InitializeDefaultFont(m_gs);
	m_fontGUI = alLib::CreateGUIFont();
	m_fontGUI->Load("../data/font.zip", m_gs);

	m_fontGUIIcons32 = alLib::CreateGUIFont();
	m_fontGUIIcons32->Load("../data/gui/guiicons.zip", m_gs);

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

	m_shortcutManager = alCreate<AppShortcutManager>();
	m_whiteTexture = m_gs->GetWhiteTexture();

	m_gui = alCreate<GUI>();
	m_gui->m_context = alLib::CreateGUIContext(m_mainWindow, m_gs);
	{
		alImage* img = alLib::LoadALImage("../data/gui/gui.png");
		if (img)
		{
			m_gui->m_taTexture = m_gs->CreateTexturePoint(img);
			alDestroy(img);
		}
	}
	
	m_gui->m_ta = alCreate<alGUITextureAtlas>(
		m_gui->m_taTexture ? m_gui->m_taTexture : m_whiteTexture);
	m_gui->CreateButtons();
	m_gui->CreatePanels();

	m_colorThemeCurr->m_GUIColorTheme.m_buttonIcon_enabled = ColorWhite;
	m_colorThemeCurr->m_GUIColorTheme.m_buttonIcon_mouseHover = ColorYellow;
	m_colorThemeCurr->m_GUIColorTheme.m_buttonIcon_press = ColorWhite;

//	_callViewportOnWindowSize();

	_initPlugins();
	for (size_t i = 0; i < m_plugins.m_size; ++i)
	{
		auto plugin = m_plugins.m_data[i];
		auto pluginType = plugin.m_plugin->PluginType();
		if (alLib::GUIDIsEqual(pluginType, PLUGIN_CLASS_ID_PLUGIN_TYPE_OBJECT))
		{
			PluginObject* po = dynamic_cast<PluginObject*>(plugin.m_plugin);
			if (po)
			{
				const char32_t* cat = po->Category();
				auto objType = po->ObjectType();
				if (cat && objType != PluginObject::EObjectType::EObjectType__end)
				{
					auto* otData = &m_new_object_basic_data.m_data[objType];
					
					Application::new_object_basic_data::_object_category* category = 0;
					for (size_t ci = 0; ci < otData->m_categories.m_size; ++ci)
					{
						category = &otData->m_categories.m_data[ci];
						if (alLib::strcmp(category->m_name, cat) == 0)
							break;
						category = 0;
					}

					if (!category)
					{
						Application::new_object_basic_data::_object_category newCategory;
						alLib::snprintf(
							newCategory.m_name, 
							Application::new_object_basic_data::NAME_SIZE,
							U"%s", cat);
						otData->m_categories.push_back(newCategory);
						category = &otData->m_categories
							.m_data[otData->m_categories.m_size - 1];
					}
				}
			}
		}
	}
	/*if(m_new_object_basic_data.m_categories.m_size)
	{
		auto e = m_gui->m_panelCreate->GetElementByID(AppGUIID_Combo_Create_Category);
		if (e)
		{
			alGUIComboBox* combo = dynamic_cast<alGUIComboBox*>(e);
			if (combo)
			{
				combo->SetItems(
					m_new_object_basic_data.m_categories.m_data,
					m_new_object_basic_data.m_categories.m_size,
					sizeof(new_object_basic_data::new_object_category),
					0);
				combo->OnComboSelectItem(0);
				combo->Rebuild();
			}
		}
	}*/
	OnWindowSizeChanged();
	return true;
}

//void Application::OnCombo_Create_Category(uint32_t index)
//{
//	
//}

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
	auto input = alLib::GetInput();
	while (m_run)
	{
	//	SendMessage(m_hwndTT, TTM_UPDATE, (WPARAM)TRUE, (LPARAM)&g_toolTipInfo);

		alLib::Update();
		m_currentCursor = AppCursorType::Arrow;

		m_dt = *dt;
		m_isCursorMove = (input->m_mouseDelta.x != 0.f) || (input->m_mouseDelta.y != 0.f);

		WaitForSingleObject(currThread, 10);
		
		// for `in place tooltip`
		// default delay is not working
		// make delay here
		if (m_showToolTip)
		{
			m_tooltipTimer += m_dt;
			if (m_tooltipTimer > 1.f)
			{
				ShowToolTip(input->m_cursorCoords.x, input->m_cursorCoords.y, g_app->m_toolTipText);
				m_showToolTip = false;
				m_tooltipTimer = 0.f;
			}
		}

		m_gui->m_context->Update(m_dt);

		UpdateViewports();
		ProcessShortcuts3D();

		m_gs->BeginDraw();
		m_gs->SetClearColor(m_colorThemeCurr->m_windowClearColor);
		m_gs->ClearAll();

		DrawViewports3D();

		m_gs->BeginDrawGUI();
		DrawViewports();
		//m_gs->BeginDrawGUI(false);
		m_gui->m_context->Draw(m_dt);

		//m_GUI->m_context->DrawAll();
		m_gs->EndDrawGUI();

		m_gs->EndDraw();
		m_gs->SwapBuffers();

	//	OnSetCursor();
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
	if (m_gui)
	{
		m_gui->m_panelCreate->SetPositionAndSize(
			m_mainWindow->m_clientSize.x - g_rightPanelWidth,
			g_topPanelHeight,
			g_rightPanelWidth,
			(float32_t)m_mainWindow->m_clientSize.y);
		m_gui->m_panelCreate->Rebuild();

		m_gui->m_panel->SetPositionAndSize(0.f,0.f, (float32_t)m_mainWindow->m_clientSize.x, 32.f);
		m_gui->m_panel->Rebuild();
	}

	if (m_gs)
	{
		m_gs->UpdateWindowData();
	}
	if (m_activeViewportLayout)
	{
		_callViewportOnWindowSize();
	}

}

void Application::ViewportToggleFullView()
{
	/*if (m_editorType != miEditorType::_3D)
		return;*/

	if (m_activeViewportLayout == m_viewportLayouts[AppViewportLayout_Full])
	{
		m_activeViewportLayout->HideGUI();
		m_activeViewportLayout = m_previousViewportLayout;
		m_activeViewportLayout->m_activeViewport->Copy(m_viewportLayouts[AppViewportLayout_Full]->m_activeViewport);
		m_activeViewportLayout->ShowGUI();
	}
	else
	{
		m_previousViewportLayout = m_activeViewportLayout;
		m_activeViewportLayout->HideGUI();
		m_activeViewportLayout = m_viewportLayouts[AppViewportLayout_Full];
		m_activeViewportLayout->m_activeViewport->Copy(m_previousViewportLayout->m_activeViewport);
		m_activeViewportLayout->ShowGUI();
	}
//	m_GUI->m_context->NeedRebuild();
	m_activeViewportLayout->m_activeViewport->UpdateAspect();
}

void Application::ProcessShortcuts3D()
{
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_cameraReset)) this->CameraReset();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_cameraMoveToSelection)) this->CameraMoveToSelection();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewPerspective)) this->ViewportChangeView(miViewportCameraType::Perspective);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewTop)) this->ViewportChangeView(miViewportCameraType::Top);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewBottom)) this->ViewportChangeView(miViewportCameraType::Bottom);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewLeft)) this->ViewportChangeView(miViewportCameraType::Left);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewRight)) this->ViewportChangeView(miViewportCameraType::Right);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewFront)) this->ViewportChangeView(miViewportCameraType::Front);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewBack)) this->ViewportChangeView(miViewportCameraType::Back);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleGrid)) this->ViewportToggleGrid();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleFullView)) this->ViewportToggleFullView();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmMaterial)) this->ViewportSetDrawMode(miViewportDrawMode::Material);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmMaterialWireframe)) this->ViewportSetDrawMode(miViewportDrawMode::MaterialWireframe);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmWireframe)) this->ViewportSetDrawMode(miViewportDrawMode::Wireframe);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleDMMaterial)) this->ViewportToggleDrawMaterial();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleDMWireframe)) this->ViewportToggleDrawWireframe();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_NoTransform)) this->SetTransformMode(miTransformMode::NoTransform);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Move)) this->SetTransformMode(miTransformMode::Move);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Scale)) this->SetTransformMode(miTransformMode::Scale);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Rotate)) this->SetTransformMode(miTransformMode::Rotate);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::select_selectAll)) this->SelectAll();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::select_deselectAll)) this->DeselectAll();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::select_invertSelection)) this->InvertSelection();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::editMode_Vertex)) this->ToggleEditMode(miEditMode::Vertex);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::editMode_Edge)) this->ToggleEditMode(miEditMode::Edge);
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::editMode_Polygon)) this->ToggleEditMode(miEditMode::Polygon);
}

void Application::OnSetCursor()
{
	m_cursors[(uint32_t)m_currentCursor]->Activate();
}

void Application::SetTransformMode(AppTransformMode mode)
{
	m_transformMode = mode;
}

void Application::OnPopupCommand(uint32_t cmd)
{
	switch (cmd)
	{
	case AppMenuID_FILE_EXIT:
		CloseProgramm();
		break;
	case AppMenuID_HELP_ABOUT:
		ShowAbout();
		break;
	}
}

void Application::CloseProgramm()
{
	m_run = false;
}

void Application::ShowAbout()
{
	alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();
	auto hdlg = CreateDialog(
		GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_PROPPAGE_SMALL),
		w32->m_hwnd,
		DialogProcAbout);
	ShowWindow(hdlg, SW_SHOW);
}

void Application::_initPlugins()
{
	for (auto& entry : std::filesystem::directory_iterator(L"plugins/"))
	{
		auto path = entry.path();
		if (!path.has_extension())
			continue;

		auto ex = path.extension();
		if (ex != ".dll")
			continue;

		auto lib_str = path.generic_string();

		auto module = alLib::DLLLoad(lib_str.c_str());
		if (!module)
			continue;

		alLog::PrintInfo("Load plugin: %s...\n", lib_str.data());
		const char* funcName_load = "PluginLoad";
		const char* funcName_unload = "PluginUnload";
		PluginLoad_t PluginLoad = (PluginLoad_t)alLib::DLLGetProc(funcName_load, module);
		if (!PluginLoad)
		{
			alLog::PrintInfo("FAIL (function %s not found)\n", funcName_load);
			continue;
		}
		PluginUnload_t PluginUnload = (PluginUnload_t)alLib::DLLGetProc(funcName_unload, module);
		if (!PluginUnload)
		{
			alLog::PrintInfo("FAIL (function %s not found)\n", funcName_load);
			continue;
		}


		auto newPlugin = PluginLoad(m_pluginInterface);
		if (newPlugin)
		{
			if (newPlugin->SDKVersion() != PLUGIN_SDK_VERSION)
			{
				alDestroy(newPlugin);
				alLog::PrintError("FAIL (bad version)\n");
				continue;
			}

			alLog::PrintInfo("\t(%s)\n", alUnicodeString(newPlugin->Name()).GetStringA().c_str());
			alLog::PrintInfo("\t(%s)\n", alUnicodeString(newPlugin->Desc()).GetStringA().c_str());
			alLog::PrintInfo("\t\t(%s)\n", alUnicodeString(newPlugin->Author()).GetStringA().c_str());
			alLog::PrintInfo("\t\t((c) %s)\n", alUnicodeString(newPlugin->Copyright()).GetStringA().c_str());

			plugin_info pi;
			pi.m_plugin = newPlugin;
			pi.m_path = path.filename().generic_string().c_str();
			pi.m_unloadFunc = PluginUnload;

			m_plugins.push_back(pi);
		}
		else
		{
			alLog::PrintInfo("FAIL (nullptr)\n");
		}
	}
}

void Application::SetRightTabMode(Application::RightTabMode mode)
{
	switch (mode)
	{
	case Application::RightTabMode::Create:
		m_gui->m_panelCreate->SetVisible(true);
		break;
	case Application::RightTabMode::Edit:
		m_gui->m_panelCreate->SetVisible(false);
		break;
	case Application::RightTabMode::Parameters:
		m_gui->m_panelCreate->SetVisible(false);
		break;
	default:
		break;
	}
}

void Application::SetPanelCreateObjectType(PluginObject::EObjectType type)
{
	
	switch (type)
	{
	case PluginObject::EObjectType_Polygonal:
	default:
	{
		/*AppGUIButton* btnPoly = dynamic_cast<AppGUIButton*>(m_gui->m_panelCreate->GetElementByID(AppGUIID_BTN_Create_Type_Poly));
		AppGUIButton* btnHelp = dynamic_cast<AppGUIButton*>(m_gui->m_panelCreate->GetElementByID(AppGUIID_BTN_Create_Type_Help));
		if (btnPoly && btnHelp)
		{
			btnPoly->
		}*/
	}
		break;
	case PluginObject::EObjectType_Helper:
		break;
	case PluginObject::EObjectType__end:
		break;
	}
}
