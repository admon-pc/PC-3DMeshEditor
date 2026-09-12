#include "AppPluginInterface.h"
#include "std.object.plane.h"
#pragma comment(lib, "Plugin.lib")

AppPluginObject_plane* g_plugin = 0;

extern "C"
{
	_declspec(dllexport) AppPlugin* AL_CDECL PluginLoad(AppPluginInterface* ei)
	{
		if(!g_plugin)
			g_plugin = new AppPluginObject_plane(ei);
		return g_plugin;
	}

	_declspec(dllexport) void AL_CDECL PluginUnload()
	{
		if (g_plugin)
		{
			delete g_plugin;
			g_plugin = 0;
		}
	}
}

AppPluginObject_plane::AppPluginObject_plane(AppPluginInterface* i)
	:
	AppPluginObject(i)
{
}

AppPluginObject_plane::~AppPluginObject_plane()
{
}

const char32_t* AppPluginObject_plane::Name()
{
	return U"Plane plugin";
}

const char32_t* AppPluginObject_plane::Desc()
{
	return U"Basic 2D plane mesh";
}

const char32_t* AppPluginObject_plane::Author()
{
	return U"Artemy Basov";
}

const char32_t* AppPluginObject_plane::Copyright()
{
	return U"Artemy Basov";
}

uint32_t AppPluginObject_plane::Version()
{
	return 1;
}

uint32_t AppPluginObject_plane::SDKVersion()
{
	return PLUGIN_SDK_VERSION;
}

AppPluginClassID AppPluginObject_plane::PluginType()
{
	return PLUGIN_CLASS_ID_PLUGIN_TYPE_OBJECT;
}

AppPluginObject::EObjectType AppPluginObject_plane::ObjectType()
{
	return AppPluginObject::EObjectType::EObjectType_Polygonal;
}

const char32_t* AppPluginObject_plane::Category()
{
	return U"Standart";
}

const char32_t* AppPluginObject_plane::TitleName()
{
	return U"Plane";
}

AppPluginClassID AppPluginObject_plane::ClassID()
{
	return PLUGIN_CLASS_ID_OBJECT_PLANE;
}

AppSceneObject* AppPluginObject_plane::CreateObject()
{
	AppSceneObject_plane* o = new AppSceneObject_plane(this);
	o->SetName(U"Plane");
	//GetPluginInterface()->MemAlloc
	return o;
}

void AppPluginObject_plane::DestroyObject(AppSceneObject* o)
{
	if(o)
		delete o;
}

// ====================================================================
AppSceneObject_plane::AppSceneObject_plane(AppPluginObject* po)
	:
	AppSceneObject(po)
{
}

AppSceneObject_plane::~AppSceneObject_plane()
{
}


