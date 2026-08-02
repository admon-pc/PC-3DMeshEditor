#include "editor.h"
#include "PluginSTD.h"

extern "C"
{
	_declspec(dllexport) EditorPlugin* AL_CDECL EditorLoadPlugin(EditorInterface* ei)
	{
		EditorPluginSTD* plugin = (EditorPluginSTD*)ei->MemAlloc(sizeof(EditorPluginSTD));
		new(plugin)EditorPluginSTD(ei);
		return plugin;
	}
}

EditorPluginSTD::EditorPluginSTD(EditorInterface* i) 
	:
	EditorPlugin(i) 
{}

EditorPluginSTD::~EditorPluginSTD() 
{
}


const char32_t* EditorPluginSTD::Name()
{
	return U"-";
}

const char32_t* EditorPluginSTD::Desc()
{
	return U"-";
}

const char32_t* EditorPluginSTD::Author()
{
	return U"-";
}

const char32_t* EditorPluginSTD::Copyright()
{
	return U"-";
}

uint32_t EditorPluginSTD::Version()
{
	return 100;
}



uint32_t EditorPluginSTD::ExporterNum()
{
	return 0;
}

uint32_t EditorPluginSTD::ImporterNum()
{
	return 0;
}

uint32_t EditorPluginSTD::ObjectNum()
{
	return 1;
}

EditorPluginObject* EditorPluginSTD::GetPluginObject(uint32_t)
{
	return 0;
}

EditorPluginImport* EditorPluginSTD::GetPluginImport(uint32_t)
{
	return 0;
}

EditorPluginExport* EditorPluginSTD::GetPluginExport(uint32_t)
{
	return 0;
}

uint32_t EditorPluginSTD::SDKVersion()
{
	return APP_SDK_VERSION;
}
