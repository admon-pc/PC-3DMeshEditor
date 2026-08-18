#include "PluginInterface.h"
#include "std.object.plane.h"

PluginObject_plane* g_plugin = 0;

extern "C"
{
	_declspec(dllexport) Plugin* AL_CDECL PluginLoad(PluginInterface* ei)
	{
		if(!g_plugin)
			g_plugin = new PluginObject_plane(ei);
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

PluginObject_plane::PluginObject_plane(PluginInterface* i)
	:
	PluginObject(i)
{
}

PluginObject_plane::~PluginObject_plane()
{
}

const char32_t* PluginObject_plane::Name()
{
	return U"Plane plugin";
}

const char32_t* PluginObject_plane::Desc()
{
	return U"Basic 2D plane mesh";
}

const char32_t* PluginObject_plane::Author()
{
	return U"Artemy Basov";
}

const char32_t* PluginObject_plane::Copyright()
{
	return U"Artemy Basov";
}

uint32_t PluginObject_plane::Version()
{
	return 1;
}

uint32_t PluginObject_plane::SDKVersion()
{
	return PLUGIN_SDK_VERSION;
}

PluginClassID PluginObject_plane::PluginType()
{
	return PLUGIN_CLASS_ID_PLUGIN_TYPE_OBJECT;
}


const char32_t* PluginObject_plane::Category()
{
	return U"Polygonal";
}

const char32_t* PluginObject_plane::SubCategory()
{
	return U"Basic shapes";
}

const char32_t* PluginObject_plane::TitleName()
{
	return U"Plane";
}

PluginClassID PluginObject_plane::ClassID()
{
	return PLUGIN_CLASS_ID_OBJECT_PLANE;
}

