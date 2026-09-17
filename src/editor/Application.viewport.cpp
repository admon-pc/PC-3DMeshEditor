#include "editor.h"

class AppViewportResizer_Standart : public AppViewportResizer
{
public:
	AppViewportResizer_Standart(int id) : AppViewportResizer(id) {}
	virtual ~AppViewportResizer_Standart() {}
	virtual void Rebuild() override
	{
		switch (m_id)
		{
		case 0:
		{
			m_rect.x = m_layout->m_viewports.m_data[0]->m_rect.z - 2;
			m_rect.y = m_layout->m_viewports.m_data[0]->m_rect.y;
			m_rect.z = m_layout->m_viewports.m_data[0]->m_rect.z + 2;
			m_rect.w = m_layout->m_viewports.m_data[3]->m_rect.w;
		}break;
		case 1:
		{
			m_rect.x = m_layout->m_viewports.m_data[0]->m_rect.x;
			m_rect.y = m_layout->m_viewports.m_data[0]->m_rect.w - 2;
			m_rect.z = m_layout->m_viewports.m_data[1]->m_rect.z;
			m_rect.w = m_layout->m_viewports.m_data[3]->m_rect.y + 2;
		}break;
		case 2:
		{
			m_rect.x = m_layout->m_viewports.m_data[0]->m_rect.z - 5;
			m_rect.y = m_layout->m_viewports.m_data[0]->m_rect.w - 5;
			m_rect.z = m_layout->m_viewports.m_data[0]->m_rect.z + 5;
			m_rect.w = m_layout->m_viewports.m_data[0]->m_rect.w + 5;
		}break;
		}		
	}
	virtual void OnResize() override
	{
		auto input = alLib::GetInput();

		bool mouseMove = false;
		if (input->m_mouseDelta.x != 0.f || input->m_mouseDelta.y != 0.f)
			mouseMove = true;
		if (mouseMove)
		{
	/*
	if (m_isOnLeftBorder) m_rect.x += g_leftPanelWidth;
	if (m_isOnRightBorder) m_rect.z -= g_rightPanelWidth;
	if (m_isOnTopBorder) m_rect.y += g_topPanelHeight;
	if (m_isOnBottomBorder) m_rect.w -= g_bottomPanelHeight;
	*/
			switch (m_id)
			{
			case 0:
			{
				float32_t leftSide = m_layout->m_viewports.m_data[0]->m_rect.x;
				float32_t W = m_layout->m_viewports.m_data[1]->m_rect.z - leftSide;
				if (input->m_cursorCoords.x >= leftSide)
				{
					if (input->m_cursorCoords.x <= m_layout->m_viewports.m_data[1]->m_rect.z)
					{
						//printf("%i\n", input->m_cursorCoords.x);
						/*float32_t v = alMath::Normalize(W, input->m_cursorCoords.x
							- leftSide 
							- g_rightPanelWidth
							+ g_leftPanelWidth
							- AppViewportBorderSize);
						if (v > 1.f)
							v = 1.f;
						if (v < 0.f)
							v = 0.f;

						m_layout->m_viewports.m_data[0]->m_rect1_0.z = v;
						m_layout->m_viewports.m_data[1]->m_rect1_0.x = v;
						m_layout->m_viewports.m_data[2]->m_rect1_0.z = v;
						m_layout->m_viewports.m_data[3]->m_rect1_0.x = v;*/
					}
				}

			}break;
			case 1:
			{
			}break;
			case 2:
			{
			}break;
			}
		}
	}
};

void UVCameraOnMoveToSelection(AppViewportCamera* c) 
{
	c->m_positionPlatform.x = 0.5f;
	c->m_positionPlatform.z = 0.5f;
	c->m_positionPlatform.w = 2.f;
}

void Application::_initViewports()
{
	const float32_t midX = 0.4f;
	for (int32_t i = 0; i < AppViewportLayout_Count; ++i)
	{
		m_viewportLayouts[i] = new AppViewportLayout;
		switch (i)
		{
		default:
			alLog::PrintError("-error- [%s][%s][%i]:(\n", AL_FILE, AL_FUNCTION, AL_LINE );
			break;
		case AppViewportLayout_Full:
			m_viewportLayouts[i]->Add(alVec4f(0.f, 0.f, 1.f, 1.f), AppViewportCameraType::Perspective, AppViewportType::Scene);
			break;
		case AppViewportLayout_Standart: {
			m_viewportLayouts[i]->Add(alVec4f(0.f, 0.f, midX, 0.5f), AppViewportCameraType::Top, AppViewportType::Scene);
			m_viewportLayouts[i]->Add(alVec4f(midX, 0.f, 1.0f, 0.5f), AppViewportCameraType::Left, AppViewportType::Scene);
			m_viewportLayouts[i]->Add(alVec4f(0.f, 0.5f, midX, 1.0f), AppViewportCameraType::Front, AppViewportType::Scene);
			m_viewportLayouts[i]->Add(alVec4f(midX, 0.5f, 1.0f, 1.0f), AppViewportCameraType::Perspective, AppViewportType::Scene);

			m_viewportLayouts[i]->AddResizer(new AppViewportResizer_Standart(2), AppCursorType::Size);
			m_viewportLayouts[i]->AddResizer(new AppViewportResizer_Standart(0), AppCursorType::SizeWE);
			m_viewportLayouts[i]->AddResizer(new AppViewportResizer_Standart(1), AppCursorType::SizeNS);
		}break;
		}
	}

	m_UVViewport = new AppViewportLayout;
	auto uvViewport = m_UVViewport->Add(alVec4f(0.f, 0.f, midX, 1.f), AppViewportCameraType::Top, AppViewportType::UV);
	m_UVViewport->Add(alVec4f(midX, 0.f, 1.f, 1.f), AppViewportCameraType::Perspective, AppViewportType::Scene);
	uvViewport->m_activeCamera->m_onMoveToSelection = UVCameraOnMoveToSelection;
	uvViewport->m_activeCamera->m_onMoveToSelection(uvViewport->m_activeCamera);

	m_activeViewportLayout = m_viewportLayouts[AppViewportLayout_Standart];
	m_activeViewportLayout->ShowGUI();

	m_viewportUnderCursor = m_activeViewportLayout->m_activeViewport;
}

void Application::UpdateViewports()
{
	auto input = alLib::GetInput();

//	if (m_gizmoModeUV != AppGizmoUVMode::NoTransform)
//	{
//		if (input->m_isLMBUp || input->m_isRMBUp)
//		{
//			if (input->m_isRMBUp)
//			{
//				UVTransformCancel();
//			}
//			else if (input->m_isLMBUp)
//			{
//				UVTransformAccept();
//	//			miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
//	//			m_cursors[(u32)miCursorType::Arrow]->Activate();
//			}
//			m_UVAabbMoveOffset.Set(0.f);
//
//			m_gizmoModeUV = AppGizmoUVMode::NoTransform;
//			m_gizmoMode = AppGizmoMode::NoTransform;
//			m_UVAabb.Reset();
//			for (size_t i = 0; i < m_selectedObjects.m_size; ++i)
//			{
//	//			m_selectedObjects.m_data[i]->UVUpdateAAABB(&m_UVAabb);
//			}
//			m_UVAabbOnClick = m_UVAabb;
//			m_viewportInMouseFocus = 0;
//			m_isViewportInFocus = false;
//			return;
//		}
//		if (m_isCursorMove && !input->m_isRMBUp)
//		{
//			UVTransform();
//		}
//	}

	if (m_isCursorInViewport)
	{
		if (input->m_isRMBUp && m_gizmoMode == AppGizmoMode::NoTransform
			&& !m_isCursorInUVEditor)
		{
//			miPopup* p = _getPopupInViewport();
//			ShowPopupAtCursor(p);
//			delete p;
		}
		if (m_gizmoMode == AppGizmoMode::NoTransform)
		{
			if (m_isCursorMove)
			{
				const float32_t _size = 6.f;
				m_selectionFrust.CreateWithFrame(
					alVec4f(
						input->m_cursorCoords.x - _size,
						input->m_cursorCoords.y - _size,
						input->m_cursorCoords.x + _size,
						input->m_cursorCoords.y + _size),
					m_viewportUnderCursor->m_rect,
					m_viewportUnderCursor->m_activeCamera->m_viewProjectionInvertMatrix);
				if (m_isCursorInViewport && !m_isCursorInUVEditor)
				{
					if (m_editMode == AppEditMode::Vertex)
					{
					//	_isObjectMouseHover();
						if (m_mouseMode != AppMouseMode::SelectVertex)
						{
							if (m_mouseHoverVertex)
							{
		//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Cross]);
		//						m_cursors[(u32)miCursorType::Cross]->Activate();
							}
							else
							{
		//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
		//						m_cursors[(u32)miCursorType::Arrow]->Activate();
							}
						}
					}
					else if (m_editMode == AppEditMode::Edge)
					{
					//	_isObjectMouseHover();
						if (m_mouseHoverEdge)
						{
	//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Cross]);
	//						m_cursors[(u32)miCursorType::Cross]->Activate();
						}
						else
						{
	//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
	//						m_cursors[(u32)miCursorType::Arrow]->Activate();
						}
					}
				}
			}
		}
	}

	if (m_isViewportInFocus/* && (_isDoNotSelect() == false)*/)
	{
		if (input->m_isLMBUp)
		{
			if (m_gizmoMode == AppGizmoMode::NoTransform)
			{
				_trySelect();
			}
			else
			{
		//		m_gizmo->OnRelease();
		//		_transformObjectsApply();
		//		UpdateSceneAabb();
		//		UpdateSelectionAabb();
			}
		}

		//if (m_gizmoMode != AppGizmoMode::NoTransform && m_activeViewportLayout->m_activeViewport->m_viewportType == AppViewportType::Scene)
		//{
		//	bool _transform = m_isCursorMove;
		//	if (input->m_isRMBUp || input->IsKeyHit(miKey::K_ESCAPE))
		//	{
		//		m_isViewportInFocus = false;
		//		m_gizmo->OnEscape();
		//		_transformObjectsReset();
		//	}
		//	else
		//	{
		//		static alKeyboardModifier prevKbm = alKeyboardModifier::None;
		//		if (input->m_kbm == alKeyboardModifier::Shift && prevKbm != alKeyboardModifier::Shift)
		//		{
		//			_transform = true;
		//			//	m_gizmo->m_var_rotation_add = -m_gizmo->m_var_rotate_snap;
		//				//m_gizmo->m_var_rotate_snap = 0.f;
		//		}
		//		prevKbm = input->m_kbm;

		//		if (_transform)
		//			_transformObjects();
		//	}
		//}
	}

//	if (m_gizmoMode == AppGizmoMode::NoTransform && m_mouseMode == AppMouseMode::CommonMode)
//	{
//		if (input->IsKeyHit(miKey::K_ESCAPE))
//		{
//			DeselectAll();
//		}
//	}
//
//	if (m_mouseMode == AppMouseMode::SelectObject
//		|| m_mouseMode == AppMouseMode::SelectVertex
//		|| m_mouseMode == AppMouseMode::Other)
//	{
//		if (input->IsKeyHit(miKey::K_ESCAPE) || input->m_isRMBUp)
//		{
//			SetMouseMode(AppMouseMode::CommonMode);
//			CallPluginGUIOnCancel();
//		}
//	}


	if (m_isSelectByRectangle)
	{
		if (input->m_isLMBUp
			|| input->m_isRMBUp
			|| input->m_isRMBDown
			|| input->m_isMMBDown
			|| input->m_isX1MBDown
			|| input->m_isX2MBDown
			|| input->IsKeyHit(alInputKey::K_ESCAPE))
		{
			m_isSelectByRectangle = false;
			m_isViewportInFocus = false;
		}
	}

//	if (m_isClickAndDrag)
//	{
//		if (input->m_isLMBUp
//			|| input->m_isRMBUp
//			|| input->m_isRMBDown
//			|| input->m_isMMBDown
//			|| input->m_isX1MBDown
//			|| input->m_isX2MBDown
//			|| input->IsKeyHit(miKey::K_ESCAPE))
//		{
//			m_isClickAndDrag = false;
//			m_isViewportInFocus = false;
//
//			if (input->m_isLMBUp)
//			{
//			}
//		}
//	}

	if (input->m_isLMBUp)
	{
		m_viewportResizeMode = false;
		m_viewportResizer = 0;
	}

	m_isCursorInViewport = false;
	for (size_t i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
	{
		auto viewport = m_activeViewportLayout->m_viewports[i];
		viewport->m_textNameColor = ColorWhite;
		viewport->m_isCursorInRect =
			alMath::PointInRect(input->m_cursorCoords,viewport->m_rect);
		if (viewport->m_isCursorInRect)
		{
			m_isCursorInViewport = true;
			m_viewportUnderCursor = viewport;
			m_isCursorInGUI = false;
		}
	}

	if (m_isCursorInViewport)
	{
		if (alMath::PointInRect(m_input->m_cursorCoordsForGUI, m_viewportUnderCursor->m_textNameRect))
		{
			m_viewportUnderCursor->m_textNameColor = ColorYellow;
			// наверное лучше вызывать popup меню нажатием ПКМ
			//m_isCursorInGUI = true;
			if (input->m_isRMBDown)
			{
				SetActiveViewport(m_viewportUnderCursor);
				ShowViewportPopup();
			}
		}
	}

	if (!m_isCursorInGUI)
	{
		if (input->m_isLMBDown)
			m_cursorLMBClickPosition = input->m_cursorCoordsForGUI;
		bool cursorInResizer = false;

		/*AppViewportResizer* rszr = 0;
		for (size_t k = 0; k < m_activeViewportLayout->m_resizers.m_size; ++k)
		{
			rszr = m_activeViewportLayout->CursorInResizer(input);
			if (rszr)
			{
				cursorInResizer = true;
				m_currentCursor = rszr->m_cursorType;
				OnSetCursor();

				if (input->m_isLMBDown)
				{
					m_viewportResizeMode = true;
					m_viewportResizer = rszr;
				}
				break;
			}
		}*/
		if (m_viewportResizer)
		{
			m_viewportResizer->OnResize();
			_callViewportOnWindowSize();
		}

		if (!cursorInResizer && !m_viewportResizeMode)
		{
			for (size_t i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
			{
				auto viewport = m_activeViewportLayout->m_viewports[i];

				viewport->m_isCursorInRect =
					alMath::PointInRect(input->m_cursorCoords.x, input->m_cursorCoords.y,
						viewport->m_rect);

				if (viewport->m_isCursorInRect)
				{
				

					GetRayFromScreen(&m_screenRayCurrent, input->m_cursorCoordsForGUI,
						m_activeViewportLayout->m_activeViewport->m_rect,
						m_activeViewportLayout->m_activeViewport->m_activeCamera->m_viewProjectionInvertMatrix);


					m_isCursorInUVEditor = false;
					if (viewport->m_viewportType == AppViewportType::UV)
					{
						m_isCursorInUVEditor = true;
					}


					if (input->m_wheelDelta)
						viewport->m_activeCamera->Zoom();

					if (input->m_isLMBDown
						|| input->m_isMMBDown
						|| input->m_isRMBDown
						|| input->m_isX1MBDown
						|| input->m_isX2MBDown)
					{
						SetActiveViewport(viewport);
						/*if (m_activeViewportLayout->m_activeViewport != viewport)
							m_activeViewportLayout->m_activeViewport = viewport;*/
					}
				}
			}
		}
	}


//	if (input->m_isLMBDown && !m_isCursorInGUI)
//	{
//		m_cursorLMBClickPosition3D = m_activeViewportLayout->m_activeViewport->GetCursorRayHitPosition(input->m_cursorCoords);
//		m_screenRayOnClick = m_screenRayCurrent;
//		m_cursorPosition3D = m_cursorLMBClickPosition3D;
//		//printf("%f %f %f\n", m_cursorLMBClickPosition3D.x, m_cursorLMBClickPosition3D.y, m_cursorLMBClickPosition3D.z);
//	}
//
//
//
//	static bool is_pan_move = false;
//	if (input->m_isMMBHold && m_isViewportInFocus)
//	{
//		switch (input->m_kbm)
//		{
//		default:
//			is_pan_move = true;
//			miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Size]);
//			m_cursors[(u32)miCursorType::Size]->Activate();
//			break;
//		case alKeyboardModifier::Alt:
//			break;
//		case alKeyboardModifier::CtrlAlt:
//			break;
//		case alKeyboardModifier::ShiftCtrlAlt:
//			break;
//		}
//	}
//	else
//	{
//		if (is_pan_move)
//		{
//			is_pan_move = false;
//			SetMouseMode(m_mouseMode);
//		}
//	}
//
//	if (m_mouseMode == AppMouseMode::HideCursor)
//	{
//		if (m_isCursorMove)
//			miSetCursorPosition(m_cursorLMBClickPosition.x, m_cursorLMBClickPosition.y, m_mainWindow);
//
//		if (input->m_isLMBUp || input->m_isRMBUp || input->IsKeyHit(miKey::K_ESCAPE))
//			this->SetMouseMode(AppMouseMode::CommonMode);
//	}

	if (m_isViewportInFocus)
	{
		if (m_isCursorMove)
		{
			m_cursorPosition3D = m_activeViewportLayout->m_activeViewport->GetCursorRayHitPosition(input->m_cursorCoordsForGUI);

			if (m_gizmoMode == AppGizmoMode::NoTransform)
			{
				if ((m_mouseMode == AppMouseMode::CommonMode) ||
					(m_mouseMode == AppMouseMode::Other))
				{
					if (input->m_isLMBHold)
						m_isSelectByRectangle = true;
				}

				if (
					(m_mouseMode == AppMouseMode::ClickAndDrag && input->m_isLMBHold)
					|| (m_mouseMode == AppMouseMode::SelectVertex && input->m_isLMBHold))
				{
					//if (!m_isClickAndDrag)
					//	m_isClickAndDrag = true;
				}
			}

		}
		if (input->m_isRMBHold)
		{
			switch (input->m_kbm)
			{
			case alKeyboardModifier::Ctrl:
				if (m_isCursorMove)
					m_activeViewportLayout->m_activeViewport->PanMove();
				m_currentCursor = AppCursorType::HandGrab;
				break;
			case alKeyboardModifier::CtrlAlt:
			case alKeyboardModifier::Alt:
				if (m_isCursorMove)
					m_activeViewportLayout->m_activeViewport->Rotate(input->m_mouseDelta.x, input->m_mouseDelta.y);
				m_currentCursor = AppCursorType::Rotate;
				break;
			case alKeyboardModifier::ShiftAlt:
				if (m_isCursorMove)
					m_activeViewportLayout->m_activeViewport->ChangeFOV();
				break;
			case alKeyboardModifier::ShiftCtrlAlt:
				if (m_isCursorMove)
					m_activeViewportLayout->m_activeViewport->RotateZ();
				m_currentCursor = AppCursorType::Rotate;
				break;
			}
			OnSetCursor();
		}
	}

	if (input->IsKeyHit(alInputKey::K_NUM_4))
		m_activeViewportLayout->m_activeViewport->Rotate(-5.f, 0.f);
	if (input->IsKeyHit(alInputKey::K_NUM_6))
		m_activeViewportLayout->m_activeViewport->Rotate(5.f, 0.f);
	if (input->IsKeyHit(alInputKey::K_NUM_2))
		m_activeViewportLayout->m_activeViewport->Rotate(0.f, -5.f);
	if (input->IsKeyHit(alInputKey::K_NUM_8))
		m_activeViewportLayout->m_activeViewport->Rotate(0.f, 5.f);
	// this now in shortcut manager
	/*if (input->IsKeyHit(alInputKey::K_NUM_5))
	{
		m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho =
		m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho ? false : true;
		m_activeViewportLayout->m_activeViewport->Rotate(0.f, 0.f);
	}*/

	if (m_isCursorInWindow && !m_isCursorInGUI)
	{
		if (input->m_isMMBDown || input->m_isLMBDown || input->m_isRMBDown)
		{
			if (!m_isViewportInFocus)
			{
				m_isViewportInFocus = true;
				m_viewportInMouseFocus = m_viewportUnderCursor;
			}
		}
	}

	if (input->m_isLMBUp || input->m_isMMBUp || input->m_isRMBUp)
	{
		m_isViewportInFocus = false;
		m_viewportInMouseFocus = 0;
	}

//	switch (m_gizmoMode)
//	{
//	case AppGizmoMode::RotateX:
//	case AppGizmoMode::RotateY:
//	case AppGizmoMode::RotateZ:
//	case AppGizmoMode::RotateScreen:
//	{
//		/*static alKeyboardModifier prevKbm = alKeyboardModifier::None;
//		if (input->m_kbm == alKeyboardModifier::Shift && prevKbm != alKeyboardModifier::Shift)
//		{
//			printf("shift\n");
//		}
//		prevKbm = input->m_kbm;*/
//
//		//printf("%f %f\n", m_cursorLMBClickPosition.x, m_cursorLMBClickPosition.y);
//		if (!m_GUI->m_txt_rotationAngle->m_visible)
//			m_GUI->m_txt_rotationAngle->SetVisible(true);
//		m_GUI->m_txt_rotationAngle->m_offset.x = m_cursorLMBClickPosition.x;
//		m_GUI->m_txt_rotationAngle->m_offset.y = m_cursorLMBClickPosition.y;
//		m_GUI->m_txt_rotationAngle->UpdateTransform();
//		//m_activeViewportLayout->m_activeViewport->
//	}break;
//	default:
//		break;
//	}
	//if (m_isViewportInFocus) printf("a");
}

//miPopup* _getPopupInViewport();
void Application::DrawViewports3D()
{
	for (size_t i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
	{
		auto viewport = m_activeViewportLayout->m_viewports[i];
		viewport->Draw3D();
	}
}
void Application::DrawViewportsGUI()
{
	for (size_t i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
	{
		m_gs->SetScissorRect(alVec4f(0.f, 0.f, (float32_t)m_mainWindow->m_clientSize.x, (float32_t)m_mainWindow->m_clientSize.y));
		m_gs->SetViewport(0, 0, m_mainWindow->m_clientSize.x, m_mainWindow->m_clientSize.y);

		auto viewport = m_activeViewportLayout->m_viewports[i];
		if (viewport == m_activeViewportLayout->m_activeViewport)
		{
			alVec4f rect; ;
			rect.x = viewport->m_rect.x;
			rect.y = viewport->m_rect.y;
			rect.z = viewport->m_rect.z;
			rect.w = viewport->m_rect.w;
			rect.x -= AppViewportBorderSize;
			rect.y -= AppViewportBorderSize;
			rect.z += AppViewportBorderSize;
			rect.w += AppViewportBorderSize;
			m_gs->DisableDepth();
			m_gs->DrawRectangle(rect, m_colorThemeCurr->m_viewportBorder);
		}

		if (!m_isCursorInUVEditor)
		{
			switch (m_editMode)
			{
			case AppEditMode::Object:
				if (m_selectedObjects.m_size)
					m_gizmo->Update(viewport);
				break;
			default:
			//	if (m_isVertexEdgePolygonSelected)
			//		m_gizmo->Update(viewport);
				break;
			}

			if (m_isCursorInViewport && viewport == m_viewportUnderCursor)
			{
			//	if (m_isGizmoMouseHover)
			//	{
			//		if (input->m_isLMBDown)
			//			m_gizmo->OnClick();
			//	}
			}
		}

		//viewport->OnDraw();
		m_gs->DisableDepth();

		
		m_gs->ActivateGUIShader();
		m_gs->DrawRectangle(viewport->m_rect, m_colorThemeCurr->m_viewportColor, 0, 0);
		m_gs->DrawRectangle(viewport->m_rect, ColorWhite, viewport->m_rtt, 0);

		if (viewport == m_activeViewportLayout->m_activeViewport)
		{
			if (m_isSelectByRectangle)
			{

				m_gs->SetScissorRect(viewport->m_rect);

				auto p1 = m_cursorLMBClickPosition;
				alVec2f p2 = m_input->m_cursorCoordsForGUI;
				alAabb aabb;
				aabb.Add(alVec3f(p1.x + 1.f, p1.y + 1.f, 0.f));
				aabb.Add(alVec3f(p2.x - 1.f, p2.y - 1.f, 0.f));
				m_gs->BeginDrawLine2D();
				m_gs->DrawLine2D(alVec2f(p1.x, p1.y), alVec2f(p2.x, p1.y), ColorWhite);
				m_gs->DrawLine2D(alVec2f(p1.x, p1.y), alVec2f(p1.x, p2.y), ColorWhite);
				m_gs->DrawLine2D(alVec2f(p1.x, p2.y), alVec2f(p2.x, p2.y), ColorWhite);
				m_gs->DrawLine2D(alVec2f(p2.x, p1.y), alVec2f(p2.x, p2.y), ColorWhite);
				m_gs->SetViewport(0, 0, m_mainWindow->m_clientSize.x, m_mainWindow->m_clientSize.y);
				m_gs->ActivateGUIShader();
			}
		}
		
		{
			
			m_gs->ActivateGUIShader();
			m_gs->DrawText(viewport->m_name.c_str(), 
				viewport->m_name.size(),
				m_fontGUI, viewport->m_textNamePos, viewport->m_textNameColor);
		}

		/*for (size_t k = 0; k < m_activeViewportLayout->m_resizers.m_size; ++k)
		{
			m_gs->DrawRectangle(m_activeViewportLayout->m_resizers.m_data[k]->m_rect, ColorRed);
		}*/
		
	}
	
	m_gs->SetViewport(0, 0, m_mainWindow->m_clientSize.x, m_mainWindow->m_clientSize.y);
	m_gs->SetScissorRect(alVec4f(0.f, 0.f, (float32_t)m_mainWindow->m_clientSize.x, (float32_t)m_mainWindow->m_clientSize.y));
}

void Application::_callViewportOnWindowSize()
{
	for (int32_t k = 0; k < AppViewportLayout_Count; ++k)
	{
		auto l = m_viewportLayouts[k];
		for (size_t i = 0, sz = l->m_viewports.size(); i < sz; ++i)
		{
			l->m_viewports[i]->OnWindowSize();
		}
	}
	for (size_t i = 0, sz = m_UVViewport->m_viewports.size(); i < sz; ++i)
	{
		m_UVViewport->m_viewports[i]->OnWindowSize();
	}

	for (size_t i = 0; i < AppViewportLayout_Count; ++i)
	{
		m_viewportLayouts[i]->Rebuild();
	}
}

AppViewportCamera* Application::GetActiveCamera()
{
	return m_activeViewportLayout->m_activeViewport->m_activeCamera;
}

void Application::SetActiveViewport(AppViewport* vp)
{
	if (vp)
	{
		for (size_t i = 0; i < m_activeViewportLayout->m_viewports.m_size; ++i)
		{
			if (m_activeViewportLayout->m_viewports.m_data[i] == vp)
			{
				if (m_activeViewportLayout->m_activeViewport != vp)
					m_activeViewportLayout->m_activeViewport = vp;
			}
		}
	}
}