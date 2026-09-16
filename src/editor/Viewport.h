#pragma once
#ifndef _PCViewport_H_
#define _PCViewport_H_

#include "ViewportCamera.h"

constexpr float32_t AppViewportBorderSize = 1.f;
constexpr float32_t g_leftPanelWidth = 24.f;
constexpr float32_t g_topPanelHeight = 32.f;
constexpr float32_t g_rightPanelWidth = 225.f;
constexpr float32_t g_rightPanelButtonWidth = 24.f;
constexpr float32_t g_bottomPanelHeight = 50.f;

enum class AppViewportType : uint32_t
{
	Scene,
	UV
};


class AppViewport
{
public:
	AppViewport(AppViewportType vt, AppViewportCameraType vct, const alVec4f& rect0_1);
	~AppViewport();

	void Init();
	void Copy(AppViewport*);

	void OnWindowSize();
	void Draw3D();
	void OnDrawUV();
	alGS* m_gs = 0;

	bool m_isOnLeftBorder = false;
	bool m_isOnRightBorder = false;
	bool m_isOnTopBorder = false;
	bool m_isOnBottomBorder = false;

	alTriangle m_rayTestTiangles[2];

	enum {
		Camera_Perspective = 0,
		Camera_Top,
		Camera_Bottom,
		Camera_Front,
		Camera_Back,
		Camera_Left,
		Camera_Right,
		Camera_count_,
	};

	AppViewportCamera* m_camera[Camera_count_];
	AppViewportCamera* m_activeCamera = 0;

	AppViewportType m_viewportType = AppViewportType::Scene;
	AppViewportCameraType m_cameraType = AppViewportCameraType::Perspective;

	void SetCameraType(AppViewportCameraType);
	void SetViewportName(const char32_t*);

	void UpdateAspect();

	void _frustum_cull(AppSceneObject*);
	alArray<AppSceneObject*> m_visibleObjects;

	//alGUIText* m_gui_text_vpName = 0;
	//alGUIPanel* m_GUI_panel = 0;
	void HideGUI();
	void ShowGUI();
	void ResetCamera();

	bool m_isDrawAabbs = true;

	alUnicodeString m_name;
	float32_t m_nameLen = 0.f;

	int32_t m_index = 0;
	
	//alVec4 m_creationRect; // 0;1
	//alVec4 m_currentRect;
	//alVec2f m_currentRectSize;
	alVec4f m_rect;
	alVec2f m_rectSz;
	alVec4f m_rect1_0;
	alGSTexture* m_rtt = 0;
	
	alVec4f m_textNameRect;
	alVec2f m_textNamePos;
	alColor m_textNameColor = ColorWhite;

	bool m_isCursorInRect = false;
	
	AppViewportData m_viewportData;
	void SetDrawMode(AppViewportDrawMode);

	bool m_drawGrid = true;
	void SetDrawGrid(bool);
	void _drawGrid();
	void _drawScene();
	void _drawAabb(const AppAabb& aabb, const AppColor& _color, const AppVec3f& positionOffset);

	void _drawSelectedObjectFrame();

	void ToggleDrawModeMaterial();
	void ToggleDrawModeWireframe();
	void ToggleDrawAABB();

	alVec4 GetCursorRayHitPosition(const alVec2f& cursorPosition);

	void PanMove();
	void Rotate(float32_t x, float32_t y);
	void RotateZ();
	void Zoom();
	void ChangeFOV();
};

#include "ViewportLayout.h"


#endif

