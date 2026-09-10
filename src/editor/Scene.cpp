#include "editor.h"

class AppSceneObjectInternalImpl : public AppSceneObjectInternal
{
public:
	AppSceneObjectInternalImpl(AppSceneObject* o):m_sceneObject(o) {}
	virtual ~AppSceneObjectInternalImpl() 
	{
		if (m_sceneObject)
		{
			m_sceneObject->Destroy();
		}
	}

	AppSceneObject* m_sceneObject = 0;
};

AppScene::_root_t::_root_t():AppSceneObject(){}
AppScene::_root_t::~_root_t(){}
void AppScene::_root_t::Destroy() { delete this; }

AppScene::AppScene()
{
	//m_rootObject = new _root_t(pi);
	m_rootObject = new AppSceneObjectInternalImpl(new _root_t());
}

AppScene::~AppScene()
{
	ClearScene();
	delete m_rootObject;
}

void AppScene::ClearScene()
{
}

void AppScene::AddObject(AppSceneObject* object)
{
	if (object)
	{
		auto newObject = new AppSceneObjectInternalImpl(object);

		/*alUnicodeString name = object->GetName();
		GetFreeName(&name);
		object->SetName(name.c_str());

		object->SetParent(m_rootObject);*/
	}
}

void AppScene::GetFreeName(alUnicodeString*)
{
}
