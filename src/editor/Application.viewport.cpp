#include "editor.h"

void UVCameraOnMoveToSelection(AppViewportCamera* c) 
{
	c->m_positionPlatform.x = 0.5f;
	c->m_positionPlatform.z = 0.5f;
	c->m_positionPlatform.w = 2.f;
}

void Application::_initViewports()
{
	const float32_t midX = 0.4;
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
//	auto input = alLib::GetInput();
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
//
//	if (m_isCursorInViewport)
//	{
//		if (input->m_isRMBUp && m_gizmoMode == AppGizmoMode::NoTransform
//			&& !m_isCursorInUVEditor)
//		{
////			miPopup* p = _getPopupInViewport();
////			ShowPopupAtCursor(p);
////			delete p;
//		}
//		if (m_gizmoMode == AppGizmoMode::NoTransform)
//		{
//			if (m_isCursorMove)
//			{
//				const float32_t _size = 6.f;
//				m_selectionFrust->CreateWithFrame(
//					alVec4f(
//						input->m_cursorCoords.x - _size,
//						input->m_cursorCoords.y - _size,
//						input->m_cursorCoords.x + _size,
//						input->m_cursorCoords.y + _size),
//					m_viewportUnderCursor->m_currentRect,
//					m_viewportUnderCursor->m_activeCamera->m_viewProjectionInvertMatrix);
//				if (m_isCursorInViewport && !m_isCursorInUVEditor)
//				{
//					if (m_editMode == AppEditMode::Vertex)
//					{
//						_isObjectMouseHover();
//						if (m_mouseMode != AppMouseMode::SelectVertex)
//						{
//							if (m_mouseHoverVertex)
//							{
//		//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Cross]);
//		//						m_cursors[(u32)miCursorType::Cross]->Activate();
//							}
//							else
//							{
//		//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
//		//						m_cursors[(u32)miCursorType::Arrow]->Activate();
//							}
//						}
//					}
//					else if (m_editMode == AppEditMode::Edge)
//					{
//						_isObjectMouseHover();
//						if (m_mouseHoverEdge)
//						{
//	//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Cross]);
//	//						m_cursors[(u32)miCursorType::Cross]->Activate();
//						}
//						else
//						{
//	//						miSetCursor(miCursorType::Arrow, m_cursors[(u32)miCursorType::Arrow]);
//	//						m_cursors[(u32)miCursorType::Arrow]->Activate();
//						}
//					}
//				}
//			}
//		}
//	}
//
//	if (m_isViewportInFocus && (_isDoNotSelect() == false))
//	{
//		if (input->m_isLMBUp)
//		{
//			if (m_gizmoMode == AppGizmoMode::NoTransform)
//			{
//				_select();
//			}
//			else
//			{
//				m_gizmo->OnRelease();
//				_transformObjectsApply();
//				UpdateSceneAabb();
//				UpdateSelectionAabb();
//			}
//		}
//
//		if (m_gizmoMode != AppGizmoMode::NoTransform && m_activeViewportLayout->m_activeViewport->m_viewportType == AppViewportType::Scene)
//		{
//			bool _transform = m_isCursorMove;
//			if (input->m_isRMBUp || input->IsKeyHit(miKey::K_ESCAPE))
//			{
//				m_isViewportInFocus = false;
//				m_gizmo->OnEscape();
//				_transformObjectsReset();
//			}
//			else
//			{
//				static miKeyboardModifier prevKbm = miKeyboardModifier::None;
//				if (input->m_kbm == miKeyboardModifier::Shift && prevKbm != miKeyboardModifier::Shift)
//				{
//					_transform = true;
//					//	m_gizmo->m_var_rotation_add = -m_gizmo->m_var_rotate_snap;
//						//m_gizmo->m_var_rotate_snap = 0.f;
//				}
//				prevKbm = input->m_kbm;
//
//				if (_transform)
//					_transformObjects();
//			}
//		}
//	}
//
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
//
//
//	if (m_isSelectByRectangle)
//	{
//		if (input->m_isLMBUp
//			|| input->m_isRMBUp
//			|| input->m_isRMBDown
//			|| input->m_isMMBDown
//			|| input->m_isX1MBDown
//			|| input->m_isX2MBDown
//			|| input->IsKeyHit(miKey::K_ESCAPE))
//		{
//			m_isSelectByRectangle = false;
//			m_isViewportInFocus = false;
//		}
//	}
//
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
//
//	m_isCursorInViewport = false;
//	if (!m_isCursorInGUI)
//	{
//		if (input->m_isLMBDown)
//			m_cursorLMBClickPosition = input->m_cursorCoords;
//
//		for (u16 i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
//		{
//			auto viewport = m_activeViewportLayout->m_viewports[i];
//
//			viewport->m_isCursorInRect =
//				math::pointInRect(input->m_cursorCoords.x, input->m_cursorCoords.y,
//					viewport->m_currentRect);
//
//			if (viewport->m_isCursorInRect)
//			{
//				m_sdk->GetRayFromScreen(&m_screenRayCurrent, input->m_cursorCoords,
//					m_activeViewportLayout->m_activeViewport->m_currentRect,
//					m_activeViewportLayout->m_activeViewport->m_activeCamera->m_viewProjectionInvertMatrix);
//
//				m_isCursorInViewport = true;
//				m_viewportUnderCursor = viewport;
//
//				m_isCursorInUVEditor = false;
//				if (viewport->m_viewportType == AppViewportType::UV)
//				{
//					m_isCursorInUVEditor = true;
//				}
//
//
//				if (input->m_wheelDelta)
//					viewport->m_activeCamera->Zoom();
//
//				if (input->m_isLMBDown
//					|| input->m_isMMBDown
//					|| input->m_isRMBDown
//					|| input->m_isX1MBDown
//					|| input->m_isX2MBDown)
//				{
//					if (m_activeViewportLayout->m_activeViewport != viewport)
//						m_activeViewportLayout->m_activeViewport = viewport;
//				}
//			}
//		}
//	}
//
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
//		case miKeyboardModifier::Alt:
//			break;
//		case miKeyboardModifier::CtrlAlt:
//			break;
//		case miKeyboardModifier::ShiftCtrlAlt:
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
//
//	if (m_isCursorMove && m_isViewportInFocus)
//	{
//		m_cursorPosition3D = m_activeViewportLayout->m_activeViewport->GetCursorRayHitPosition(input->m_cursorCoords);
//
//		if (m_gizmoMode == AppGizmoMode::NoTransform)
//		{
//			if ((m_mouseMode == AppMouseMode::CommonMode) ||
//				(m_mouseMode == AppMouseMode::Other))
//			{
//				if (input->m_isLMBHold)
//					m_isSelectByRectangle = true;
//			}
//
//			if (
//				(m_mouseMode == AppMouseMode::ClickAndDrag && input->m_isLMBHold)
//				|| (m_mouseMode == AppMouseMode::SelectVertex && input->m_isLMBHold))
//			{
//				if (!m_isClickAndDrag)
//					m_isClickAndDrag = true;
//			}
//		}
//
//		if (input->m_isMMBHold)
//		{
//			switch (input->m_kbm)
//			{
//			default:
//				m_activeViewportLayout->m_activeViewport->PanMove();
//				break;
//			case miKeyboardModifier::Alt:
//				m_activeViewportLayout->m_activeViewport->Rotate(input->m_mouseDelta.x, input->m_mouseDelta.y);
//				break;
//			case miKeyboardModifier::CtrlAlt:
//				m_activeViewportLayout->m_activeViewport->ChangeFOV();
//				break;
//			case miKeyboardModifier::ShiftCtrlAlt:
//				m_activeViewportLayout->m_activeViewport->RotateZ();
//				break;
//			}
//		}
//	}
//
//	if (input->IsKeyHit(miKey::K_NUM_4))
//		m_activeViewportLayout->m_activeViewport->Rotate(-5.f, 0.f);
//	if (input->IsKeyHit(miKey::K_NUM_6))
//		m_activeViewportLayout->m_activeViewport->Rotate(5.f, 0.f);
//	if (input->IsKeyHit(miKey::K_NUM_2))
//		m_activeViewportLayout->m_activeViewport->Rotate(0.f, -5.f);
//	if (input->IsKeyHit(miKey::K_NUM_8))
//		m_activeViewportLayout->m_activeViewport->Rotate(0.f, 5.f);
//	if (input->IsKeyHit(miKey::K_NUM_5))
//		m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho =
//		m_activeViewportLayout->m_activeViewport->m_activeCamera->m_forceOrtho ? false : true;
//
//	if (m_isCursorInWindow && !m_isCursorInGUI)
//	{
//		if (input->m_isMMBDown || input->m_isLMBDown)
//		{
//			m_isViewportInFocus = true;
//			m_viewportInMouseFocus = m_viewportUnderCursor;
//		}
//	}
//
//	if (input->m_isLMBUp || input->m_isMMBUp)
//	{
//		m_isViewportInFocus = false;
//		m_viewportInMouseFocus = 0;
//	}
//
//	switch (m_gizmoMode)
//	{
//	case AppGizmoMode::RotateX:
//	case AppGizmoMode::RotateY:
//	case AppGizmoMode::RotateZ:
//	case AppGizmoMode::RotateScreen:
//	{
//		/*static miKeyboardModifier prevKbm = miKeyboardModifier::None;
//		if (input->m_kbm == miKeyboardModifier::Shift && prevKbm != miKeyboardModifier::Shift)
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
void Application::DrawViewports()
{
	/*for (u16 i = 0, sz = m_activeViewportLayout->m_viewports.size(); i < sz; ++i)
	{
		m_gpu->SetScissorRect(alVec4f(0.f, 0.f, (float32_t)m_mainWindow->m_currentSize.x, (float32_t)m_mainWindow->m_currentSize.y), m_mainWindow, 0);
		m_gpu->SetViewport(0.f, 0.f, (float32_t)m_mainWindow->m_currentSize.x, (float32_t)m_mainWindow->m_currentSize.y, m_mainWindow, 0);

		auto viewport = m_activeViewportLayout->m_viewports[i];
		if (viewport == m_activeViewportLayout->m_activeViewport)
		{
			alVec4f rect = viewport->m_currentRect;
			rect.x -= miViewportBorderSize;
			rect.y -= miViewportBorderSize;
			rect.z += miViewportBorderSize;
			rect.w += miViewportBorderSize;
			m_gpu->UseDepth(false);
			m_gpu->DrawRectangle(rect, m_color_viewportBorder, m_color_viewportBorder);
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
				if (m_isVertexEdgePolygonSelected)
					m_gizmo->Update(viewport);
				break;
			}

			if (m_isCursorInViewport && viewport == m_viewportUnderCursor)
			{
				if (m_isGizmoMouseHover)
				{
					if (input->m_isLMBDown)
						m_gizmo->OnClick();
				}
			}
		}

		viewport->OnDraw();

		if (viewport == m_activeViewportLayout->m_activeViewport)
		{
			if (m_isSelectByRectangle)
			{
				m_gpu->SetViewport(0.f, 0.f, (float32_t)m_mainWindow->m_currentSize.x, (float32_t)m_mainWindow->m_currentSize.y, m_mainWindow, 0);
				DrawSelectionBox(m_cursorLMBClickPosition, input->m_cursorCoords);
			}
		}

	}

	m_gpu->SetViewport(0.f, 0.f, (float32_t)m_mainWindow->m_currentSize.x, (float32_t)m_mainWindow->m_currentSize.y, m_mainWindow, 0);
	m_gpu->SetScissorRect(alVec4f(0.f, 0.f, (float32_t)m_mainWindow->m_currentSize.x, (float32_t)m_mainWindow->m_currentSize.y), m_mainWindow, 0);*/
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
}

AppViewportCamera* Application::GetActiveCamera()
{
	return m_activeViewportLayout->m_activeViewport->m_activeCamera;
}

