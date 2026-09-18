#include "editor.h"
#include "System/alSystemPopup.h"

#include <filesystem>

#pragma comment(lib, "editor.lib.lib")

Application* g_app = 0;
alMat4 g_emptyMatrix;
TOOLINFO g_toolTipInfo;
INT_PTR CALLBACK DialogProcAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProcObjectList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void AppGUIListBox::OnListSelectItem(size_t index)
{
	
	uint8_t* ptr = (uint8_t*)m_items;
	uint32_t* flags = (uint32_t*)(&ptr[index * m_stride] + m_flagsOffset);
	if (*flags & flag_selected)
	{
		*flags &= ~flag_selected;
	}
	else
	{
		DeselectAll();
		*flags |= flag_selected;
	}

	switch (GetID())
	{
	case Application::GUI::elementID_lbCreate:
	{
		uint8_t* ptr = (uint8_t*)m_items;
		Application::new_object_basic_data::_object* object = (Application::new_object_basic_data::_object*)
			(&ptr[index * m_stride]);

		g_app->_onLBSelect_createPanel(object->m_pluginObject);
	}
		break;
	}
}

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
	case Application::GUI::elementID_btnCreate_CreateButton:
		g_app->m_toolTipText = L"Create";
		break;
	}
	g_app->ShowToolTip();
}

void AppGUIButton::OnMouseLeave()
{
	g_app->HideToolTip();
}

void AppGUIButton::OnButtonRelease()
{
	switch (GetID())
	{
	case Application::GUI::elementID_btnCreate_CreateButton:
		g_app->OnButtonCreateNewObject();
		break;
	}
}

void AppGUIButton::OnButtonToggleOn()
{
	switch (GetID())
	{
	case Application::GUI::elementID_btnCreateTypePoly:
		g_app->SetPanelCreateObjectType(AppPluginObject::EObjectType::EObjectType_Polygonal);
		break;
	case Application::GUI::elementID_btnCreateTypeHelp:
		g_app->SetPanelCreateObjectType(AppPluginObject::EObjectType::EObjectType_Helper);
		break;
	}
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

	AppGUIListBox* lb = new AppGUIListBox(m_context,
		alVec2f(0, 30), alVec2f(g_rightPanelWidth, 300));
	lb->SetFont(g_app->m_fontGUI);
	lb->SetID(GUI::elementID_lbCreate);
	m_panelCreate->AddElement(lb, true);
	/*AppGUICombo* combo = new AppGUICombo(m_context,
		alVec2f(0,10), alVec2f(g_rightPanelWidth,10));
	combo->SetFont(g_app->m_fontGUI);
	combo->SetID(AppGUIID_Combo_Create_Category);
	m_panelCreate->AddElement(combo, true);*/
	btn = new AppGUIButton(m_context,
		alVec2f(35, 10), alVec2f(150, 32));
	btn->SetFont(g_app->m_fontGUI);
	btn->SetText(U"Create");
	btn->SetID(GUI::elementID_btnCreate_CreateButton);
	btn->m_alignment = alGUIElementAlignment::RightBottom;
	btn->SetVisible(false);
	m_panelCreate->AddElement(btn, true);

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
	AL_DESTROY(m_scene);

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
	AL_DESTROY(m_shaderPointModel);
	AL_DESTROY(m_shaderDefaultTriangle);
	AL_DESTROY(m_gs);
	AL_DESTROY(m_windowCallback);
	AL_DESTROY(m_pluginInterface);

	DestroyWindow(m_hwndTT);

	if (m_hwnd_About)
		DestroyWindow(m_hwnd_About);

	if (m_objectListWindowData.m_hwnd_ObjectList)
		DestroyWindow(m_objectListWindowData.m_hwnd_ObjectList);

	if (m_objectListWindowData.m_hImgList_treeView)
		ImageList_Destroy(m_objectListWindowData.m_hImgList_treeView);
	
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

	m_pluginInterface = alCreate<AppPluginInterfaceImpl>(this);

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

		HMENU menu_edit_transformation_mode = CreateMenu();
		AppendMenu(menu_edit_transformation_mode, MF_STRING, AppMenuID_EDIT_SETTRANSMODE_SELECT, L"Selection Mode");
		AppendMenu(menu_edit_transformation_mode, MF_STRING, AppMenuID_EDIT_SETTRANSMODE_MOVE, L"Move");
		AppendMenu(menu_edit_transformation_mode, MF_STRING, AppMenuID_EDIT_SETTRANSMODE_ROTATE, L"Rotate");
		AppendMenu(menu_edit_transformation_mode, MF_STRING, AppMenuID_EDIT_SETTRANSMODE_SCALE, L"Scale");

		HMENU menu_edit = CreateMenu();
		AppendMenu(menu_edit, MF_STRING | MF_POPUP, (UINT)menu_edit_transformation_mode, L"Transform Mode");
		AppendMenu(menu_edit, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_edit, MF_STRING, AppMenuID_EDIT_SELECTALL, L"Select All");
		AppendMenu(menu_edit, MF_STRING, AppMenuID_EDIT_INVERTSELECT, L"Invert Selection");
		AppendMenu(menu_edit, MF_SEPARATOR, 0, 0);
		AppendMenu(menu_edit, MF_STRING, AppMenuID_EDIT_OBJECTLISTWINDOW, L"Open Objects List");

		m_winMenu.m_menu_view = CreateMenu();
		AppendMenu(m_winMenu.m_menu_view, MF_STRING, AppMenuID_VIEW_TOGGLEFULLVIEW, L"Toggle Full View");
	

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
		AppendMenu(mMainMenu, MF_STRING | MF_POPUP, (UINT)m_winMenu.m_menu_view, L"View");
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
	{
		m_shaderPointModel = alCreate<AppGSShaderCallback_PointModel>();
		if (!m_shaderPointModel->Create(m_gs))
			return false;
	}
	{
		m_shaderDefaultTriangle = alCreate<AppGSShaderCallback_DefaultTriangle>();
		if (!m_shaderDefaultTriangle->Create(m_gs))
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

	m_scene = new AppScene();
	
	AppString pstr;

	_initPlugins();
	for (size_t i = 0; i < m_plugins.m_size; ++i)
	{
		auto plugin = m_plugins.m_data[i];
		auto objectNum = plugin.m_plugin->GetObjectPluginNum();
		for (uint32_t o = 0; o < objectNum; ++o)
		{
			auto po = plugin.m_plugin->GetObjectPlugin(o);
			const char32_t* cat = po->Category();
			const char32_t* title = po->TitleName();
			auto objType = po->ObjectType();
			if (cat && objType != AppPluginObject::EObjectType::EObjectType__end)
			{
				auto* otData = &m_new_object_basic_data.m_data[objType];
				Application::new_object_basic_data::_object obj;
				alLib::snprintf(
					obj.m_name,
					Application::new_object_basic_data::NAME_SIZE,
					U"%s:%s", cat, title);
				obj.m_pluginObject = po;
				otData->m_objs.push_back(obj);
			}
		}
		//auto pluginType = plugin.m_plugin->PluginType();
		//if (alLib::GUIDIsEqual(pluginType, APP_CLASS_ID_PLUGIN_TYPE_OBJECT))
		//{
		//	AppPluginObject* po = dynamic_cast<AppPluginObject*>(plugin.m_plugin);
		//	if (po)
		//	{
		//		const char32_t* cat = po->Category();
		//		const char32_t* title = po->TitleName();
		//		auto objType = po->ObjectType();
		//		if (cat && objType != AppPluginObject::EObjectType::EObjectType__end)
		//		{
		//			auto* otData = &m_new_object_basic_data.m_data[objType];
		//			
		//			// ПОКА БУДЕТ НЕ ТАК
		//			/*Application::new_object_basic_data::_object* category = 0;
		//			for (size_t ci = 0; ci < otData->m_objs.m_size; ++ci)
		//			{
		//				category = &otData->m_objs.m_data[ci];
		//				if (alLib::strcmp(category->m_name, cat) == 0)
		//					break;
		//				category = 0;
		//			}

		//			if (!category)
		//			{
		//				Application::new_object_basic_data::_object newCategory;
		//				alLib::snprintf(
		//					newCategory.m_name, 
		//					Application::new_object_basic_data::NAME_SIZE,
		//					U"%s", cat);
		//				otData->m_objs.push_back(newCategory);
		//				category = &otData->m_objs
		//					.m_data[otData->m_objs.m_size - 1];
		//			}*/

		//			// ПОКА БУДЕТ ПРОСТОЙ СПИСОК
		//			Application::new_object_basic_data::_object obj;
		//			alLib::snprintf(
		//				obj.m_name,
		//				Application::new_object_basic_data::NAME_SIZE,
		//				U"%s:%s", cat, title);
		//			obj.m_pluginObject = po;
		//			otData->m_objs.push_back(obj);
		//		}
		//	}
		//}
	}
	SetRightTabMode(Application::RightTabMode::Create);
	SetPanelCreateObjectType(AppPluginObject::EObjectType::EObjectType_Polygonal);
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

	InitCommonControls();
	
	alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();
	{
		m_hwnd_testDlg = CreateDialog(
			GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOGBAR),
			w32->m_hwnd,
			DialogProcObjectList);

		ShowWindow(m_hwnd_testDlg, SW_SHOW);
		RECT rc;
		GetClientRect(m_hwnd_testDlg, &rc);
		MoveWindow(m_hwnd_testDlg, m_mainWindow->m_clientSize.x-g_rightPanelWidth,
			g_topPanelHeight, g_rightPanelWidth, 400, TRUE);
		//void* buffer = 0;
		//BITMAPINFO bmi;
		//memset(&bmi, 0, sizeof(BITMAPINFO));
		//bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//bmi.bmiHeader.biWidth = rc.right - rc.left;
		//bmi.bmiHeader.biHeight = -(rc.bottom - rc.top);
		//bmi.bmiHeader.biPlanes = 1;
		//bmi.bmiHeader.biBitCount = 32;
		//bmi.bmiHeader.biCompression = BI_RGB;
		//HDC hdcSrc = GetDC(m_hwnd_testDlg);
		//HDC hdcMem = CreateCompatibleDC(hdcSrc);

		//m_bitmap_testDlg = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS,
		//	&buffer, 0, 0);
		//if (!m_bitmap_testDlg)
		//{
		//	ReleaseDC(m_hwnd_testDlg, hdcSrc);
		//	DeleteDC(hdcMem);
		//}
		//else
		//{

		///*	BitBlt(dc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight,
		//		);*/
		//}
	}

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

		// if cursor not in veiwport, then in GUI
		m_isCursorInGUI = true;

		m_isCursorInWindow = false;
		if (alMath::PointInRect(m_input->m_cursorCoords.x, m_input->m_cursorCoords.y,
			alVec4f(0.f, 0.f, (float32_t)m_mainWindow->m_clientSize.x, (float32_t)m_mainWindow->m_clientSize.y)))
		{
			m_isCursorInWindow = true;
		}
		
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
		DrawViewportsGUI();
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
			m_gridModel_perspective1 = m_gs->CreateMesh(mesh);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -5.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 5, AppViewportCameraType::Perspective);

		{
			m_gridModel_perspective2 = m_gs->CreateMesh(mesh);
		}
		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorLime, ColorRed, 251, AppViewportCameraType::Top);

		{
			m_gridModel_top1 = m_gs->CreateMesh(mesh);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Top);

		{
			m_gridModel_top2 = m_gs->CreateMesh(mesh);
		}
		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorBlue, ColorRed, 251, AppViewportCameraType::Front);

		{
			m_gridModel_front1 = m_gs->CreateMesh(mesh);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Front);

		{
			m_gridModel_front2 = m_gs->CreateMesh(mesh);
		}

		AL_DESTROY(mesh);
	}
	{
		const int32_t linesNum = 1002;
		auto mesh = _get_mesh(linesNum);

		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, ColorBlue, ColorLime, 251, AppViewportCameraType::Left);

		{
			m_gridModel_left1 = m_gs->CreateMesh(mesh);
		}

		colorBase = alColor(150, 150, 150, 255);
		_build(mesh, -251.f, linesNum, 1.f, colorBase, colorBase, colorBase, colorBase, 251, AppViewportCameraType::Left);

		{
			m_gridModel_left2 = m_gs->CreateMesh(mesh);
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
		/*m_gui->m_panelCreate->SetPositionAndSize(
			m_mainWindow->m_clientSize.x - g_rightPanelWidth,
			g_topPanelHeight,
			g_rightPanelWidth,
			(float32_t)m_mainWindow->m_clientSize.y - g_topPanelHeight);*/
		m_gui->m_panelCreate->m_position.x = m_mainWindow->m_clientSize.x - g_rightPanelWidth;
		m_gui->m_panelCreate->m_position.y = g_topPanelHeight;
		m_gui->m_panelCreate->m_size.x = g_rightPanelWidth;
		m_gui->m_panelCreate->m_size.y = (float32_t)m_mainWindow->m_clientSize.y - g_topPanelHeight;
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

void Application::ViewportCameraReset()
{
	m_activeViewportLayout->m_activeViewport->m_activeCamera->Reset();
	m_activeViewportLayout->m_activeViewport->UpdateAspect();
}

void Application::ViewportToggleGrid()
{
	auto old = m_activeViewportLayout->m_activeViewport->m_drawGrid;
	m_activeViewportLayout->m_activeViewport->m_drawGrid = old ? false : true;
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
		CheckMenuItem(m_winMenu.m_menu_view, AppMenuID_VIEW_TOGGLEFULLVIEW, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		m_previousViewportLayout = m_activeViewportLayout;
		m_activeViewportLayout->HideGUI();
		m_activeViewportLayout = m_viewportLayouts[AppViewportLayout_Full];
		m_activeViewportLayout->m_activeViewport->Copy(m_previousViewportLayout->m_activeViewport);
		m_activeViewportLayout->ShowGUI();

		CheckMenuItem(m_winMenu.m_menu_view, AppMenuID_VIEW_TOGGLEFULLVIEW, MF_BYCOMMAND | MF_CHECKED);
	}
//	m_GUI->m_context->NeedRebuild();
	m_activeViewportLayout->m_activeViewport->UpdateAspect();
}

void Application::ViewportCameraMoveToSelection()
{
	m_activeViewportLayout->m_activeViewport->m_activeCamera->MoveToSelection();
	m_activeViewportLayout->m_activeViewport->UpdateAspect();
}

void Application::ViewportChangeView(AppViewportCameraType ct)
{
	m_activeViewportLayout->m_activeViewport->SetCameraType(ct);
	m_activeViewportLayout->m_activeViewport->m_activeCamera->Reset();
}

void Application::ViewportToggleOrtho()
{
	m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho =
		m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho ? false : true;
	m_activeViewportLayout->m_activeViewport->Rotate(0.f, 0.f);
}

void Application::ViewportSetDrawMode(AppViewportDrawMode dm)
{
	m_activeViewportLayout->m_activeViewport->SetDrawMode(dm);
}

void Application::ViewportToggleAABB()
{
	m_activeViewportLayout->m_activeViewport->ToggleDrawAABB();
}

void Application::ProcessShortcuts3D()
{
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_cameraReset)) this->ViewportCameraReset();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_cameraMoveToSelection)) this->ViewportCameraMoveToSelection();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewPerspective)) this->ViewportChangeView(AppViewportCameraType::Perspective);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewTop)) this->ViewportChangeView(AppViewportCameraType::Top);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewBottom)) this->ViewportChangeView(AppViewportCameraType::Bottom);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewLeft)) this->ViewportChangeView(AppViewportCameraType::Left);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewRight)) this->ViewportChangeView(AppViewportCameraType::Right);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewFront)) this->ViewportChangeView(AppViewportCameraType::Front);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_viewBack)) this->ViewportChangeView(AppViewportCameraType::Back);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleGrid)) this->ViewportToggleGrid();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleFullView)) this->ViewportToggleFullView();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleOrtho)) this->ViewportToggleOrtho();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmMaterial)) this->ViewportSetDrawMode(AppViewportDrawMode::Material);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmMaterialWireframe)) this->ViewportSetDrawMode(AppViewportDrawMode::MaterialWireframe);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_dmWireframe)) this->ViewportSetDrawMode(AppViewportDrawMode::Wireframe);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleDrawAABB)) this->ViewportToggleAABB();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleDMMaterial)) this->ViewportToggleDrawMaterial();
	//if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::viewport_toggleDMWireframe)) this->ViewportToggleDrawWireframe();
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_NoTransform)) this->SetTransformMode(AppTransformMode::NoTransform);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Move)) this->SetTransformMode(AppTransformMode::Move);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Scale)) this->SetTransformMode(AppTransformMode::Scale);
	if (m_shortcutManager->IsShortcutActive(AppShortcutCommandType::transfromMode_Rotate)) this->SetTransformMode(AppTransformMode::Rotate);
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
	case AppMenuID_FILE_EXPORT:
		break;
	case AppMenuID_FILE_IMPORT:
		break;
	case AppMenuID_FILE_NEW_SCENE:
		break;
	case AppMenuID_FILE_OPEN_SCENE:
		break;
	case AppMenuID_FILE_SAVEAS_SCENE:
		break;
	case AppMenuID_FILE_SAVE_SCENE:
		break;
	case AppMenuID_HELP_ABOUT:
		ShowAbout();
		break;
	case AppMenuID_EDIT_OBJECTLISTWINDOW:
		ShowObjectListWindow();
		break;
	case AppMenuID_EDIT_INVERTSELECT:
		break;
	case AppMenuID_EDIT_SELECTALL:
		break;
	case AppMenuID_VIEW_TOGGLEFULLVIEW:
		ViewportToggleFullView();
		break;
	case AppMenuID_VIEW_TOGGLEGRID:
		ViewportToggleGrid();
		break;
	case AppMenuID_VIEW_CAMERARESET:
		ViewportCameraReset();
		break;
	case AppMenuID_VIEW_CAMERAMOVETOSELECTION:
		ViewportCameraMoveToSelection();
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_BACK:
		ViewportChangeView(AppViewportCameraType::Back);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_FRONT:
		ViewportChangeView(AppViewportCameraType::Front);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_TOP:
		ViewportChangeView(AppViewportCameraType::Top);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_BOTTOM:
		ViewportChangeView(AppViewportCameraType::Bottom);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_LEFT:
		ViewportChangeView(AppViewportCameraType::Left);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_RIGHT:
		ViewportChangeView(AppViewportCameraType::Right);
		break;
	case AppMenuID_VIEW_SETCAMERAVIEW_PERSPECTIVE:
		ViewportChangeView(AppViewportCameraType::Perspective);
		break;
	case AppMenuID_VIEW_TOGGLEORTHO:
		ViewportToggleOrtho();
		break;
	case AppMenuID_VIEW_CAMERADRAWMATERIAL:
		ViewportSetDrawMode(AppViewportDrawMode::Material);
		break;
	case AppMenuID_VIEW_CAMERADRAWMATERIALWIREFRAME:
		ViewportSetDrawMode(AppViewportDrawMode::MaterialWireframe);
		break;
	case AppMenuID_VIEW_CAMERADRAWWIREFRAME:
		ViewportSetDrawMode(AppViewportDrawMode::Wireframe);
		break;
	case AppMenuID_VIEW_TOGGLEAABB:
		ViewportToggleAABB();
		break;
	}
}



void Application::CloseProgramm()
{
	m_run = false;
}

void ExpandAllItems(HWND hTree, HTREEITEM hItem)
{
	SendMessage(hTree, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItem);
	// hParent = NULL for root level
	HTREEITEM hChild = (HTREEITEM)SendMessage(hTree, TVM_GETNEXTITEM,
		TVGN_CHILD, (LPARAM)hItem);
	while (hChild)
	{
		ExpandAllItems(hTree, hChild);
		hChild = (HTREEITEM)SendMessage(hTree, TVM_GETNEXTITEM,
			TVGN_NEXT, (LPARAM)hChild);
	}
}

void ExpandTree(HWND hTree)
{
	HTREEITEM root = (HTREEITEM)SendMessage(hTree, TVM_GETNEXTITEM,
		TVGN_ROOT, (LPARAM)0);
	ExpandAllItems(hTree, root);
}

void Application::_updateObjectList(HTREEITEM parent, AppSceneObject* sceneObject)
{
	auto& children = sceneObject->GetChildren();
	if (children.m_head)
	{
		auto curr = children.m_head;
		auto last = children.m_head->m_left;
		while (true)
		{
			TV_INSERTSTRUCT tvInsert = { 0 };
			tvInsert.hParent = parent;
			tvInsert.hInsertAfter = TVI_LAST;
			tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvInsert.item.pszText = (LPWSTR)curr->m_data->GetNameW();
			tvInsert.item.iImage = m_objectListWindowData.m_iconID;         
			tvInsert.item.iSelectedImage = m_objectListWindowData.m_iconID;
			HTREEITEM hItem = (HTREEITEM)SendMessage(m_objectListWindowData.m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

			_updateObjectList(hItem, curr->m_data);

			if (curr == last)
				break;

			curr = curr->m_right;
		}
	}
}
void Application::UpdateObjectList()
{
	if (m_objectListWindowData.m_hwnd_ObjectList)
	{
		m_objectListWindowData.m_hTreeView_objectList = GetDlgItem(m_objectListWindowData.m_hwnd_ObjectList, IDC_TREE1);
		SendMessage(m_objectListWindowData.m_hTreeView_objectList, WM_SETREDRAW, FALSE, 0);
		TreeView_DeleteAllItems(m_objectListWindowData.m_hTreeView_objectList);
		SendMessage(m_objectListWindowData.m_hTreeView_objectList, WM_SETREDRAW, TRUE, 0);
		InvalidateRect(m_objectListWindowData.m_hTreeView_objectList, NULL, TRUE);

		// From resource icons
		m_objectListWindowData.m_hImgList_treeView = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
		HICON hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDER),
			IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		m_objectListWindowData.m_iconID = ImageList_AddIcon(m_objectListWindowData.m_hImgList_treeView, hIcon);
		DestroyIcon(hIcon);
		TreeView_SetImageList(m_objectListWindowData.m_hTreeView_objectList, m_objectListWindowData.m_hImgList_treeView, TVSIL_NORMAL);

		TV_INSERTSTRUCT tvInsert = { 0 };
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = TVI_ROOT;
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvInsert.item.pszText = (LPWSTR)L"Root";
		tvInsert.item.iImage = m_objectListWindowData.m_iconID;           // icon when unselected
		tvInsert.item.iSelectedImage = m_objectListWindowData.m_iconID;   // icon when selected

		HTREEITEM hRoot = (HTREEITEM)SendMessage(m_objectListWindowData.m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		_updateObjectList(hRoot, m_scene->GetRootObject());
		ExpandTree(m_objectListWindowData.m_hTreeView_objectList);
	}
}

void Application::ShowObjectListWindow()
{
	alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();
	if (!m_objectListWindowData.m_hwnd_ObjectList)
	{
		m_objectListWindowData.m_hwnd_ObjectList = CreateDialog(
			GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOG1),
			w32->m_hwnd,
			DialogProcObjectList);

		UpdateObjectList();
		//m_hTreeView_objectList = GetDlgItem(m_hwnd_ObjectList, IDC_TREE1);
		//// From resource icons
		//m_hImgList_treeView = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
		//HICON hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDER),
		//	IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		//int iFolder = ImageList_AddIcon(m_hImgList_treeView, hIcon);
		//DestroyIcon(hIcon);

		//hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FILE),
		//	IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		//int iFile = ImageList_AddIcon(m_hImgList_treeView, hIcon);
		//TreeView_SetImageList(m_hTreeView_objectList, m_hImgList_treeView, TVSIL_NORMAL);

		//DestroyIcon(hIcon);
		//TV_INSERTSTRUCT tvInsert = { 0 };
		//tvInsert.hParent = NULL;
		//tvInsert.hInsertAfter = TVI_ROOT;
		//tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		//tvInsert.item.pszText = (LPWSTR)L"Root Item";
		//tvInsert.item.iImage = iFolder;           // icon when unselected
		//tvInsert.item.iSelectedImage = iFolder;   // icon when selected

		//HTREEITEM hRoot = (HTREEITEM)SendMessage(m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		////	SendMessage(m_hTreeView_objectList, TVM_EXPAND, TVE_EXPAND, (LPARAM)&hRoot);

		//	// Add a child
		//tvInsert.hParent = hRoot;
		//tvInsert.hInsertAfter = TVI_LAST;
		//tvInsert.item.pszText = (LPWSTR)L"Child Item";
		//tvInsert.item.iImage = iFile;           // icon when unselected
		//tvInsert.item.iSelectedImage = iFile;   // icon when selected
		//HTREEITEM hChild = (HTREEITEM)SendMessage(m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		//hChild = (HTREEITEM)SendMessage(m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
		//SendMessage(m_hTreeView_objectList, TVM_EXPAND, TVE_EXPAND, (LPARAM)&hChild);

		//tvInsert.hParent = hChild;
		//tvInsert.hInsertAfter = TVI_LAST;
		//tvInsert.item.pszText = (LPWSTR)L"Child Item2";
		//tvInsert.item.iImage = iFile;           // icon when unselected
		//tvInsert.item.iSelectedImage = iFile;   // icon when selected
		//hChild = (HTREEITEM)SendMessage(m_hTreeView_objectList, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);


		//ExpandTree(m_hTreeView_objectList);
	}
	ShowWindow(m_objectListWindowData.m_hwnd_ObjectList, SW_SHOW);
}

void Application::ShowAbout()
{
	alSystemWindowOSDataWin32* w32 = (alSystemWindowOSDataWin32*)m_mainWindow->GetOSData();
	if (!m_hwnd_About)
	{
		m_hwnd_About = CreateDialog(
			GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_PROPPAGE_SMALL),
			w32->m_hwnd,
			DialogProcAbout);
	}
	ShowWindow(m_hwnd_About, SW_SHOW);
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
		const char* funcName_load = "AppPluginLoad";
		const char* funcName_unload = "AppPluginUnload";
		AppPluginLoad_t PluginLoad = (AppPluginLoad_t)alLib::DLLGetProc(funcName_load, module);
		if (!PluginLoad)
		{
			alLog::PrintInfo("FAIL (function %s not found)\n", funcName_load);
			continue;
		}
		AppPluginUnload_t PluginUnload = (AppPluginUnload_t)alLib::DLLGetProc(funcName_unload, module);
		if (!PluginUnload)
		{
			alLog::PrintInfo("FAIL (function %s not found)\n", funcName_load);
			continue;
		}


		auto newPlugin = PluginLoad(m_pluginInterface);
		if (newPlugin)
		{
			if (newPlugin->SDKVersion() != APP_SDK_VERSION)
			{
				alDestroy(newPlugin);
				alLog::PrintError("FAIL (bad version)\n");
				continue;
			}

			alLog::PrintInfo("\t(%s)\n", alUnicodeString(newPlugin->Name()).GetStringA().c_str());
			alLog::PrintInfo("\t(%s)\n", alUnicodeString(newPlugin->Desc()).GetStringA().c_str());
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

void Application::SetPanelCreateObjectType(AppPluginObject::EObjectType type)
{
	// для простоты реализации от множеств панелей решил
	// отказаться. Буду просто обновлять список.
	//
	auto lb = dynamic_cast<AppGUIListBox*>(m_gui->m_panelCreate->GetElementByID(GUI::elementID_lbCreate));

	switch (type)
	{
	case AppPluginObject::EObjectType_Polygonal:
	default:
	{
		if (lb)
		{
			auto* o = &m_new_object_basic_data.m_data[
				AppPluginObject::EObjectType::EObjectType_Polygonal];
			void* ptr = &o->m_objs.m_data[0];
			lb->SetItems(
				ptr, 
				o->m_objs.m_size,
				sizeof(new_object_basic_data::_object),
				4,
				0);
		}
		/*AppGUIButton* btnPoly = dynamic_cast<AppGUIButton*>(m_gui->m_panelCreate->GetElementByID(AppGUIID_BTN_Create_Type_Poly));
		AppGUIButton* btnHelp = dynamic_cast<AppGUIButton*>(m_gui->m_panelCreate->GetElementByID(AppGUIID_BTN_Create_Type_Help));
		if (btnPoly && btnHelp)
		{
			btnPoly->
		}*/
	}
		break;
	case AppPluginObject::EObjectType_Helper:
		if (lb)
		{
			auto* o = &m_new_object_basic_data.m_data[
				AppPluginObject::EObjectType::EObjectType_Helper];
			void* ptr = &o->m_objs.m_data[0];
			lb->SetItems(
				ptr,
				o->m_objs.m_size,
				sizeof(new_object_basic_data::_object),
				4,
				0);
		}
		break;
	case AppPluginObject::EObjectType__end:
		lb->SetItems(0,0,0,0,0);
		break;
	}
}

void Application::_onLBSelect_createPanel(AppPluginObject* po)
{
	auto btn = dynamic_cast<AppGUIButton*>(m_gui->m_panelCreate->GetElementByID(GUI::elementID_btnCreate_CreateButton));
	if (btn)
	{
		btn->SetVisible(true);
		m_pluginObject_onCreateNew = po;
	}
}

void Application::OnButtonCreateNewObject()
{
	if (m_pluginObject_onCreateNew)
	{
		auto o = m_pluginObject_onCreateNew->CreateObject();
		if (o)
		{
			m_scene->AddObject(o);
		}
	}
}

void Application::ShowViewportPopup()
{
	alSystemPopup* popup = alLib::CreateSystemPopup();
	if (popup)
	{
		bool checked = false;
		{
			bool chP = false;
			bool chT = false;
			bool chBt = false;
			bool chL = false;
			bool chR = false;
			bool chB = false;
			bool chF = false;
			switch (m_activeViewportLayout->m_activeViewport->m_activeCamera->m_type)
			{
			case AppViewportCameraType::Perspective:
				chP = true;
				break;
			case AppViewportCameraType::Top:
				chT = true;
				break;
			case AppViewportCameraType::Bottom:
				chBt = true;
				break;
			case AppViewportCameraType::Left:
				chL = true;
				break;
			case AppViewportCameraType::Right:
				chR = true;
				break;
			case AppViewportCameraType::Front:
				chF = true;
				break;
			case AppViewportCameraType::Back:
				chB = true;
				break;
			}

			popup->AddItem(U"Perspective", AppMenuID_VIEW_SETCAMERAVIEW_PERSPECTIVE, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewPerspective), chP);
			popup->AddItem(U"Top", AppMenuID_VIEW_SETCAMERAVIEW_TOP, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewTop), chT);
			popup->AddItem(U"Bottom", AppMenuID_VIEW_SETCAMERAVIEW_BOTTOM, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewBottom), chBt);
			popup->AddItem(U"Left", AppMenuID_VIEW_SETCAMERAVIEW_LEFT, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewLeft), chL);
			popup->AddItem(U"Right", AppMenuID_VIEW_SETCAMERAVIEW_RIGHT, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewRight), chR);
			popup->AddItem(U"Front", AppMenuID_VIEW_SETCAMERAVIEW_FRONT, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewFront), chF);
			popup->AddItem(U"Back", AppMenuID_VIEW_SETCAMERAVIEW_BACK, m_shortcutManager->GetText(AppShortcutCommandType::viewport_viewBack), chB);
			popup->AddSeparator();
		}

		checked = m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho;
		popup->AddItem(U"Force Ortho", AppMenuID_VIEW_TOGGLEORTHO, m_shortcutManager->GetText(AppShortcutCommandType::viewport_toggleOrtho), checked);
		popup->AddSeparator();

		if (m_activeViewportLayout == m_viewportLayouts[AppViewportLayout_Full])
			checked = true;
		else
			checked = false;
		popup->AddItem(U"Toggle full view", AppMenuID_VIEW_TOGGLEFULLVIEW, m_shortcutManager->GetText(AppShortcutCommandType::viewport_toggleFullView), checked);

		checked = m_activeViewportLayout->m_activeViewport->m_drawGrid;		
		popup->AddItem(U"Toggle grid", AppMenuID_VIEW_TOGGLEGRID, m_shortcutManager->GetText(AppShortcutCommandType::viewport_toggleGrid), checked);

		popup->AddSeparator();
		bool chM = false;
		bool chMW = false;
		bool chW = false;
		switch (m_activeViewportLayout->m_activeViewport->m_viewportData.m_drawMode)
		{
		case AppViewportDrawMode::Material:
			chM = true;
			break;
		case AppViewportDrawMode::MaterialWireframe:
			chMW = true;
			break;
		case AppViewportDrawMode::Wireframe:
			chW = true;
			break;
		}
		popup->AddItem(U"Material", AppMenuID_VIEW_CAMERADRAWMATERIAL, m_shortcutManager->GetText(AppShortcutCommandType::viewport_dmMaterial), chM);
		popup->AddItem(U"Material+Wireframe", AppMenuID_VIEW_CAMERADRAWMATERIALWIREFRAME, m_shortcutManager->GetText(AppShortcutCommandType::viewport_dmMaterialWireframe), chMW);
		popup->AddItem(U"Wireframe", AppMenuID_VIEW_CAMERADRAWWIREFRAME, m_shortcutManager->GetText(AppShortcutCommandType::viewport_dmWireframe), chW);

		popup->AddSeparator();
		checked = m_activeViewportLayout->m_activeViewport->m_drawAabbs;
		popup->AddItem(U"Toggle draw AABB", AppMenuID_VIEW_TOGGLEAABB, m_shortcutManager->GetText(AppShortcutCommandType::viewport_toggleDrawAABB), checked);

		popup->AddSeparator();
		popup->AddItem(U"Camera Reset", AppMenuID_VIEW_CAMERARESET, m_shortcutManager->GetText(AppShortcutCommandType::viewport_cameraReset));
		popup->AddItem(U"Camera Move to selection", AppMenuID_VIEW_CAMERAMOVETOSELECTION, m_shortcutManager->GetText(AppShortcutCommandType::viewport_cameraMoveToSelection));

		popup->Show(m_mainWindow, m_input->m_cursorCoords.x, m_input->m_cursorCoords.y);

		AL_DESTROY(popup);
	}
}

alVec3f Application::AppVecToAlVec(const AppVec3f& in)
{
	return alVec3f(in.x, in.y, in.z);
}

alVec4f Application::AppVecToAlVec(const AppVec4f& in)
{
	return alVec4f(in.x, in.y, in.z, in.w);
}

alVec4 Application::AppVecToAlVec(const AppVec4& in)
{
	return alVec4(in.x, in.y, in.z, in.w);
}

alMat4 Application::AppMatToAlMat(const AppMat4& in)
{
	alMat4 m;
	m.m_data[0].x = in.m_data[0].x;
	m.m_data[0].y = in.m_data[0].y;
	m.m_data[0].z = in.m_data[0].z;
	m.m_data[0].w = in.m_data[0].w;
	m.m_data[1].x = in.m_data[1].x;
	m.m_data[1].y = in.m_data[1].y;
	m.m_data[1].z = in.m_data[1].z;
	m.m_data[1].w = in.m_data[1].w;
	m.m_data[2].x = in.m_data[2].x;
	m.m_data[2].y = in.m_data[2].y;
	m.m_data[2].z = in.m_data[2].z;
	m.m_data[2].w = in.m_data[2].w;
	m.m_data[3].x = in.m_data[3].x;
	m.m_data[3].y = in.m_data[3].y;
	m.m_data[3].z = in.m_data[3].z;
	m.m_data[3].w = in.m_data[3].w;
	return m;
}

AppMat4 Application::AlMatToAppMat(const alMat4& in)
{
	AppMat4 m;
	m.m_data[0].x = in.m_data[0].x;
	m.m_data[0].y = in.m_data[0].y;
	m.m_data[0].z = in.m_data[0].z;
	m.m_data[0].w = in.m_data[0].w;
	m.m_data[1].x = in.m_data[1].x;
	m.m_data[1].y = in.m_data[1].y;
	m.m_data[1].z = in.m_data[1].z;
	m.m_data[1].w = in.m_data[1].w;
	m.m_data[2].x = in.m_data[2].x;
	m.m_data[2].y = in.m_data[2].y;
	m.m_data[2].z = in.m_data[2].z;
	m.m_data[2].w = in.m_data[2].w;
	m.m_data[3].x = in.m_data[3].x;
	m.m_data[3].y = in.m_data[3].y;
	m.m_data[3].z = in.m_data[3].z;
	m.m_data[3].w = in.m_data[3].w;
	return m;
}

AppVec4f Application::AlVecToAppVec(const alVec4f& v)
{
	return AppVec4f(v.x, v.y, v.z, v.w);
}

AppVec3f Application::AlVecToAppVec(const alVec3f& v)
{
	return AppVec3f(v.x, v.y, v.z);
}

AppVec4 Application::AlVecToAppVec(const alVec4& v)
{
	return AppVec4(v.x, v.y, v.z, v.w);
}

AppVec3 Application::AlVecToAppVec(const alVec3& v)
{
	return AppVec3(v.x, v.y, v.z);
}

