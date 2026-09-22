#pragma once
#ifndef _PLUGINMATH_H_
#define _PLUGINMATH_H_

#define AppInfinity std::numeric_limits<float32_t>::infinity()
#define AppEpsilon std::numeric_limits<float32_t>::epsilon()

#ifndef AL_MATH
/*
		   0 : 0
		22.5 : 0.3926991
		  45 : 0.7853982
		67.5 : 1.1780972
		  90 : 1.5707963
	   112.5 : 1.9634954
		 135 : 2.3561945
	   157.5 : 2.7488936
		 180 : 3.1415927
	   202.5 : 3.5342917
		 225 : 3.9269908
	   247.5 : 4.3196899
		 270 : 4.7123890
	   292.5 : 5.1050881
		 315 : 5.4977871
	   337.5 : 5.8904862
		 360 : 0
	*/
constexpr float32_t PIf = static_cast<float32_t>(3.14159265358979323846);
constexpr float32_t PIfHalf = static_cast<float32_t>(3.14159265358979323846 * 0.5);
constexpr float32_t PIfPlusHalf = static_cast<float32_t>(3.14159265358979323846 + PIfHalf);
constexpr float32_t PIPIf = 6.2831853f;
constexpr float64_t PI = 3.14159265358979323846;
constexpr float64_t PIHalf = 3.14159265358979323846 * 0.5;
constexpr float64_t PIPlusHalf = 3.14159265358979323846 + PIHalf;
constexpr float64_t PIPI = PI + PI;
#endif

#include "AppVec.h"
#include "AppQuaternion.h"
#include "AppMatrix.h"
#include "AppTriangle.h"

class AppRay;

class EDITORLIB_API AppMath
{
public:
	static float32_t Distance2d(float32_t p1x, float32_t p1y, float32_t p2x, float32_t p2y);
	static float32_t Distance2d(const AppVec2f p1, const AppVec2f p2);
	static bool PointInRect(int32_t x, int32_t y, const AppVec4i& rect);
	static bool PointInRect(int32_t x, int32_t y, const AppVec4f& rect);
	static bool PointInRect(float32_t x, float32_t y, const AppVec4f& rect);
	static float32_t DegToRad(float32_t degrees);
	static float32_t RadToDeg(float32_t radians);
	static float32_t Clamp(float32_t x, float32_t min, float32_t max);
	static float32_t Lerp1(float32_t a, float32_t b, float32_t t);
	static float64_t Lerp1(float64_t a, float64_t b, float64_t t);
	static void Lerp1(const AppColor& c1, const AppColor& c2, float32_t t, AppColor& out);
	static float32_t Lerp2(float32_t a, float32_t b, float32_t t);
	static float64_t Lerp2(float64_t a, float64_t b, float64_t t);
	static AppQuaternion Slerp(const AppQuaternion& pStart, const AppQuaternion& pEnd, const float32_t pFactor);
	static float32_t Normalize(float32_t max, float32_t val);
	static AppVec2i WorldToScreen(const AppMat4& VP, const AppVec3& point3D, const AppVec2f& viewportSize, const AppVec2f& offset);
	static AppVec2i ScreenToClient(const AppVec2i& screen_coord, const AppVec4i& client_rect);
	//	static void PerspectiveLHMatrix(alMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far);
	static void PerspectiveRHMatrix(AppMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far);
	static void OrthoRHMatrix(AppMat4& out, float32_t width, float32_t height, float32_t Near, float32_t Far);
	static void OrthographicOffCenterRHMatrix(AppMat4& out, float32_t ViewLeft, float32_t ViewRight, float32_t ViewBottom, float32_t ViewTop, float32_t NearZ, float32_t FarZ);
	static void LookAtRHMatrix(AppMat4& out, const AppVec3& eye, const AppVec3& center, const AppVec3& up = AppVec3(0.f, 1.f, 0.f));
	static void TranslationMatrix(AppMat4& out, const AppVec3& position);
	static void ScaleMatrix(AppMat4& out, const AppVec3& scale);
	static AppVec4 Mul(const AppVec4& vec, const AppMat4& mat);
	static AppVec3 Mul(const AppVec3& vec, const AppMat4& mat);
	static AppVec3f Mul(const AppVec3f& vec, const AppMat4& mat);
	static AppVec3f Mul(const AppVec3f& vec, const AppMat3& mat);
	static AppVec3f MulBasis(const AppVec3f& vec, const AppMat4& mat);
	static AppQuaternion MatToQuat(const AppMat4& mat);
	static bool RayTriangleMT(AppTriangle&, AppRay&, bool withBackFace, float64_t& T, float64_t& U, float64_t& V, float64_t& W);
	static bool RayTriangleMTf(AppTriangle&, AppRay&, bool withBackFace, float32_t& T, float32_t& U, float32_t& V, float32_t& W);
	static bool RayTriangleWT(AppTriangle&, AppRay&, bool withBackFace, float64_t& T, float64_t& U, float64_t& V, float64_t& W);
};

#endif
