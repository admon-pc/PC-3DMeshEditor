#pragma once
#ifndef _PCSHRTCTMNGR_H_
#define _PCSHRTCTMNGR_H_

enum class AppShortcutCommandType : uint32_t
{
	viewport_cameraReset,
	viewport_cameraMoveToSelection,
	viewport_viewPerspective,
	viewport_viewTop,
	viewport_viewBottom,
	viewport_viewLeft,
	viewport_viewRight,
	viewport_viewFront,
	viewport_viewBack,
	viewport_toggleOrtho,
	viewport_toggleGrid,
	viewport_toggleFullView,
	viewport_dmMaterial,
	viewport_dmMaterialWireframe,
	viewport_dmWireframe,
	viewport_toggleDMMaterial,
	viewport_toggleDMWireframe,
	viewport_toggleDrawAABB,
	transfromMode_NoTransform,
	transfromMode_Move,
	transfromMode_Scale,
	transfromMode_Rotate,
	select_selectAll,
	select_deselectAll,
	select_invertSelection,
	editMode_Vertex,
	editMode_Edge,
	editMode_Polygon,
	UV_selectAll,
	edit_duplicate,

	count
};

struct AppShortcutCommand {
	AppShortcutCommand() {
		m_type = AppShortcutCommandType::count;
		m_keyMod = alKeyboardModifier::None;
		m_key = alInputKey::END;
	}
	AppShortcutCommandType m_type;
	alUnicodeString m_text; // Z
	alUnicodeString m_name; // "viewport_cameraMoveToSelection"
	alKeyboardModifier m_keyMod;
	alInputKey m_key;
};

class AppShortcutManager
{
	AppShortcutCommand m_commands[(uint32_t)AppShortcutCommandType::count];
public:
	AppShortcutManager();
	~AppShortcutManager();

	void SetCommand(AppShortcutCommandType, const char32_t* commandname, alKeyboardModifier, alInputKey);
	bool IsShortcutActive(AppShortcutCommandType);
	const char32_t* GetText(AppShortcutCommandType);
};

#endif

