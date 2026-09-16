#include "editor.h"
extern Application* g_app;

AppViewportCamera::AppViewportCamera(
	AppViewport* vp, 
	AppViewportCameraType ct) 
{
	m_viewport = vp;
	m_type = ct;
	m_forceOrtho = false;
	m_onMoveToSelection = 0;
}


AppViewportCamera::~AppViewportCamera() {}

void AppViewportCamera::Copy(AppViewportCamera* other)
{
	m_type = other->m_type;
	m_aspect = other->m_aspect;
	m_far = other->m_far;
	m_fov = other->m_fov;
	m_near = other->m_near;
	m_positionCamera = other->m_positionCamera;
	m_rotationPlatform = other->m_rotationPlatform;
	m_positionPlatform = other->m_positionPlatform;
}

void AppViewportCamera::Update()
{
	{
		float32_t zoom = (float32_t)m_positionPlatform.w;
		alMath::OrthoRHMatrix(m_projectionMatrixOrtho,
			zoom * m_aspect,
			zoom,
			-m_far,
			m_far);
		alMath::PerspectiveRHMatrix(m_projectionMatrixPersp, m_fov, m_aspect, m_near, m_far);
	}

	if (m_type != AppViewportCameraType::Perspective || m_forceOrtho)
	{
		m_projectionMatrix = m_projectionMatrixOrtho;
	}
	else
	{
		m_projectionMatrix = m_projectionMatrixPersp;
	}

	alMat4 MX(alQuaternion((float32_t)m_rotationPlatform.x, 0.f, 0.f));
	alMat4 MY(alQuaternion(0.f, (float32_t)m_rotationPlatform.y, 0.f));
	//alMat4 MZ(alQuaternion(0.f, 0.f, (float32_t)m_rotationPlatform.z));

	m_positionCamera = alVec3(0.f, m_positionPlatform.w, 0.f);
	m_positionCamera = alMath::Mul(m_positionCamera, (MY * MX));
	m_positionCamera += alVec3(m_positionPlatform.x, m_positionPlatform.y, m_positionPlatform.z);

	alMat4 T;
	T.m_data[3].x = -m_positionCamera.x;
	T.m_data[3].y = -m_positionCamera.y;
	T.m_data[3].z = -m_positionCamera.z;
	T.m_data[3].w = 1.f;

	alMat4 P(alQuaternion(alVec4f(float32_t(-m_rotationPlatform.x) + alMath::DegToRad(-90.f), 0.f, 0.f, 1.f)));
	alMat4 Y(alQuaternion(alVec4f(0.f, float32_t(-m_rotationPlatform.y) + alMath::DegToRad(0.f), 0.f, 1.f)));
	alMat4 R(alQuaternion(alVec4f(0.f, 0.f, float32_t(m_rotationPlatform.z), 1.f)));

	m_viewMatrix = (R * (P * Y)) * T;
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;


	m_viewMatrixInvert = m_viewMatrix; m_viewMatrixInvert.Invert();
	auto pi = m_projectionMatrix; pi.Invert();
	m_viewProjectionInvertMatrix = m_viewMatrixInvert * pi;
	m_frust.CalculateFrustum(m_projectionMatrix, m_viewMatrix);

	m_direction = alDirection::NorthEast;

	if (m_rotationPlatform.y >= 2.7488936 && m_rotationPlatform.y <= 3.5342917)
	{
		m_direction = alDirection::North;
	}
	else if (m_rotationPlatform.y >= 5.8904862 || m_rotationPlatform.y <= 0.3926991)
	{
		m_direction = alDirection::South;
	}
	else if (m_rotationPlatform.y >= 1.1780972 && m_rotationPlatform.y <= 1.9634954)
	{
		m_direction = alDirection::West;
	}
	else if (m_rotationPlatform.y >= 4.3196899 && m_rotationPlatform.y <= 5.1050881)
	{
		m_direction = alDirection::East;
	}
	else if (m_rotationPlatform.y >= 1.9634954 && m_rotationPlatform.y <= 2.7488936)
	{
		m_direction = alDirection::NorthWest;
	}
	else if (m_rotationPlatform.y >= 0.3926991 && m_rotationPlatform.y <= 1.1780972)
	{
		m_direction = alDirection::SouthWest;
	}
	else if (m_rotationPlatform.y >= 5.1050881 && m_rotationPlatform.y <= 5.8904862)
	{
		m_direction = alDirection::SouthEast;
	}
	//printf("%f %s\n", m_rotationPlatform.y, miGetDirectionName(m_direction));
}

void AppViewportCamera::MoveToSelection()
{
	if (m_onMoveToSelection)
	{
		m_onMoveToSelection(this);
		return;
	}

	if (!g_app->m_selectionAabb.IsEmpty())
	{
		m_positionPlatform = g_app->m_selectionAabb_center;
		m_positionPlatform.w = g_app->m_selectionAabb_extent.Length();
	}
	else if (!g_app->m_sceneAabb.IsEmpty())
	{
		alVec4 e;
		g_app->m_sceneAabb.Extent(e);

		alVec4 c;
		g_app->m_sceneAabb.Center(c);
		m_positionPlatform = c;
		m_positionPlatform.w = e.Length();
	}
	else
	{
		Reset();
	}

	this->Update();
}

void AppViewportCamera::UpdateAspect(AppViewport* v)
{
	m_aspect = v->m_rectSz.x / v->m_rectSz.y;
}

void AppViewportCamera::Reset()
{
	m_near = 0.01f;
	m_far = 2500.f;
	m_fov = 0.683264f;
	m_aspect = 800.f / 600.f;
	m_positionPlatform = alVec4(0.f, 0.f, 0.f, 15.f);
	switch (m_type)
	{
	case AppViewportCameraType::Perspective:
		m_rotationPlatform = alVec3(alMath::DegToRad(-45.f), 0.f, 0.f);
		break;
	case AppViewportCameraType::Bottom:
		m_rotationPlatform = alVec3(alMath::DegToRad(-180.f), 0.f, 0.f);
		break;
	case AppViewportCameraType::Left:
		m_rotationPlatform = alVec3(alMath::DegToRad(-90.f), alMath::DegToRad(-90.f), 0.f);
		break;
	case AppViewportCameraType::Right:
		m_rotationPlatform = alVec3(alMath::DegToRad(-90.f), alMath::DegToRad(90.f), 0.f);
		break;
	case AppViewportCameraType::Back:
		m_rotationPlatform = alVec3(alMath::DegToRad(-90.f), alMath::DegToRad(180.f), 0.f);
		break;
	case AppViewportCameraType::Front:
		m_rotationPlatform = alVec3(alMath::DegToRad(-90.f), alMath::DegToRad(0.f), 0.f);
		break;
	case AppViewportCameraType::Top:
		m_rotationPlatform = alVec3();
		break;
	}

	Rotate(0, 0);

	m_viewport->SetCameraType(m_viewport->m_cameraType);
	m_viewport->UpdateAspect();
	Update();
}

void AppViewportCamera::PanMove()
{
	float32_t speed = 10.f * ((float32_t)m_positionPlatform.w * 0.01f);

	auto input = alLib::GetInput();

	alVec4 vec(
		speed * -input->m_mouseDelta.x * g_app->m_dt,
		0.f,
		speed * -input->m_mouseDelta.y * g_app->m_dt,
		0.f);
	alMat4 MX(alQuaternion((float32_t)m_rotationPlatform.x, 0.f, 0.f));
	alMat4 MY(alQuaternion(0.f, (float32_t)m_rotationPlatform.y, 0.f));
	//alMat4 MZ(alQuaternion(0.f, 0.f, m_rotationPlatform.z));
	vec = alMath::Mul(vec, MY * MX);
	m_positionPlatform += vec;
	Update();
}

void AppViewportCamera::Rotate(float32_t x, float32_t y)
{
	const float32_t speed = 0.69f * g_app->m_dt;
	m_rotationPlatform.x += y * speed;
	m_rotationPlatform.y += x * speed;

	if (m_rotationPlatform.y < 0.f) m_rotationPlatform.y = m_rotationPlatform.y + PIPI;

	if (m_rotationPlatform.x > PIPI) m_rotationPlatform.x = 0.f;
	if (m_rotationPlatform.y > PIPI) m_rotationPlatform.y = 0.f;
	//if (m_rotationPlatform.x < -alMath::PIPI) m_rotationPlatform.x = 0.f;
	//if (m_rotationPlatform.y < -alMath::PIPI) m_rotationPlatform.y = 0.f;


	if (m_type != AppViewportCameraType::Perspective)
		m_viewport->SetViewportName(U"Orthogonal");

	Update();
}

void AppViewportCamera::Zoom() 
{
	auto input = alLib::GetInput();
	float32_t mult = 1.f;
	if (input->m_kbm == alKeyboardModifier::Shift)
		mult = 3.f;

	if (input->m_wheelDelta > 0)
		m_positionPlatform.w *= 0.9f * (1.f / mult);
	else
		m_positionPlatform.w *= 1.1f * mult;

	if (m_positionPlatform.w < 0.01f)
		m_positionPlatform.w = 0.01f;

	Update();
}

void AppViewportCamera::ChangeFOV()
{
	auto input = alLib::GetInput();
	m_fov += input->m_mouseDelta.x * g_app->m_dt;
	if (m_fov < 0.01f)
		m_fov = 0.01f;
	if (m_fov > PIf)
		m_fov = PIf;
	//printf("m_fov %f\n", m_fov);
	Update();
}

void AppViewportCamera::RotateZ()
{
	auto input = alLib::GetInput();
	m_rotationPlatform.z += input->m_mouseDelta.x * g_app->m_dt;
	Update();
}
