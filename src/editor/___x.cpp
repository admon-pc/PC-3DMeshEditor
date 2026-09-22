#include "editor.h"
#include "AppSceneObjectImpl.h"

extern Application* g_app;

AppSceneObjectImpl::AppSceneObjectImpl(AppPluginObject* po) 
	:
	m_pluginObject(po) 
{}

AppSceneObjectImpl::~AppSceneObjectImpl()
{
}

void AppSceneObjectImpl::SetParent(AppSceneObject* parent)
{
	if (m_parent)
	{
		m_parent->m_children.erase_first(this);
	}

	m_parent = dynamic_cast<AppSceneObjectImpl*>(parent);

	if (m_parent)
		m_parent->m_children.push_back(this);
}

void AppSceneObjectImpl::GetChildren(AppList<AppSceneObject*>& out) 
{
	out.clear();
	if (m_children.m_head)
	{
		auto curr = m_children.m_head;
		auto last = curr->m_left;
		while (true)
		{
			out.push_back(curr->m_data);

			if (curr == last)
				break;

			curr = curr->m_right;
		}
	}
	//return m_children; 
}

void AppSceneObjectImpl::ClearChildrenList()
{
	m_children.clear();
}

bool AppSceneObjectImpl::IsSelected()
{
	return m_isSelected; 
}

// m_aabb хранит оригинальное значение. Он не изменяет своего значения если
// изменили масштаб объекта или покрутили его.
// Для того, чтобы получить Aabb в соответствии с масштабов и вращением
// нужно получить m_aabbTransformed. Это метод GetAABBTransformed().
AppAabb* AppSceneObjectImpl::GetAABB()
{
	return &m_aabb; 
}
AppAabb* AppSceneObjectImpl::GetAABBTransformed()
{
	return &m_aabbTransformed; 
}

AppVec4* AppSceneObjectImpl::GetLocalPosition()
{
	return &m_localPosition; 
}

AppVec4* AppSceneObjectImpl::GetGlobalPosition()
{
	return &m_globalPosition; 
}

AppMat4* AppSceneObjectImpl::GetRotationScaleMatrix()
{
	return &m_rotationScaleMatrix; 
}

AppMat4* AppSceneObjectImpl::GetWVPMatrix()
{
	return &m_worldViewProjection; 
}

AppMat4* AppSceneObjectImpl::GetWMatrix()
{
	return &m_worldMatrix; 
}

void AppSceneObjectImpl::UpdateAabb()
{
	// В старой версии делал вот это.
	// Думаю - нужно ли?
	// m_aabb и есть тот AABB который устанавливался при создании объекта.
	// имея базовое значение, делаем его модифицированную версию - m_aabbTransformed
	// и дальше, используем m_aabbTransformed.
	// Возможно, может потребоваться изменять m_aabb. В каких случаях? В моменте редактирования?
	//  Тогда, просто нужно будет брать, и обновлять m_aabb вручную там где происходит редактирование.
	// Здесь же, происходит обновление m_aabbTransformed в соответствии с матрицей 
	// хранящей вращение и масштаб. Вся эта штука работает для корректного frustum cull.
	// Так-же вероятно как я помню, выбор объекта мышкой так-же происходит с учётом
	//  попал ли луч в m_aabbTransformed.
	// Ну и получается что надо вызывать этот метод после вращения или масштабирования объекта.
	//
	/*m_aabb.reset();
	for (int i = 0, sz = GetVisualObjectCount(); i < sz; ++i)
	{
		m_aabb.add(GetVisualObject(i)->GetAabb());
	}*/

	//m_aabb.m_min += m_globalPosition;//no
	//m_aabb.m_max += m_globalPosition;//no
	// m_aabb must be in space center

	AppMat4 m = m_rotationScaleMatrix;

	m_aabbTransformed = m_aabb;
	m_aabbTransformed.Transform(&m_aabb, &m, &m_globalPosition);
}

AppColor* AppSceneObjectImpl::GetEdgeColor()
{
	return &m_edgeColor; 
}

void AppSceneObjectImpl::SetEdgeColor(const AppColor& c)
{
	m_edgeColor = c; 
}

AppSceneObject* AppSceneObjectImpl::GetParent()
{
	return m_parent; 
}

const char32_t* AppSceneObjectImpl::GetName()
{
	return m_name.Data(); 
}

const wchar_t* AppSceneObjectImpl::GetNameW()
{
	return m_nameW.c_str(); 
}

void AppSceneObjectImpl::SetName(const char32_t* s)
{
	if (s)
	{
		m_name.Assign(s);
	} 
	
	m_name.ToUTF16(m_nameW); 
}

const AppPluginClassID& AppSceneObjectImpl::GetClassID()
{
	return m_classID; 
}

AppPluginObject* AppSceneObjectImpl::GetPluginObject()
{
	return m_pluginObject; 
}

AppSceneObjectType AppSceneObjectImpl::GetSceneObjectType() 
{
	return m_sceneObjectType; 
}
