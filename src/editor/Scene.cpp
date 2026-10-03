#include "editor.h"
#include "Scene.h"

extern Application* g_app;


AppSceneImpl::_root_t::_root_t():AppSceneObject(0){}
AppSceneImpl::_root_t::~_root_t(){}

AppSceneImpl::AppSceneImpl()
{
	m_rootObject = new _root_t();
	m_rootObject->SetName(U"Root");
}

AppSceneImpl::~AppSceneImpl()
{
	ClearScene();
	delete m_rootObject;
}

void AppSceneImpl::DeleteObject(AppSceneObject* o)
{
	if (o == m_rootObject)
		return;

	o->SetParent(0);
	o->GetPluginObject()->DestroyObject(o);
	_updateOnSelectObject();
}

void AppSceneImpl::ClearScene()
{
	AppArray<AppSceneObject*> arr;
	GetAllObjects(&arr);
	for (size_t i = 0; i < arr.m_size; ++i)
	{
		DeleteObject(arr.m_data[i]);
	}

	m_rootObject->ClearChildrenList();
	g_app->UpdateObjectList();
	_updateOnSelectObject();
}

void AppSceneImpl::AddObject(AppSceneObject* object)
{
	if (object)
	{
		if (object->m_flags & AppSceneObject::flag_addedToScene)
			return;

		alUnicodeString name = object->GetName();
		GetFreeName(&name);
		object->SetName(name.c_str());

		object->SetParent(m_rootObject);
		object->m_flags |= AppSceneObject::flag_addedToScene;
		object->UpdateAabb();
		g_app->UpdateObjectList();
		_onGetAllObjectsIntoArray();
	}
}

void AppSceneImpl::_onGetAllObjects(AppSceneObject* o)
{
	m_getAllObjectArrayPtr->push_back(o);

	auto& children = o->GetChildren();
	auto node = children.m_head;
	if (node)
	{
		auto last = node->m_left;
		while (true)
		{
			_onGetAllObjects(node->m_data);

			if (node == last)
				break;

			node = node->m_right;
		}
	}
}

void AppSceneImpl::_onGetAllObjectsIntoArray()
{
	GetAllObjects(&m_allObjectsOnScene);
}

void AppSceneImpl::GetAllObjects(AppArray<AppSceneObject*>* out)
{
	m_getAllObjectArrayPtr = out;
	m_getAllObjectArrayPtr->clear();

	auto& children = m_rootObject->GetChildren();
	auto node = children.m_head;
	if (node)
	{
		auto last = node->m_left;
		while (true)
		{
			_onGetAllObjects(node->m_data);

			if (node == last)
				break;

			node = node->m_right;
		}
	}
}

void AppSceneImpl::GetFreeName(alUnicodeString* str)
{
	alUnicodeString name = *str;
	uint32_t nameNumber = 0;

	while (true)
	{
		if (IsNameFree(m_rootObject, &name))
		{
			break;
		}
		else
		{
			name = *str;
			name.Append(nameNumber);
			++nameNumber;
		}
	}
	*str = name;
}

bool AppSceneImpl::IsNameFree(AppSceneObject* o, alUnicodeString* name)
{
	if (alLib::strcmp(o->GetName(), name->c_str()) == 0)
		return false;

	auto& children = o->GetChildren();
	auto node = children.m_head;
	if (node)
	{
		auto last = node->m_left;
		while (true)
		{
			if (!IsNameFree(node->m_data, name))
				return false;

			if (node == last)
				break;

			node = node->m_right;
		}
	}

	return true;
}

AppSceneObject* AppSceneImpl::GetRootObject()
{
	return m_rootObject;
}

void AppSceneImpl::DeselectAll()
{
	for (auto o : m_allObjectsOnScene)
	{
		o->m_isSelected = false;
	}
	_updateOnSelectObject();
}

void AppSceneImpl::SelectObject(AppSceneObject* o)
{
	o->m_isSelected = true;
	_updateOnSelectObject();
}

void AppSceneImpl::_updateOnSelectObject()
{
	m_selectedObjects.clear();
	for (auto o : m_allObjectsOnScene)
	{
		if (o->m_isSelected)
		{
			m_selectedObjects.push_back(o);
		}
	}
}

//void AppSceneImpl::Update(float32_t dt)
//{
//}
//
//void AppSceneImpl::Draw(float32_t dt)
//{
//}

