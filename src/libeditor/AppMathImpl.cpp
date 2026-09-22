#include "editor.lib.h"

float32_t AppMath::Distance2d(float32_t p1x, float32_t p1y, float32_t p2x, float32_t p2y)
{
	p1x -= p2x;
	p1y -= p2y;
	return sqrtf((p1x * p1x) + (p1y * p1y));
}
float32_t AppMath::Distance2d(const AppVec2f p1, const AppVec2f p2)
{
	float32_t px = p1.x - p2.x;
	float32_t py = p1.y - p2.y;
	return sqrtf((px * px) + (py * py));
}

bool AppMath::PointInRect(int32_t x, int32_t y, const AppVec4i& rect)
{
	if (x >= rect.x)
	{
		if (x <= rect.z)
		{
			if (y >= rect.y)
			{
				if (y <= rect.w)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool AppMath::PointInRect(int32_t x, int32_t y, const AppVec4f& rect)
{
	if (x >= rect.x)
	{
		if (x <= rect.z)
		{
			if (y >= rect.y)
			{
				if (y <= rect.w)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool AppMath::PointInRect(float32_t x, float32_t y, const AppVec4f& rect)
{
	if (x >= rect.x)
	{
		if (x <= rect.z)
		{
			if (y >= rect.y)
			{
				if (y <= rect.w)
				{
					return true;
				}
			}
		}
	}
	return false;
}

float32_t AppMath::DegToRad(float32_t degrees)
{
	return degrees * (PIf / 180.f);
}

float32_t AppMath::RadToDeg(float32_t radians)
{
	return radians * (180.f / PIf);
}

float32_t AppMath::Clamp(float32_t x, float32_t min, float32_t max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	return x;
}

float32_t AppMath::Lerp1(float32_t a, float32_t b, float32_t t)
{
	return (a * (1.f - t)) + (b * t);
}

float64_t AppMath::Lerp1(float64_t a, float64_t b, float64_t t)
{
	return (a * (1.f - t)) + (b * t);
}

void AppMath::Lerp1(const AppColor& c1, const AppColor& c2, float32_t t, AppColor& out)
{
	out.m_data[0] = Lerp1(c1.m_data[0], c2.m_data[0], t);
	out.m_data[1] = Lerp1(c1.m_data[1], c2.m_data[1], t);
	out.m_data[2] = Lerp1(c1.m_data[2], c2.m_data[2], t);
	out.m_data[3] = Lerp1(c1.m_data[3], c2.m_data[3], t);
}

float32_t AppMath::Lerp2(float32_t a, float32_t b, float32_t t)
{
	return a + t * (b - a);
}

float64_t AppMath::Lerp2(float64_t a, float64_t b, float64_t t)
{
	return a + t * (b - a);
}

// assimp
AppQuaternion AppMath::Slerp(const AppQuaternion& pStart, const AppQuaternion& pEnd, const float32_t pFactor)
{
	AppQuaternion pOut;
	float32_t cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

	// adjust signs (if necessary)
	AppQuaternion end = pEnd;
	if (cosom < 0.0f)
	{
		cosom = -cosom;
		end.x = -end.x;   // Reverse all signs
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	// Calculate coefficients
	float32_t sclp, sclq;
	if ((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float32_t omega, sinom;
		omega = std::acos(cosom); // extract theta from dot product's cos theta
		sinom = std::sin(omega);
		sclp = std::sin((1.0f - pFactor) * omega) / sinom;
		sclq = std::sin(pFactor * omega) / sinom;
	}
	else
	{
		// Very close, do linear interp (because it's faster)
		sclp = 1.0f - pFactor;
		sclq = pFactor;
	}

	pOut.x = sclp * pStart.x + sclq * end.x;
	pOut.y = sclp * pStart.y + sclq * end.y;
	pOut.z = sclp * pStart.z + sclq * end.z;
	pOut.w = sclp * pStart.w + sclq * end.w;
	return pOut;
}

float32_t AppMath::Normalize(float32_t max, float32_t val)
{
	return val * (1.f / max);
}

AppVec2i AppMath::WorldToScreen(const AppMat4& VP, const AppVec3& point3D, const AppVec2f& viewportSize, const AppVec2f& offset)
{
	AppVec4 point;
	point.x = point3D.x;
	point.y = point3D.y;
	point.z = point3D.z;
	point.w = 1.f;

	point = AppMath::Mul(point, VP);

	return AppVec2i
	(
		int32_t(offset.x + (viewportSize.x * 0.5f + point.x * viewportSize.x * 0.5f / point.w))
		,
		int32_t(offset.y + (viewportSize.y - (viewportSize.y * 0.5f + point.y * viewportSize.y * 0.5f / point.w)))
	);
}

AppVec2i AppMath::ScreenToClient(const AppVec2i& screen_coord, const AppVec4i& client_rect)
{
	return AppVec2i(
		screen_coord.x - client_rect.x,
		screen_coord.y - client_rect.y
	);
}

//void AppMath::PerspectiveLHMatrix(AppMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far)
//{
//	float32_t S = std::sin(0.5f * FOV);
//	float32_t C = std::cos(0.5f * FOV);
//	float32_t H = C / S;
//	float32_t W = H / aspect;
//	out[0] = AppVec4(W, 0.f, 0.f, 0.f);
//	out[1] = AppVec4(0.f, H, 0.f, 0.f);
//	out[2] = AppVec4(0.f, 0.f, Far / (Far - Near), 1.f);
//	out[3] = AppVec4(0.f, 0.f, -out[2].z * Near, 0.f);
//}

void AppMath::PerspectiveRHMatrix(AppMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far)
{
	float32_t S = std::sin(0.5f * FOV);
	float32_t C = std::cos(0.5f * FOV);
	float32_t H = C / S;
	float32_t W = H / aspect;
	out[0] = AppVec4(W, 0.f, 0.f, 0.f);
	out[1] = AppVec4(0.f, H, 0.f, 0.f);
	out[2] = AppVec4(0.f, 0.f, Far / (Near - Far), -1.f);
	out[3] = AppVec4(0.f, 0.f, out[2].z * Near, 0.f);
}

void AppMath::OrthoRHMatrix(AppMat4& out, float32_t width, float32_t height, float32_t Near, float32_t Far)
{
	out[0] = AppVec4(2.f / width, 0.f, 0.f, 0.f);
	out[1] = AppVec4(0.f, 2.f / height, 0.f, 0.f);
	out[2] = AppVec4(0.f, 0.f, 1.f / (Near - Far), 0.f);
	out[3] = AppVec4(0.f, 0.f, out[2].z * Near, 1.f);
}

void AppMath::OrthographicOffCenterRHMatrix(AppMat4& out, float32_t ViewLeft, float32_t ViewRight, float32_t ViewBottom, float32_t ViewTop, float32_t NearZ, float32_t FarZ)
{
	float32_t ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	float32_t ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);

	out.m_data[0].Set(ReciprocalWidth + ReciprocalWidth, 0.0f, 0.0f, 0.0f);
	out.m_data[1].Set(0.0f, ReciprocalHeight + ReciprocalHeight, 0.0f, 0.0f);
	out.m_data[2].Set(0.0f, 0.0f, 1.0f / (NearZ - FarZ), 0.0f);
	out.m_data[3].Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
		-(ViewTop + ViewBottom) * ReciprocalHeight,
		out.m_data[2].z * NearZ,
		1.0f);
}

void AppMath::LookAtRHMatrix(AppMat4& out, const AppVec3& eye, const AppVec3& center, const AppVec3& up)
{
	AppVec3 f(center - eye);
	f.Normalize2();

	AppVec3 s;
	f.Cross2(up, s);
	s.Normalize2();

	AppVec3 u;
	s.Cross2(f, u);

	out.Identity();

	out[0].x = s.x;
	out[1].x = s.y;
	out[2].x = s.z;
	out[0].y = u.x;
	out[1].y = u.y;
	out[2].y = u.z;
	out[0].z = -f.x;
	out[1].z = -f.y;
	out[2].z = -f.z;
	out[3].x = -s.Dot(eye);
	out[3].y = -u.Dot(eye);
	out[3].z = f.Dot(eye);
}

void AppMath::TranslationMatrix(AppMat4& out, const AppVec3& position)
{
	out[3].x = position.x;
	out[3].y = position.y;
	out[3].z = position.z;
}

void AppMath::ScaleMatrix(AppMat4& out, const AppVec3& scale)
{
	out[0].x = scale.x;
	out[1].y = scale.y;
	out[2].z = scale.z;
}

AppVec4 AppMath::Mul(const AppVec4& vec, const AppMat4& mat)
{
	return AppVec4
	(
		mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x * vec.z + mat[3].x * vec.w,
		mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y * vec.z + mat[3].y * vec.w,
		mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z * vec.z + mat[3].z * vec.w,
		mat[0].w * vec.x + mat[1].w * vec.y + mat[2].w * vec.z + mat[3].w * vec.w
	);
}

AppVec3 AppMath::Mul(const AppVec3& vec, const AppMat4& mat)
{
	return AppVec3
	(
		mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x * vec.z,
		mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y * vec.z,
		mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z * vec.z
	);
}

AppVec3f AppMath::Mul(const AppVec3f& vec, const AppMat4& mat)
{
	return AppVec3f
	(
		(float32_t)mat[0].x * vec.x + (float32_t)mat[1].x * vec.y + (float32_t)mat[2].x * vec.z + (float32_t)mat[3].x,
		(float32_t)mat[0].y * vec.x + (float32_t)mat[1].y * vec.y + (float32_t)mat[2].y * vec.z + (float32_t)mat[3].y,
		(float32_t)mat[0].z * vec.x + (float32_t)mat[1].z * vec.y + (float32_t)mat[2].z * vec.z + (float32_t)mat[3].z
	);
}

AppVec3f AppMath::Mul(const AppVec3f& vec, const AppMat3& mat)
{
	return AppVec3f
	(
		(float32_t)mat[0].x * vec.x + (float32_t)mat[1].x * vec.y + (float32_t)mat[2].x * vec.z + (float32_t)mat[3].x,
		(float32_t)mat[0].y * vec.x + (float32_t)mat[1].y * vec.y + (float32_t)mat[2].y * vec.z + (float32_t)mat[3].y,
		(float32_t)mat[0].z * vec.x + (float32_t)mat[1].z * vec.y + (float32_t)mat[2].z * vec.z + (float32_t)mat[3].z
	);
}

AppVec3f AppMath::MulBasis(const AppVec3f& vec, const AppMat4& mat)
{
	return AppVec3f
	(
		(float32_t)mat[0].x * vec.x + (float32_t)mat[1].x * vec.y + (float32_t)mat[2].x * vec.z,
		(float32_t)mat[0].y * vec.x + (float32_t)mat[1].y * vec.y + (float32_t)mat[2].y * vec.z,
		(float32_t)mat[0].z * vec.x + (float32_t)mat[1].z * vec.y + (float32_t)mat[2].z * vec.z
	);
}

AppQuaternion AppMath::MatToQuat(const AppMat4& mat)
{
	AppQuaternion q;
	float32_t trace = float32_t(mat[0].x + mat[1].y + mat[2].z);
	if (trace > 0.0f)
	{

		float32_t t = trace + 1.0f;
		float32_t s = std::sqrt(t) * 0.5f;

		q.w = s * t;
		q.x = float32_t(mat[2].y - mat[1].z) * s;
		q.y = float32_t(mat[0].z - mat[2].x) * s;
		q.z = float32_t(mat[1].x - mat[0].y) * s;

	}
	else
	{
		static int32_t	next[3] = { 1, 2, 0 };
		int32_t i = 0;
		if (mat[1][1] > mat[0][0])
			i = 1;
		if (mat[2][2] > mat[i][i])
			i = 2;
		int32_t j = next[i];
		int32_t k = next[j];

		float32_t t = float32_t(mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f;
		float32_t s = std::sqrt(t) * 0.5f;

		q[i] = s * t;
		q[3] = float32_t(mat[k][j] - mat[j][k]) * s;
		q[j] = float32_t(mat[j][i] + mat[i][j]) * s;
		q[k] = float32_t(mat[k][i] + mat[i][k]) * s;
	}
	return q;
}

bool AppMath::RayTriangleMT(AppTriangle& tri, AppRay& ray, bool withBackFace, float64_t& T, float64_t& U, float64_t& V, float64_t& W)
{
	AppVec4  pvec;
	ray.m_direction.Cross2(AppVec4(tri.e2.x, tri.e2.y, tri.e2.z, tri.e2.w), pvec);
	float64_t det = tri.e1.Dot(AppVec4(pvec.x, pvec.y, pvec.z, pvec.w));

	if (withBackFace)
	{
		if (std::fabs(det) < AppEpsilon)
			return false;
	}
	else
	{
		if (det < AppEpsilon && det > -AppEpsilon)
			return false;
	}

	AppVec4 tvec(
		ray.m_origin.x - tri.v1.x,
		ray.m_origin.y - tri.v1.y,
		ray.m_origin.z - tri.v1.z,
		0.f);

	float64_t inv_det = 1.f / det;
	U = (float32_t)tvec.Dot(pvec) * inv_det;

	if (U < 0.f || U > 1.f)
		return false;

	AppVec4  qvec;
	tvec.Cross2(AppVec4(tri.e1.x, tri.e1.y, tri.e1.z, tri.e1.w), qvec);
	V = ray.m_direction.Dot(qvec) * inv_det;

	if (V < 0.f || U + V > 1.f)
		return false;

	T = tri.e2.Dot(AppVec4(qvec.x, qvec.y, qvec.z, qvec.w)) * inv_det;

	if (T < AppEpsilon) return false;

	W = 1.f - U - V;
	return true;
}

bool AppMath::RayTriangleMTf(AppTriangle& tri, AppRay& ray, bool withBackFace, float32_t& T, float32_t& U, float32_t& V, float32_t& W)
{
	AppVec4f  pvec;
	AppVec4f rayDir(ray.m_direction.x, ray.m_direction.y, ray.m_direction.z, ray.m_direction.w);
	rayDir.Cross2(AppVec4f(tri.e2.x, tri.e2.y, tri.e2.z, tri.e2.w), pvec);
	
	AppVec4f triE1(tri.e1.x, tri.e1.y, tri.e1.z, tri.e1.w);
	AppVec4f triE2(tri.e2.x, tri.e2.y, tri.e2.z, tri.e2.w);

	float32_t det = triE1.Dot(pvec);

	if (withBackFace)
	{
		if (std::fabs(det) < AppEpsilon)
			return false;
	}
	else
	{
		if (det < AppEpsilon && det > -AppEpsilon)
			return false;
	}

	AppVec4f tvec(
		ray.m_origin.x - tri.v1.x,
		ray.m_origin.y - tri.v1.y,
		ray.m_origin.z - tri.v1.z,
		0.f);

	float32_t inv_det = 1.f / det;
	U = tvec.Dot(pvec) * inv_det;

	if (U < 0.f || U > 1.f)
		return false;

	AppVec4f  qvec;
	tvec.Cross2(triE1, qvec);
	V = rayDir.Dot(qvec) * inv_det;

	if (V < 0.f || U + V > 1.f)
		return false;

	T = triE2.Dot(qvec) * inv_det;

	if (T < AppEpsilon) return false;

	W = 1.f - U - V;
	return true;
}

bool AppMath::RayTriangleWT(AppTriangle& tri, AppRay& ray, bool withBackFace, float64_t& T, float64_t& U, float64_t& V, float64_t& W)
{
	tri.v1.w = 1.f;
	tri.v2.w = 1.f;
	tri.v3.w = 1.f;
	const auto A = tri.v2 - ray.m_origin;
	const auto B = tri.v3 - ray.m_origin;
	const auto C = tri.v1 - ray.m_origin;

	const float64_t Ax = A[ray.m_kx] - (ray.m_Sx * A[ray.m_kz]);
	const float64_t Ay = A[ray.m_ky] - (ray.m_Sy * A[ray.m_kz]);
	const float64_t Bx = B[ray.m_kx] - (ray.m_Sx * B[ray.m_kz]);
	const float64_t By = B[ray.m_ky] - (ray.m_Sy * B[ray.m_kz]);
	const float64_t Cx = C[ray.m_kx] - (ray.m_Sx * C[ray.m_kz]);
	const float64_t Cy = C[ray.m_ky] - (ray.m_Sy * C[ray.m_kz]);

	U = (Cx * By) - (Cy * Bx);
	V = (Ax * Cy) - (Ay * Cx);
	W = (Bx * Ay) - (By * Ax);

	if (U == 0.f || V == 0.f || W == 0.f)
	{
		float64_t CxBy = (float64_t)Cx * (float64_t)By;
		float64_t CyBx = (float64_t)Cy * (float64_t)Bx;
		U = (float64_t)(CxBy - CyBx);

		float64_t AxCy = (float64_t)Ax * (float64_t)Cy;
		float64_t AyCx = (float64_t)Ay * (float64_t)Cx;
		V = (float64_t)(AxCy - AyCx);

		float64_t BxAy = (float64_t)Bx * (float64_t)Ay;
		float64_t ByAx = (float64_t)By * (float64_t)Ax;
		W = (float64_t)(BxAy - ByAx);
	}

	if (withBackFace)
	{
		if ((U < 0.f || V < 0.f || W < 0.f) &&
			(U > 0.f || V > 0.f || W > 0.f))
			return false;
	}
	else
	{
		if (U < 0.f || V < 0.f || W < 0.f)
			return false;
	}

	float64_t det = U + V + W;

	if (det == 0.f)
		return false;

	const float64_t Az = ray.m_Sz * A[ray.m_kz];
	const float64_t Bz = ray.m_Sz * B[ray.m_kz];
	const float64_t Cz = ray.m_Sz * C[ray.m_kz];
	const float64_t Ts = (U * Az) + (V * Bz) + (W * Cz);

	if (!withBackFace) // CULL
	{
		if (Ts < 0.f || Ts > AppInfinity * det)
			return false;
	}
	else
	{
		if (det < 0.f && (Ts >= 0.f || Ts < AppInfinity * det))
			return false;
		else if (det > 0.f && (Ts <= 0.f || Ts > AppInfinity * det))
			return false;
	}

	const float64_t invDet = 1.f / det;
	U = U * invDet;
	V = V * invDet;
	W = W * invDet;
	T = Ts * invDet;
	if (T < AppEpsilon)
		return false;
	return true;
}



