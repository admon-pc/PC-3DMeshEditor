#include "AppPluginInterface.h"
#include "std.object.plane.h"
#pragma comment(lib, "libeditor.lib")

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
		
	//	o->m_testGO_triangle = m_plugin->GetPluginInterface()->CreateGraphicsObject(&mesh);
	}

	{
		auto aabb = o->GetAABB();

		AppMesh mesh;
		mesh.Allocate(4, AppMeshVertexType::Line);
		AppMeshVertexLine* line = (AppMeshVertexLine*)mesh.m_vertices;
		line[0].Position.Set(-1.f, 0.f, -1.f);
		line[0].Color.Set(1.f, 0.f, -0.f, 1.f);
		aabb->Add(line[0].Position);

		line[1].Position.Set(2.f, 0.f, 1.f);
		line[1].Color.Set(1.f, 1.f, -0.f, 1.f);
		aabb->Add(line[1].Position);

		line[2].Position.Set(2.f, -1.f, 2.f);
		line[2].Color.Set(1.f, 0.f, 1.f, 1.f);
		aabb->Add(line[2].Position);

		line[3] = line[0];

		uint16_t* ind = (uint16_t*)mesh.m_indices;
		ind[0] = 0;
		ind[1] = 1;
		ind[2] = 1;
		ind[3] = 2;
		ind[4] = 2;
		ind[5] = 0;

	//	o->m_testGO_line = m_plugin->GetPluginInterface()->CreateGraphicsObject(&mesh);
	}

	{
		auto aabb = o->GetAABB();

		AppMesh mesh;
		mesh.Allocate(3, AppMeshVertexType::Point);
		AppMeshVertexPoint* point = (AppMeshVertexPoint*)mesh.m_vertices;
		point[0].Position.Set(-1.f, 2.f, -1.f);
		point[0].Color.Set(1.f, 1.f, 1.f, 1.f);
		aabb->Add(point[0].Position);

		point[1].Position.Set(2.f, 3.f, 1.f);
		point[1].Color.Set(1.f, 1.f, 1.f, 1.f);
		aabb->Add(point[1].Position);

		point[2].Position.Set(2.f, 1.f, 2.f);
		point[2].Color.Set(1.f, 1.f, 1.f, 1.f);
		aabb->Add(point[2].Position);

		uint16_t* ind = (uint16_t*)mesh.m_indices;
		ind[0] = 0;
		ind[1] = 1;
		ind[2] = 2;

	//	o->m_testGO_point = m_plugin->GetPluginInterface()->CreateGraphicsObject(&mesh);
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
	/*if(m_testGO_triangle)
		pi->Destroy(m_testGO_triangle);
	if (m_testGO_line)
		pi->Destroy(m_testGO_line);
	if (m_testGO_point)
		pi->Destroy(m_testGO_point);*/
}

void AppSceneObject_plane::Draw(AppViewportData* viewportData, AppPluginInterface* )
{
	/*if (m_testGO_triangle)
		m_testGO_triangle->Draw(viewportData, this);
	if (m_testGO_line)
		m_testGO_line->Draw(viewportData, this);
	if (m_testGO_point)
		m_testGO_point->Draw(viewportData, this);*/

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

bool AppSceneObject_plane::OnSelect(AppSelectionFrust* f, AppRay* r , bool selectByRectangle, AppEditMode em)
{
	if (selectByRectangle)
	{
		if (em == AppEditMode::Object)
		{
		}
	}
	else
	{
		if (em == AppEditMode::Object)
		{
		}
	}
	return false;
}

bool AppSceneObject_plane::IsCanSelect(AppSelectionFrust* f, AppRay* r)
{
	// this method currently only will be called when AppEditMode is AppEditMode::Object
	// 
	//if (em == AppEditMode::Object)
	{
		AppTriangle tri(AppVec3f(-1.f, 0.f, -1.f), AppVec3f(-1.f, 0.f, 1.f), AppVec3f(1.f, 0.f, 1.f));
		float64_t T, U, V, W;
		T = U = V = W = 0.f;
		if (AppMath::RayTriangleMT(tri, *r, true, T, U, V, W))
		{

		}
		//r->
	}
	return false;
}


