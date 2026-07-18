#pragma once
#ifndef _PCApp_H_
#define _PCApp_H_

#include "SelectionFrustum.h"

#define AppViewportLayout_Full 0
#define AppViewportLayout_Standart 1
#define AppViewportLayout_Count 2

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

enum class AppCursorType : uint32_t
{
	Arrow,
	Cross,
	Hand,
	Help,
	IBeam,
	No,
	Size,
	SizeNESW,
	SizeNS,
	SizeNWSE,
	SizeWE,
	UpArrow,
	Wait,

	SelectObject,
	SelectVertex,
	Rotate,
	_count
};

// Some objects are supposed to be editable objects. Some not, like Dummy.
// Lets just have minimum modes for editable polygonal object.
enum class AppEditMode : uint32_t
{
	Vertex,
	Edge,
	Polygon,
	Object,
};

// When Transform Mode != NoTransform app will draw gizmo
// When user will move cursor on gizmo and will press LMB
//  application will remember what part of the gizme he pressed
enum class AppGizmoMode : uint32_t
{
	NoTransform,
	MoveX,
	MoveY,
	MoveZ,
	MoveXZ,
	MoveXY,
	MoveZY,
	MoveScreen,
	ScaleX,
	ScaleY,
	ScaleZ,
	ScaleXZ,
	ScaleXY,
	ScaleZY,
	ScaleScreen,
	RotateX,
	RotateY,
	RotateZ,
	RotateScreen,
	Other
};

enum class AppGizmoUVMode : uint32_t
{
	NoTransform,
	LeftTop,
	Top,
	RightTop,
	Left,
	Right,
	LeftBottom,
	Bottom,
	RightBottom,
	Center
};

enum class AppMouseMode : uint32_t
{
	// select by rect
	// just default mode
	CommonMode,

	// like target weld or creation new object like plane
	ClickAndDrag,

	// It will hide cursor, save coords, and setCursorPo( this coords )
	HideCursor,

	// It will change cursor. Will be disabled on escape/RMB/miSceneObject selection
	// set callbacks sdk->SetSelectObjectCallbacks(...); before using
	SelectObject,

	// set callbacks sdk->SetSelectVertexCallbacks(...); before using
	SelectVertex,

	// for some actions.
	// For example some actions like Weld: you can select vertices at any time when Weld is active
	// must be disabled on escape/RMB/toggle edit mode/toggle transform mode
	Other
};

class SystemWindowCallback : public alSystemWindowCallback
{
	Application* m_app = 0;
public:
	SystemWindowCallback(Application*);
	virtual ~SystemWindowCallback();
	virtual void OnSizeChanged(alSystemWindow*) override;
	virtual alVec2i OnGPUUpdateSize(alSystemWindow* w) override;
	virtual alVec2i OnMinMaxInfo(alSystemWindow* w) override;
	virtual void OnClose(alSystemWindow* window) override;
};

struct AppColorTheme
{
	alColor m_windowClearColor = alColor(0.41f);
	alColor m_viewportColor = alColor(0.35f);
	alColor m_viewportBorder = ColorDarkGray;
};

class Application
{
	friend class SystemWindowCallback;
	friend class AppGizmo;
	friend class AppViewportCamera;
	friend class AppViewport;


	FILE* m_fileLog = 0;
	alCursor* m_cursors[(uint32_t)AppCursorType::_count];

	bool m_run = true;
	float32_t m_dt = 0.f;
	SystemWindowCallback* m_windowCallback = 0;
	alSystemWindow* m_mainWindow = 0;
	alGS* m_gs = 0;
	alVec2f m_gpuDepthRange;
	alGSTexture* m_transparentTexture = 0;
	alGSTexture* m_blackTexture = 0;

	// true if mouseDelta != 0
	bool m_isCursorMove = false;
	bool m_isCursorInViewport = false;
	bool m_isCursorInUVEditor = false;

	AppMouseMode m_mouseMode = AppMouseMode::CommonMode;
	void SetMouseMode(AppMouseMode mm);

	bool _isVertexMouseHover();
	bool _isEdgeMouseHover();
	void _isObjectMouseHover();
	//bool m_isMouseHoverEdge = false;
	AppVertex* m_mouseHoverVertex = 0;
	AppEdge* m_mouseHoverEdge = 0;
	AppSceneObject* m_mouseHoverObject = 0;

	alAabb m_sceneAabb;
	alAabb m_selectionAabb;
	alVec4 m_selectionAabb_center;
	alVec4 m_selectionAabb_extent;

	AppEditMode m_editMode = AppEditMode::Object;

	alArray<AppSceneObject*> m_selectedObjects;

	AppGizmo* m_gizmo = 0;
	AppGizmoMode m_gizmoMode = AppGizmoMode::NoTransform;
	AppGizmoUVMode m_gizmoModeUV = AppGizmoUVMode::NoTransform;

	alGSMesh* m_gridModel_perspective1 = 0;
	alGSMesh* m_gridModel_perspective2 = 0;
	alGSMesh* m_gridModel_top1 = 0;
	alGSMesh* m_gridModel_top2 = 0;
	alGSMesh* m_gridModel_front1 = 0;
	alGSMesh* m_gridModel_front2 = 0;
	alGSMesh* m_gridModel_left1 = 0;
	alGSMesh* m_gridModel_left2 = 0;
	void _initGridMesh();

	alRay m_screenRayOnClick;
	alRay m_screenRayCurrent;

	AppSelectionFrust m_selectionFrust;

	// every viewport will set this when drawing
	AppViewport* m_currentViewportDraw = 0;
	AppViewport* m_viewportUnderCursor = 0;
	AppViewport* m_viewportInMouseFocus = 0; // lmb down and hold
	AppViewportLayout* m_activeViewportLayout = 0;
	AppViewportLayout* m_previousViewportLayout = 0; // save here when Alt + W
	AppViewportLayout* m_viewportLayouts[AppViewportLayout_Count];
	AppViewportLayout* m_UVViewport = 0;
	void _initViewports();
	void UpdateViewports();
	//miPopup* _getPopupInViewport();
	void DrawViewports3D();
	void DrawViewports();
	void _callViewportOnWindowSize();
	// надо определить первый клик в зоне вьюпорта. если был то true. потом двигать камеру и объекты
	// только если m_isViewportInFocus == true;
	bool m_isViewportInFocus = false;
	AppViewportCamera* GetActiveCamera();

	alGSTexture* m_UVPlaneTexture = 0;
	alGSMesh* m_UVPlaneModel = 0;
	void OnGizmoUVClick();
	void UVTransform();
	void UVTransformCancel();
	void UVTransformAccept();
	void UvMakePlanar(bool useScreenPlane);
	void UvFlattenMapping();
	void UvSaveUVTemplate();
	alAabb m_UVAabb;
	alAabb m_UVAabbOnClick;
	alVec4f m_UVAabbCenterOnClick;
	alVec4f m_UVAabbMoveOffset;
	float32_t m_UVAngle = 0.f;
	void UVSelectAll();

	AppColorTheme m_colorTheme;
	AppColorTheme* m_colorThemeCurr = 0;

	AppGSShaderCallback_LineModel3D* m_shaderLineModel = 0;

public:
	Application();
	~Application();

	bool OnCreate(const char* videoDriver);
	void MainLoop();
	void PrintLog(const char* s);
	void UpdateWindowTitle();

	void GetRayFromScreen(alRay* ray, const alVec2f& coords, const alVec4f& viewportRect, const alMat4& VPInvert);

	void OnWindowSizeChanged();
};

#endif

