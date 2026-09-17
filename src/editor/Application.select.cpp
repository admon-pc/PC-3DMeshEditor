#include "editor.h"

void Application::_trySelect()
{
	auto viewport = m_activeViewportLayout->m_activeViewport;
	if (viewport->m_visibleObjects.m_size)
	{
		if (m_isSelectByRectangle)
		{
			AppAabb aabb;
			aabb.Add(AppVec3f(m_input->m_cursorCoordsForGUI.x, m_input->m_cursorCoordsForGUI.y, 0.f));
			aabb.Add(AppVec3f(m_cursorLMBClickPosition.x, m_cursorLMBClickPosition.y, 0.f));

			m_selectionFrust.CreateWithFrame(
				alVec4f(aabb.m_min.x, aabb.m_min.y, aabb.m_max.x, aabb.m_max.y),
				viewport->m_rect,
				viewport->m_activeCamera->m_viewProjectionInvertMatrix);

			for (size_t i = 0; i < viewport->m_visibleObjects.m_size; ++i)
			{
				AppRay ray;
				ray.m_origin = Application::AlVecToAppVec(m_screenRayCurrent.m_origin);
				ray.m_direction = Application::AlVecToAppVec(m_screenRayCurrent.m_direction);
				ray.m_end = Application::AlVecToAppVec(m_screenRayCurrent.m_end);

				if (viewport->m_visibleObjects.m_data[i].OnSelect(&m_selectionFrust, &ray, true, m_editMode))
				{
				}
			}
			//_select_multiple();
		}
		else
		{
			for (size_t i = 0; i < viewport->m_visibleObjects.m_size; ++i)
			{
				AppRay ray;
				ray.m_origin = Application::AlVecToAppVec(m_screenRayCurrent.m_origin);
				ray.m_direction = Application::AlVecToAppVec(m_screenRayCurrent.m_direction);
				ray.m_end = Application::AlVecToAppVec(m_screenRayCurrent.m_end);

				if (viewport->m_visibleObjects.m_data[i].OnSelect(&m_selectionFrust, &ray, true, m_editMode))
				{
				}
			}
			//_select_single();
		}
	}
}
