#include "editor.h"
extern Application* g_app;

AppShortcutManager::AppShortcutManager()
{
	SetCommand(AppShortcutCommandType::viewport_cameraReset, U"viewport_cameraReset", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_cameraMoveToSelection, U"viewport_cameraMoveToSelection", alKeyboardModifier::None, alInputKey::K_Z);
	SetCommand(AppShortcutCommandType::viewport_viewPerspective, U"viewport_viewPerspective", alKeyboardModifier::None, alInputKey::K_P);
	SetCommand(AppShortcutCommandType::viewport_viewTop, U"viewport_viewTop", alKeyboardModifier::None, alInputKey::K_T);
	SetCommand(AppShortcutCommandType::viewport_viewBottom, U"viewport_viewBottom", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_viewLeft, U"viewport_viewLeft", alKeyboardModifier::None, alInputKey::K_L);
	SetCommand(AppShortcutCommandType::viewport_viewRight, U"viewport_viewRight", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_viewFront, U"viewport_viewFront", alKeyboardModifier::None, alInputKey::K_F);
	SetCommand(AppShortcutCommandType::viewport_viewBack, U"viewport_viewBack", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_toggleGrid, U"viewport_toggleGrid", alKeyboardModifier::None, alInputKey::K_G);
	SetCommand(AppShortcutCommandType::viewport_toggleFullView, U"viewport_toggleFullView", alKeyboardModifier::Alt, alInputKey::K_W);
	SetCommand(AppShortcutCommandType::viewport_dmMaterial, U"viewport_dmMaterial", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_dmMaterialWireframe, U"viewport_dmMaterialWireframe", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_dmWireframe, U"viewport_dmWireframe", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::viewport_toggleDMMaterial, U"viewport_toggleDMMaterial", alKeyboardModifier::None, alInputKey::K_F1);
	SetCommand(AppShortcutCommandType::viewport_toggleDMWireframe, U"viewport_toggleDMWireframe", alKeyboardModifier::None, alInputKey::K_F2);
	SetCommand(AppShortcutCommandType::viewport_toggleDrawAABB, U"viewport_toggleDrawAABB", alKeyboardModifier::None, alInputKey::END);
	SetCommand(AppShortcutCommandType::transfromMode_NoTransform, U"transfromMode_NoTransform", alKeyboardModifier::None, alInputKey::K_Q);
	SetCommand(AppShortcutCommandType::transfromMode_Move, U"transfromMode_Move", alKeyboardModifier::None, alInputKey::K_W);
	SetCommand(AppShortcutCommandType::transfromMode_Scale, U"transfromMode_Scale", alKeyboardModifier::None, alInputKey::K_E);
	SetCommand(AppShortcutCommandType::transfromMode_Rotate, U"transfromMode_Rotate", alKeyboardModifier::None, alInputKey::K_R);
	SetCommand(AppShortcutCommandType::select_selectAll, U"select_selectAll", alKeyboardModifier::Ctrl, alInputKey::K_A);
	SetCommand(AppShortcutCommandType::select_deselectAll, U"select_deselectAll", alKeyboardModifier::Ctrl, alInputKey::K_D);
	SetCommand(AppShortcutCommandType::select_invertSelection, U"select_invertSelection", alKeyboardModifier::Ctrl, alInputKey::K_I);
	SetCommand(AppShortcutCommandType::editMode_Vertex, U"editMode_Vertex", alKeyboardModifier::None, alInputKey::K_1);
	SetCommand(AppShortcutCommandType::editMode_Edge, U"editMode_Edge", alKeyboardModifier::None, alInputKey::K_2);
	SetCommand(AppShortcutCommandType::editMode_Polygon, U"editMode_Polygon", alKeyboardModifier::None, alInputKey::K_3);
	SetCommand(AppShortcutCommandType::UV_selectAll, U"UV_selectAll", alKeyboardModifier::Ctrl, alInputKey::K_A);
	SetCommand(AppShortcutCommandType::edit_duplicate, U"edit_duplicate", alKeyboardModifier::None, alInputKey::END);
}

AppShortcutManager::~AppShortcutManager()
{
}

void AppShortcutManager::SetCommand(AppShortcutCommandType type,
	const char32_t* commandname, alKeyboardModifier kmod, alInputKey k)
{
	m_commands[(uint32_t)type].m_type = type;
	m_commands[(uint32_t)type].m_name = commandname;
	m_commands[(uint32_t)type].m_key = k;
	m_commands[(uint32_t)type].m_keyMod = kmod;
	m_commands[(uint32_t)type].m_text.Clear();

	if (k != alInputKey::END)
	{
		switch (kmod)
		{
		case alKeyboardModifier::None:break;
		case alKeyboardModifier::Ctrl:m_commands[(uint32_t)type].m_text += U"Ctrl+"; break;
		case alKeyboardModifier::Alt:m_commands[(uint32_t)type].m_text += U"Alt+"; break;
		case alKeyboardModifier::Shift:m_commands[(uint32_t)type].m_text += U"Shift+"; break;
		case alKeyboardModifier::ShiftAlt:m_commands[(uint32_t)type].m_text += U"Shift+Alt+"; break;
		case alKeyboardModifier::ShiftCtrl:m_commands[(uint32_t)type].m_text += U"Shift+Ctrl+"; break;
		case alKeyboardModifier::ShiftCtrlAlt:m_commands[(uint32_t)type].m_text += U"Shift+Ctrl+Alt+"; break;
		case alKeyboardModifier::CtrlAlt:m_commands[(uint32_t)type].m_text += U"Ctrl+Alt+"; break;
		default:break;
		}

		switch (k)
		{
		case alInputKey::K_PAUSE:m_commands[(uint32_t)type].m_text += U"Pause"; break;
		case alInputKey::K_SPACE:m_commands[(uint32_t)type].m_text += U"Space"; break;
		case alInputKey::K_PGUP:m_commands[(uint32_t)type].m_text += U"PageUp"; break;
		case alInputKey::K_PGDOWN:m_commands[(uint32_t)type].m_text += U"PageDown"; break;
		case alInputKey::K_END:m_commands[(uint32_t)type].m_text += U"End"; break;
		case alInputKey::K_HOME:m_commands[(uint32_t)type].m_text += U"Home"; break;
		case alInputKey::K_LEFT:m_commands[(uint32_t)type].m_text += U"Left"; break;
		case alInputKey::K_UP:m_commands[(uint32_t)type].m_text += U"Up"; break;
		case alInputKey::K_RIGHT:m_commands[(uint32_t)type].m_text += U"Right"; break;
		case alInputKey::K_DOWN:m_commands[(uint32_t)type].m_text += U"Down"; break;
		case alInputKey::K_PRTSCR:m_commands[(uint32_t)type].m_text += U"PrtScr"; break;
		case alInputKey::K_INSERT:m_commands[(uint32_t)type].m_text += U"Insert"; break;
		case alInputKey::K_0:m_commands[(uint32_t)type].m_text += U"0"; break;
		case alInputKey::K_1:m_commands[(uint32_t)type].m_text += U"1"; break;
		case alInputKey::K_2:m_commands[(uint32_t)type].m_text += U"2"; break;
		case alInputKey::K_3:m_commands[(uint32_t)type].m_text += U"3"; break;
		case alInputKey::K_4:m_commands[(uint32_t)type].m_text += U"4"; break;
		case alInputKey::K_5:m_commands[(uint32_t)type].m_text += U"5"; break;
		case alInputKey::K_6:m_commands[(uint32_t)type].m_text += U"6"; break;
		case alInputKey::K_7:m_commands[(uint32_t)type].m_text += U"7"; break;
		case alInputKey::K_8:m_commands[(uint32_t)type].m_text += U"8"; break;
		case alInputKey::K_9:m_commands[(uint32_t)type].m_text += U"9"; break;
		case alInputKey::K_A:m_commands[(uint32_t)type].m_text += U"A"; break;
		case alInputKey::K_B:m_commands[(uint32_t)type].m_text += U"B"; break;
		case alInputKey::K_C:m_commands[(uint32_t)type].m_text += U"C"; break;
		case alInputKey::K_D:m_commands[(uint32_t)type].m_text += U"D"; break;
		case alInputKey::K_E:m_commands[(uint32_t)type].m_text += U"E"; break;
		case alInputKey::K_F:m_commands[(uint32_t)type].m_text += U"F"; break;
		case alInputKey::K_G:m_commands[(uint32_t)type].m_text += U"G"; break;
		case alInputKey::K_H:m_commands[(uint32_t)type].m_text += U"H"; break;
		case alInputKey::K_I:m_commands[(uint32_t)type].m_text += U"I"; break;
		case alInputKey::K_J:m_commands[(uint32_t)type].m_text += U"J"; break;
		case alInputKey::K_K:m_commands[(uint32_t)type].m_text += U"K"; break;
		case alInputKey::K_L:m_commands[(uint32_t)type].m_text += U"U"; break;
		case alInputKey::K_M:m_commands[(uint32_t)type].m_text += U"M"; break;
		case alInputKey::K_N:m_commands[(uint32_t)type].m_text += U"N"; break;
		case alInputKey::K_O:m_commands[(uint32_t)type].m_text += U"O"; break;
		case alInputKey::K_P:m_commands[(uint32_t)type].m_text += U"P"; break;
		case alInputKey::K_Q:m_commands[(uint32_t)type].m_text += U"Q"; break;
		case alInputKey::K_R:m_commands[(uint32_t)type].m_text += U"R"; break;
		case alInputKey::K_S:m_commands[(uint32_t)type].m_text += U"S"; break;
		case alInputKey::K_T:m_commands[(uint32_t)type].m_text += U"T"; break;
		case alInputKey::K_U:m_commands[(uint32_t)type].m_text += U"U"; break;
		case alInputKey::K_V:m_commands[(uint32_t)type].m_text += U"V"; break;
		case alInputKey::K_W:m_commands[(uint32_t)type].m_text += U"W"; break;
		case alInputKey::K_X:m_commands[(uint32_t)type].m_text += U"X"; break;
		case alInputKey::K_Y:m_commands[(uint32_t)type].m_text += U"Y"; break;
		case alInputKey::K_Z:m_commands[(uint32_t)type].m_text += U"Z"; break;
		case alInputKey::K_NUM_0:m_commands[(uint32_t)type].m_text += U"Num0"; break;
		case alInputKey::K_NUM_1:m_commands[(uint32_t)type].m_text += U"Num1"; break;
		case alInputKey::K_NUM_2:m_commands[(uint32_t)type].m_text += U"Num2"; break;
		case alInputKey::K_NUM_3:m_commands[(uint32_t)type].m_text += U"Num3"; break;
		case alInputKey::K_NUM_4:m_commands[(uint32_t)type].m_text += U"Num4"; break;
		case alInputKey::K_NUM_5:m_commands[(uint32_t)type].m_text += U"Num5"; break;
		case alInputKey::K_NUM_6:m_commands[(uint32_t)type].m_text += U"Num6"; break;
		case alInputKey::K_NUM_7:m_commands[(uint32_t)type].m_text += U"Num7"; break;
		case alInputKey::K_NUM_8:m_commands[(uint32_t)type].m_text += U"Num8"; break;
		case alInputKey::K_NUM_9:m_commands[(uint32_t)type].m_text += U"Num9"; break;
		case alInputKey::K_NUM_MUL:m_commands[(uint32_t)type].m_text += U"Num*"; break;
		case alInputKey::K_NUM_ADD:m_commands[(uint32_t)type].m_text += U"Num+"; break;
		case alInputKey::K_NUM_SUB:m_commands[(uint32_t)type].m_text += U"Num-"; break;
		case alInputKey::K_NUM_DOT:m_commands[(uint32_t)type].m_text += U"Num."; break;
		case alInputKey::K_NUM_SLASH:m_commands[(uint32_t)type].m_text += U"Num/"; break;
		case alInputKey::K_F1:m_commands[(uint32_t)type].m_text += U"F1"; break;
		case alInputKey::K_F2:m_commands[(uint32_t)type].m_text += U"F2"; break;
		case alInputKey::K_F3:m_commands[(uint32_t)type].m_text += U"F3"; break;
		case alInputKey::K_F4:m_commands[(uint32_t)type].m_text += U"F4"; break;
		case alInputKey::K_F5:m_commands[(uint32_t)type].m_text += U"F5"; break;
		case alInputKey::K_F6:m_commands[(uint32_t)type].m_text += U"F6"; break;
		case alInputKey::K_F7:m_commands[(uint32_t)type].m_text += U"F7"; break;
		case alInputKey::K_F8:m_commands[(uint32_t)type].m_text += U"F8"; break;
		case alInputKey::K_F9:m_commands[(uint32_t)type].m_text += U"F9"; break;
		case alInputKey::K_F10:m_commands[(uint32_t)type].m_text += U"F10"; break;
		case alInputKey::K_F11:m_commands[(uint32_t)type].m_text += U"F11"; break;
		case alInputKey::K_F12:m_commands[(uint32_t)type].m_text += U"F12"; break;
		case alInputKey::K_COLON:m_commands[(uint32_t)type].m_text += U";"; break;
		case alInputKey::K_ADD:m_commands[(uint32_t)type].m_text += U"="; break;
		case alInputKey::K_COMMA:m_commands[(uint32_t)type].m_text += U","; break;
		case alInputKey::K_SUB:m_commands[(uint32_t)type].m_text += U"-"; break;
		case alInputKey::K_DOT:m_commands[(uint32_t)type].m_text += U"."; break;
		case alInputKey::K_SLASH:m_commands[(uint32_t)type].m_text += U"/"; break;
		case alInputKey::K_TILDE:m_commands[(uint32_t)type].m_text += U"`"; break;
		case alInputKey::K_FIGURE_OPEN:m_commands[(uint32_t)type].m_text += U"["; break;
		case alInputKey::K_FIGURE_CLOSE:m_commands[(uint32_t)type].m_text += U"]"; break;
		case alInputKey::K_QUOTE:m_commands[(uint32_t)type].m_text += U"'"; break;
		default:break;
		}
	}
}

bool AppShortcutManager::IsShortcutActive(AppShortcutCommandType type)
{
	auto& command = m_commands[(uint32_t)type];
	if (command.m_key == alInputKey::END)
		return false;

	if (command.m_keyMod != g_app->m_input->m_kbm)
		return false;

	return g_app->m_input->IsKeyHit(command.m_key);
}

const char32_t* AppShortcutManager::GetText(AppShortcutCommandType type)
{
	return m_commands[(uint32_t)type].m_text.data();
}

