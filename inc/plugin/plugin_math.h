#pragma once
#ifndef _PLUGINMATH_H_
#define _PLUGINMATH_H_

#define plInfinity std::numeric_limits<float32_t>::infinity()
#define plEpsilon std::numeric_limits<float32_t>::epsilon()

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

#include "plugin/plugin_vec.h"
#include "plugin/plugin_quaternion.h"
#include "plugin/plugin_matrix.h"

class PLUGIN_API PluginMath
{
public:
	static float32_t Distance2d(float32_t p1x, float32_t p1y, float32_t p2x, float32_t p2y);
	static float32_t Distance2d(const plVec2f p1, const plVec2f p2);
	static bool PointInRect(int32_t x, int32_t y, const plVec4i& rect);
	static bool PointInRect(int32_t x, int32_t y, const plVec4f& rect);
	static bool PointInRect(float32_t x, float32_t y, const plVec4f& rect);
	static float32_t DegToRad(float32_t degrees);
	static float32_t RadToDeg(float32_t radians);
	static float32_t Clamp(float32_t x, float32_t min, float32_t max);
	static float32_t Lerp1(float32_t a, float32_t b, float32_t t);
	static float64_t Lerp1(float64_t a, float64_t b, float64_t t);
	static void Lerp1(const PluginColor& c1, const PluginColor& c2, float32_t t, PluginColor& out);
	static float32_t Lerp2(float32_t a, float32_t b, float32_t t);
	static float64_t Lerp2(float64_t a, float64_t b, float64_t t);
	static plQuaternion Slerp(const plQuaternion& pStart, const plQuaternion& pEnd, const float32_t pFactor);
	static float32_t Normalize(float32_t max, float32_t val);
	static plVec2i WorldToScreen(const plMat4& VP, const plVec3& point3D, const plVec2f& viewportSize, const plVec2f& offset);
	static plVec2i ScreenToClient(const plVec2i& screen_coord, const plVec4i& client_rect);
	//	static void PerspectiveLHMatrix(alMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far);
	static void PerspectiveRHMatrix(plMat4& out, float32_t FOV, float32_t aspect, float32_t Near, float32_t Far);
	static void OrthoRHMatrix(plMat4& out, float32_t width, float32_t height, float32_t Near, float32_t Far);
	static void OrthographicOffCenterRHMatrix(plMat4& out, float32_t ViewLeft, float32_t ViewRight, float32_t ViewBottom, float32_t ViewTop, float32_t NearZ, float32_t FarZ);
	static void LookAtRHMatrix(plMat4& out, const plVec3& eye, const plVec3& center, const plVec3& up = plVec3(0.f, 1.f, 0.f));
	static void TranslationMatrix(plMat4& out, const plVec3& position);
	static void ScaleMatrix(plMat4& out, const plVec3& scale);
	static plVec4 Mul(const plVec4& vec, const plMat4& mat);
	static plVec3 Mul(const plVec3& vec, const plMat4& mat);
	static plVec3f Mul(const plVec3f& vec, const plMat4& mat);
	static plVec3f Mul(const plVec3f& vec, const plMat3& mat);
	static plVec3f MulBasis(const plVec3f& vec, const plMat4& mat);
	static plQuaternion MatToQuat(const plMat4& mat);
};

#endif
