#pragma once
#ifndef _PCApp_H_
#define _PCApp_H_

#include "SelectionFrustum.h"

#define AppViewportLayout_Full 0
#define AppViewportLayout_Standart 1
#define AppViewportLayout_Count 2

#define AppMenuID_FILE_NEW_SCENE 1
#define AppMenuID_FILE_OPEN_SCENE 2
#define AppMenuID_FILE_SAVE_SCENE 3
#define AppMenuID_FILE_SAVEAS_SCENE 4
#define AppMenuID_FILE_IMPORT 5
#define AppMenuID_FILE_EXPORT 6
#define AppMenuID_FILE_EXIT 7
#define AppMenuID_EDIT_SELECTALL 8
#define AppMenuID_EDIT_INVERTSELECT 9
#define AppMenuID_VIEW_TOGGLEFULLVIEW 10
#define AppMenuID_HELP_ABOUT 11

class AppViewportResizer;

class AppCombo_CreatePanel_Categories : public alGUIComboBox
{
public:
	AppCombo_CreatePanel_Categories(alGUIContext* ct, const alVec2f& position, const alVec2f& size)
		:
		alGUIComboBox(ct,position,size)
	{}
	virtual ~AppCombo_CreatePanel_Categories() {}
	AL_DECLARE_DEFAULT_ALLOCATOR(AppCombo_CreatePanel_Categories);
};

class AppButtonIcon : public alGUIButtonIcon
{
public:
	AppButtonIcon(alGUIContext* ct,
		alGUITextureAtlas* ta, uint32_t ii, const alVec2f& position, const alVec2f& size)
		:
		alGUIButtonIcon(ct, ta, ii,position,size)
	{}
	virtual ~AppButtonIcon() {}
	AL_DECLARE_DEFAULT_ALLOCATOR(AppButtonIcon);

	virtual void OnMouseEnter() override;
	virtual void OnMouseLeave() override;

	virtual void OnButtonToggleOn() override;
};

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
	HandGrab,
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

enum class AppTransformMode : uint32_t
{
	NoTransform,
	Move,
	Rotate,
	Scale,
	RotateLocal,
	ScaleLocal,
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
	virtual void OnSetCursor() override;
	virtual void OnPopupCommand(uint32_t cmd) override;
};

struct AppColorTheme
{
	alColor m_windowClearColor = alColor(0.41f);
	alColor m_viewportColor = alColor(0.35f);
	alColor m_viewportBorder = ColorYellow;

	alGUIColorTheme m_GUIColorTheme;
};

class Application
{
	friend class SystemWindowCallback;
	friend class AppGizmo;
	friend class AppViewportCamera;
	friend class AppViewport;
	friend class AppShortcutManager;
	friend class AppButtonIcon;

	PluginInterface* m_pluginInterface = 0;

	FILE* m_fileLog = 0;
	alCursor* m_cursors[(uint32_t)AppCursorType::_count];
	AppCursorType m_currentCursor = AppCursorType::Arrow;

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
	bool m_isCursorInGUI = false;

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

	alVec4 m_cursorPosition3D;         // intersection point
	alVec2i m_cursorLMBClickPosition;

	bool m_viewportResizeMode = false;
	AppViewportResizer* m_viewportResizer = 0;

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

	alGUIFont* m_fontGUI = 0;

	AppGSShaderCallback_LineModel3D* m_shaderLineModel = 0;

	alInput* m_input = 0;

	AppShortcutManager* m_shortcutManager = 0;

	alGSTexture* m_whiteTexture = 0;

	struct GUI
	{

		GUI() {}
		~GUI() 
		{
			AL_DESTROY(m_ta);
			AL_DESTROY(m_taTexture);

			/*AL_DESTROY(m_button_gizmoSelect);
			AL_DESTROY(m_button_gizmoMove);
			AL_DESTROY(m_button_gizmoRotate);
			AL_DESTROY(m_button_gizmoScale);*/

			//m_context->DeleteAllPanels();
			AL_DESTROY(m_context);
		}
		/*AppButtonIcon* m_button_gizmoSelect = 0;
		AppButtonIcon* m_button_gizmoMove = 0;
		AppButtonIcon* m_button_gizmoRotate = 0;
		AppButtonIcon* m_button_gizmoScale= 0;*/
		void CreateButtons();
		void CreatePanels();

		alGUIContext* m_context = 0;
		alGUIPanel* m_panel = 0;
		alGUIPanel* m_panelCreate = 0;
		alGUITextureAtlas* m_ta = 0;
		alGSTexture* m_taTexture = 0;

		AppCombo_CreatePanel_Categories* m_comboCategories = 0;

		enum {
			elementID_btnGizmoSelect = 1,
			elementID_btnGizmoMove,
			elementID_btnGizmoRotate,
			elementID_btnGizmoScale,
			elementID_btnGizmoRotateLocal,
			elementID_btnGizmoScaleLocal,
			elementID_btnCreateAdd,
			elementID_btnObjectParameters,
			elementID_btnObjectEdit,
		};
	};
	GUI* m_gui = 0;

	AppTransformMode m_transformMode = AppTransformMode::NoTransform;

	alStringW m_toolTipBuffer;
	HWND m_hwndTT = 0;
	bool m_showToolTip = false;
	float32_t m_tooltipTimer = 0.f;
	const wchar_t* m_toolTipText = 0;
	
	struct plugin_info {
		Plugin* m_plugin = 0;
		alStringA m_path;
		PluginUnload_t m_unloadFunc = 0;
	};
	alArray<plugin_info> m_plugins;
	void _initPlugins();
	
	struct new_object_basic_data
	{
		static const uint32_t NAME_SIZE = 50;
		struct new_object_subcategory
		{
			char32_t m_name[NAME_SIZE];
			//Plugin* m_plugin = 0;
		};
		struct new_object_category
		{
			char32_t m_name[NAME_SIZE];
			alArray<new_object_subcategory> m_subcategories;
		};
		alArray<new_object_category> m_categories;
	};
	new_object_basic_data m_new_object_basic_data;

public:
	Application();
	~Application();

	void ShowToolTip();
	void ShowToolTip(int x, int y, const wchar_t* text);
	void HideToolTip();

	bool OnCreate(const char* videoDriver);
	void MainLoop();
	void PrintLog(const char* s);
	void UpdateWindowTitle();
	void OnSetCursor();
	void OnPopupCommand(uint32_t cmd);
	void CloseProgramm();
	void ShowAbout();
	alSystemWindow* GetMainWindow() { return m_mainWindow; }

	void GetRayFromScreen(alRay* ray, const alVec2f& coords, const alVec4f& viewportRect, const alMat4& VPInvert);

	void OnWindowSizeChanged();
	
	void ProcessShortcuts3D();

	void ViewportToggleFullView();
	
	void SetTransformMode(AppTransformMode);
};

#endif

