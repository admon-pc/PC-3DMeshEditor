#pragma once
#ifndef _PCGizmo_H_

class Application;
class AppViewport;
class AppGizmo
{
	void _reset_variables();
	Application* m_app = 0;
public:
	AppGizmo(Application*);
	~AppGizmo();

	float32_t m_gizmo_arrow_body_size = 0.125f;
	float32_t m_gizmo_head_size = 0.01f;
	float32_t m_gizmo_head_len = 0.025f;
	float32_t m_gizmo_2pl_sz = 0.01f;
	float32_t m_gizmo_rot_sz = 0.1f;
	float32_t m_gizmo_rot_sz_screen = 0.f;
	float32_t m_gizmo_rot_sz_mn = 0.01f;
	float32_t m_gizmo_rot_sz_mx = 0.01f;

	//miMaterial m_commonMaterial;
	//miMaterial m_pivotModelMaterial;

	alGSMesh* m_pivotModel = 0;

	alGSMesh* m_rotateX = 0;
	alGSMesh* m_rotateY = 0;
	alGSMesh* m_rotateZ = 0;
	alGSMesh* m_rotateScreen = 0;
	//miSprite* m_rotateSprite = 0;

	alGSMesh* m_X = 0;
	alGSMesh* m_Y = 0;
	alGSMesh* m_Z = 0;

	alGSMesh* m_HeadMoveX = 0;
	alGSMesh* m_HeadMoveY = 0;
	alGSMesh* m_HeadMoveZ = 0;

	alGSMesh* m_HeadScaleX = 0;
	alGSMesh* m_HeadScaleY = 0;
	alGSMesh* m_HeadScaleZ = 0;

	alAabb m_XAabb;
	alAabb m_YAabb;
	alAabb m_ZAabb;

	alAabb m_XAabbMod;
	alAabb m_YAabbMod;
	alAabb m_ZAabbMod;


	alAabb m_HeadXAabb;
	alAabb m_HeadYAabb;
	alAabb m_HeadZAabb;

	alAabb m_HeadXAabbMod;
	alAabb m_HeadYAabbMod;
	alAabb m_HeadZAabbMod;

	alAabb m_HeadScaleXAabb;
	alAabb m_HeadScaleYAabb;
	alAabb m_HeadScaleZAabb;

	alAabb m_HeadScaleXAabbMod;
	alAabb m_HeadScaleYAabbMod;
	alAabb m_HeadScaleZAabbMod;

	bool m_isDrawAabbX = false;
	bool m_isDrawAabbY = false;
	bool m_isDrawAabbZ = false;

	bool m_isDrawAabbHeadX = false;
	bool m_isDrawAabbHeadY = false;
	bool m_isDrawAabbHeadZ = false;

	bool m_isDrawAabbScaleHeadX = false;
	bool m_isDrawAabbScaleHeadY = false;
	bool m_isDrawAabbScaleHeadZ = false;

	bool m_isRotationHoverX = false;
	bool m_isRotationHoverY = false;
	bool m_isRotationHoverZ = false;
	bool m_isRotationHoverScreen = false;

	alGSMesh* m_XZ = 0;
	alAabb m_XZAabb;
	alAabb m_XZAabbMod;
	bool m_isDrawAabbXZ = false;

	alGSMesh* m_XY = 0;
	alAabb m_XYAabb;
	alAabb m_XYAabbMod;
	bool m_isDrawAabbXY = false;

	alGSMesh* m_ZY = 0;
	alAabb m_ZYAabb;
	alAabb m_ZYAabbMod;
	bool m_isDrawAabbZY = false;

	void Draw(AppViewport*);
	void Update(AppViewport*);
	void OnStartFrame();
	void OnEndFrame();

	alMat4 m_S;
	alMat4 m_T;
	alMat4 m_W;
	alMat4 m_WVP;

	alVec2f m_2d_point;
	bool m_isIn2d = false;
	float32_t m_size_2d = 10.f;

	alColor m_color_x;
	alColor m_color_y;
	alColor m_color_z;

	alVec3f m_moveDelta;
	alVec3f m_var_move_old;
	alVec3f m_var_move;
	alVec3f m_var_move_onEscape; // just for moving verts back to position 
	alVec3f m_var_scale;
	alVec3f m_var_scale2;
	alVec4f m_var_rotate;
	alVec4f m_var_rotate_final;
	//float32_t m_var_rotation_add = 0.f; // when shift, rotate object on good angle
	float32_t m_var_rotate_snap = 5.f;
	float32_t m_var_rotate_angle = 0.f; // for information
	alMat4 m_rotateScreenMatrix;

	alVec3f m_position;

	//alVec4f m_selectionAabbCenterOnClick;
	void OnClick();
	void OnRelease();
	void OnEscape();
};


#endif

