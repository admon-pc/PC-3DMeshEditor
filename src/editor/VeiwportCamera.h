#pragma once
#ifndef _PCViewportCam_H_

// Each viewport have something like camera
enum class AppViewportCameraType : uint32_t
{
	Perspective,
	Left,
	Right,
	Top,
	Bottom,
	Front,
	Back
};

class AppViewport;
class AppViewportCamera
{
public:
	AppViewportCamera(AppViewport*, AppViewportCameraType ct);
	~AppViewportCamera();

	void Copy(AppViewportCamera*);

	bool m_forceOrtho = false;

	AppViewport* m_viewport = 0;
	AppViewportCameraType m_type = AppViewportCameraType::Perspective;
	alDirection m_direction;

	// calculate matrices
	void Update();
	alMat4 m_viewMatrix;
	alMat4 m_viewMatrixInvert;

	alMat4 m_projectionMatrixOrtho;
	alMat4 m_projectionMatrixPersp;
	alMat4 m_projectionMatrix;

	alMat4 m_viewProjectionMatrix;
	alMat4 m_viewProjectionInvertMatrix;


	float32_t m_near;
	float32_t m_far;
	float32_t m_fov;
	float32_t m_aspect;

	alVec3 m_rotationPlatform;
	alVec4 m_positionPlatform; // w = height, zoom, mouse wheel value
	alVec3 m_positionCamera; // in world

	void Reset();
	void MoveToSelection();

	alVec4 m_creationRect;

	void PanMove();
	void Rotate(float32_t x, float32_t y);
	void RotateZ();
	void Zoom();
	void ChangeFOV();

	struct Frustum
	{
		alVec4 m_planes[6u];

		bool PointInFrustum(const alVec4& point)
		{

			if ((
				m_planes[0].x * point.x +
				m_planes[0].y * point.y +
				m_planes[0].z * point.z +
				m_planes[0].w)
				< 0.f)
				return false;
			if ((
				m_planes[1].x * point.x +
				m_planes[1].y * point.y +
				m_planes[1].z * point.z +
				m_planes[1].w)
				< 0.f)
				return false;
			if ((
				m_planes[2].x * point.x +
				m_planes[2].y * point.y +
				m_planes[2].z * point.z +
				m_planes[2].w)
				< 0.f)
				return false;
			if ((
				m_planes[3].x * point.x +
				m_planes[3].y * point.y +
				m_planes[3].z * point.z +
				m_planes[3].w)
				< 0.f)
				return false;
			if ((
				m_planes[4].x * point.x +
				m_planes[4].y * point.y +
				m_planes[4].z * point.z +
				m_planes[4].w)
				< 0.f)
				return false;
			if ((
				m_planes[5].x * point.x +
				m_planes[5].y * point.y +
				m_planes[5].z * point.z +
				m_planes[5].w)
				< 0.f)
				return false;
			return true;
		}

		bool SphereInFrustum(float32_t radius, const alVec4& position)
		{
			for (uint32_t i = 0u; i < 6u; ++i)
			{
				if (m_planes[i].x * position.x +
					m_planes[i].y * position.y +
					m_planes[i].z * position.z +
					m_planes[i].w <= -radius)
					return false;
			}
			return true;
			/*float dist01 = std::min(DistanceToPlane(m_planes[0], position), DistanceToPlane(m_planes[1], position));
			float dist23 = std::min(DistanceToPlane(m_planes[2], position), DistanceToPlane(m_planes[3], position));
			float dist45 = std::min(DistanceToPlane(m_planes[4], position), DistanceToPlane(m_planes[5], position));
			return std::min(std::min(dist01, dist23), dist45) + radius;*/
		}

		enum FrustumSide
		{
			RIGHT = 0,		// The RIGHT side of the frustum
			LEFT = 1,		// The LEFT	 side of the frustum
			BOTTOM = 2,		// The BOTTOM side of the frustum
			TOP = 3,		// The TOP side of the frustum
			BACK = 4,		// The BACK	side of the frustum
			FRONT = 5			// The FRONT side of the frustum
		};

		void NormalizePlane(alVec4& plane)
		{
			// Here we calculate the magnitude of the normal to the plane (point A B C)
			// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
			// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
			float magnitude = (float)sqrt(plane.x * plane.x +
				plane.y * plane.y +
				plane.z * plane.z);

			// Then we divide the plane's values by it's magnitude.
			// This makes it easier to work with.
			plane.x /= magnitude;
			plane.y /= magnitude;
			plane.z /= magnitude;
			plane.w /= magnitude;
		}

		void CalculateFrustum(alMat4& P, alMat4& V)
		{
			float64_t* proj = P.GetPtr();
			float64_t* modl = V.GetPtr();
			float64_t   clip[16]; //clipping planes

			clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
			clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
			clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
			clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

			clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
			clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
			clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
			clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

			clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
			clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
			clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
			clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

			clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
			clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
			clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
			clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];


			//RIGHT 
			m_planes[RIGHT].x = (clip[3] - clip[0]);
			m_planes[RIGHT].y = (clip[7] - clip[4]);
			m_planes[RIGHT].z = (clip[11u] - clip[8]);
			m_planes[RIGHT].w = (clip[15u] - clip[12u]);
			NormalizePlane(m_planes[RIGHT]);

			//LEFT  
			m_planes[LEFT].x = (clip[3] + clip[0]);
			m_planes[LEFT].y = (clip[7] + clip[4]);
			m_planes[LEFT].z = (clip[11u] + clip[8]);
			m_planes[LEFT].w = (clip[15u] + clip[12u]);
			NormalizePlane(m_planes[LEFT]);

			//BOTTOM  
			m_planes[BOTTOM].x = (clip[3] + clip[1]);
			m_planes[BOTTOM].y = (clip[7] + clip[5]);
			m_planes[BOTTOM].z = (clip[11u] + clip[9]);
			m_planes[BOTTOM].w = (clip[15u] + clip[13u]);
			NormalizePlane(m_planes[BOTTOM]);

			//TOP  
			m_planes[TOP].x = (clip[3] - clip[1]);
			m_planes[TOP].y = (clip[7] - clip[5]);
			m_planes[TOP].z = (clip[11u] - clip[9]);
			m_planes[TOP].w = (clip[15u] - clip[13u]);
			NormalizePlane(m_planes[TOP]);

			//FAR  
			m_planes[BACK].x = (clip[3] - clip[2]);
			m_planes[BACK].y = (clip[7] - clip[6]);
			m_planes[BACK].z = (clip[11u] - clip[10u]);
			m_planes[BACK].w = (clip[15u] - clip[14u]);
			NormalizePlane(m_planes[BACK]);

			//NEAR  
			m_planes[FRONT].x = (clip[3] + clip[2]);
			m_planes[FRONT].y = (clip[7] + clip[6]);
			m_planes[FRONT].z = (clip[11u] + clip[10u]);
			m_planes[FRONT].w = (clip[15u] + clip[14u]);
			NormalizePlane(m_planes[FRONT]);
		}
	};
	Frustum m_frust;

	void(*m_onMoveToSelection)(AppViewportCamera*);
};

#endif

