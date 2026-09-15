#include "AppPluginInterface.h"
#include "std.object.plane.h"
#pragma comment(lib, "editor.lib.lib")

//AppPluginObject_plane* g_plugin = 0;
AppPlugin_STD* g_plugin = 0;

extern "C"
{
	_declspec(dllexport) AppPlugin* AL_CDECL AppPluginLoad(AppPluginInterface* ei)
	{
		if(!g_plugin)
			g_plugin = new AppPlugin_STD(ei);
		return g_plugin;
	}

	_declspec(dllexport) void AL_CDECL AppPluginUnload()
	{
		if (g_plugin)
		{
			delete g_plugin;
			g_plugin = 0;
		}
	}
}

AppPlugin_STD::AppPlugin_STD(AppPluginInterface* i)
	:
	AppPlugin(i)
{
	m_objects[0] = new AppPluginObject_plane(this);
}

AppPlugin_STD::~AppPlugin_STD()
{
	delete m_objects[0];
}

const char32_t* AppPlugin_STD::Name()
{
	return U"Standart";
}

const char32_t* AppPlugin_STD::Desc()
{
	return U"Plugin with all basic functuionality";
}

const char32_t* AppPlugin_STD::Copyright()
{
	return U"Artemy Basov";
}

uint32_t AppPlugin_STD::Version()
{
	return 1;
}

uint32_t AppPlugin_STD::SDKVersion()
{
	return APP_SDK_VERSION;
}

AppPluginClassID AppPlugin_STD::PluginID()
{
	return APP_CLASS_ID_PLUGIN_STD;
}

uint32_t AppPlugin_STD::GetObjectPluginNum()
{
	return m_objectsNum;
}

uint32_t AppPlugin_STD::GetImportPluginNum()
{
	return 0;
}

uint32_t AppPlugin_STD::GetExportPluginNum()
{
	return 0;
}

AppPluginObject* AppPlugin_STD::GetObjectPlugin(uint32_t i)
{
	if (i >= m_objectsNum)
		return 0;

	return m_objects[i];
}

AppPluginImport* AppPlugin_STD::GetImportPlugin(uint32_t)
{
	return 0;
}

AppPluginExport* AppPlugin_STD::GetExportPlugin(uint32_t)
{
	return 0;
}



AppPluginObject_plane::AppPluginObject_plane(AppPlugin* plugin)
	:
	AppPluginObject(plugin)
{
}

AppPluginObject_plane::~AppPluginObject_plane()
{
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
	return APP_CLASS_ID_OBJECT_PLANE;
}

AppSceneObject* AppPluginObject_plane::CreateObject()
{
	AppSceneObject_plane* o = new AppSceneObject_plane(this);
	o->SetName(U"Plane");
	o->SetEdgeColor(AppColor(0xFFFF00FF));

	{
		auto aabb = o->GetAABB();

		AppMesh mesh;
		mesh.Allocate(1, AppMeshVertexType::Triangle);
		AppMeshVertexTriangle* triangle = (AppMeshVertexTriangle*)mesh.m_vertices;
		triangle[0].Position.Set(0.f, 3.f, 0.f);
		triangle[0].Normal.Set(0.f, 1.f, 0.f);
		aabb->Add(triangle[0].Position);
		
		triangle[1].Position.Set(1.f, 0.f, 0.f);
		triangle[1].Normal.Set(0.f, 1.f, 0.f);
		aabb->Add(triangle[1].Position);
		
		triangle[2].Position.Set(1.f, 0.f, 1.f);
		triangle[2].Normal.Set(0.f, 1.f, 0.f);
		aabb->Add(triangle[2].Position);
		uint16_t* ind = (uint16_t*)mesh.m_indices;
		ind[0] = 0;
		ind[1] = 1;
		ind[2] = 2;
	//	mesh.m_stride = sizeof(AppMeshVertexTriangle);
	//	mesh.m_vertices = malloc();

		
		o->m_testGO_triangle = m_plugin->GetPluginInterface()->CreateGraphicsObject(&mesh);
	}

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
	auto pi = GetPluginObject()->GetPlugin()->GetPluginInterface();
	if(m_testGO_triangle)
		pi->Destroy(m_testGO_triangle);
	if (m_testGO_line)
		pi->Destroy(m_testGO_line);
	if (m_testGO_point)
		pi->Destroy(m_testGO_point);
}

void AppSceneObject_plane::Draw(AppViewportData* viewportData, AppPluginInterface* )
{
	if (m_testGO_triangle)
		m_testGO_triangle->Draw(viewportData, this);
	if (m_testGO_line)
		m_testGO_line->Draw(viewportData, this);
	if (m_testGO_point)
		m_testGO_point->Draw(viewportData, this);

	/*if (dm == AppViewportDrawMode::Material
		|| dm == AppViewportDrawMode::MaterialWireframe)
	{
		if (m_visualObject_polygon) m_visualObject_polygon->Draw(false);
	}

	if ((dm == AppViewportDrawMode::Wireframe || dm == AppViewportDrawMode::MaterialWireframe)
		|| (m_isSelected && em == AppEditMode::Edge)
		|| (m_isSelected && em == AppEditMode::Polygon)
		)
	{
		if (m_visualObject_edge) m_visualObject_edge->Draw(false);
	}*/
}
