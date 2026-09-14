#pragma once
#ifndef _ELVEC_H_
#define _ELVEC_H_


template<typename Type>
class AppVec4_t;

template<typename Type>
class AppVec2_t
{
public:
	AppVec2_t<Type>() {}
	AppVec2_t<Type>(Type _x, Type _y) :x(_x), y(_y) {}
	AppVec2_t<Type>(Type val) : x(val), y(val) {}
	Type x = 0;
	Type y = 0;

	void Set(Type _x, Type _y) { x = _x; y = _y; }
	AppVec2_t<Type> operator*(const AppVec2_t<Type>& v)const { AppVec2_t<Type> r; r.x = x * v.x; r.y = y * v.y; return r; }
	AppVec2_t<Type> operator-(const AppVec2_t<Type>& v)const { AppVec2_t<Type> r; r.x = x - v.x; r.y = y - v.y; return r; }
	AppVec2_t<Type> operator+(const AppVec2_t<Type>& v)const { AppVec2_t<Type> r; r.x = x + v.x; r.y = y + v.y; return r; }
	AppVec2_t<Type> operator/(const AppVec2_t<Type>& v)const { AppVec2_t<Type> r; r.x = x / v.x; r.y = y / v.y; return r; }

	void operator+=(const AppVec2_t<Type>& v) { x += v.x; y += v.y; }
	void operator-=(const AppVec2_t<Type>& v) { x -= v.x; y -= v.y; }
	void operator*=(const AppVec2_t<Type>& v) { x *= v.x; y *= v.y; }
	void operator/=(const AppVec2_t<Type>& v) { x /= v.x; y /= v.y; }
	bool operator==(const AppVec2_t<Type>& v)const { if (x != v.x)return false; if (y != v.y)return false; return true; }
	Type Distance(const AppVec2_t<Type>& from)const { return AppVec2_t<Type>(x - from.x, y - from.y).Length(); }
	Type Length()const { return std::sqrt(LengthSqrt()); }
	Type LengthSqrt()const { return(x * x) + (y * y); }
	Type	Dot()const { return (x * x) + (y * y); }
	void	Normalize2()
	{
		Type len = std::sqrt(Dot());
		if (len > 0)
			len = 1.0f / len;
		x *= len; y *= len;
	}
	AppVec2_t<Type> operator*(Type v)const { AppVec2_t<Type> r; r.x = x * v; r.y = y * v; return r; }
};

template<typename Type>
class AppVec3_t
{
public:
	AppVec3_t<Type>() {}
	AppVec3_t<Type>(const AppVec4_t<Type>& v);// : x(v.x), y(v.y), z(v.z) {}
	AppVec3_t<Type>(Type _v) :x(_v), y(_v), z(_v) {}
	AppVec3_t<Type>(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z) {}
	Type x = 0;
	Type y = 0;
	Type z = 0;

	void Set(Type _x, Type _y, Type _z) { x = _x; y = _y; z = _z; }
	void Set(Type val) { x = y = z = val; }
	Type* data() { return &x; }
	Type Distance(const AppVec3_t<Type>& from)const { return AppVec3_t<Type>(x - from.x, y - from.y, z - from.z).Length(); }
	//Type Distance(const AppVec4_t<Type>& from)const { return AppVec3_t<Type>(x - from.x, y - from.y, z - from.z).Length(); }
	void operator+=(const AppVec3_t<Type>& v) { x += v.x; y += v.y; z += v.z; }
	void operator-=(const AppVec3_t<Type>& v) { x -= v.x; y -= v.y; z -= v.z; }
	void operator*=(const AppVec3_t<Type>& v) { x *= v.x; y *= v.y; z *= v.z; }
	void operator/=(const AppVec3_t<Type>& v) { x /= v.x; y /= v.y; z /= v.z; }
	Type operator[](int32_t index)const { return (&x)[index]; }
	Type& operator[](int32_t index) { return (&x)[index]; }
	AppVec3_t<Type> operator+(const AppVec3_t<Type>& v)const { AppVec3_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
	AppVec3_t<Type> operator-(const AppVec3_t<Type>& v)const { AppVec3_t<Type> r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
	AppVec3_t<Type> operator*(const AppVec3_t<Type>& v)const { AppVec3_t<Type> r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
	AppVec3_t<Type> operator/(const AppVec3_t<Type>& v)const { AppVec3_t<Type> r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
	//AppVec3_t<Type> operator+(const AppVec4_t<Type>& v)const { AppVec3_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
	//AppVec3_t<Type>& operator=(const AppVec4_t<Type>& v) { x = v.x; y = v.y; z = v.z; return *this; }
	//void operator+=(const AppVec4_t<Type>& v) { x += v.x; y += v.y; z += v.z; }
	bool operator==(const AppVec3_t<Type>& v)const {
		if (x != v.x)return false;
		if (y != v.y)return false;
		if (z != v.z)return false;
		return true;
	}
	bool operator!=(const AppVec3_t<Type>& v)const {
		if (x != v.x)return true;
		if (y != v.y)return true;
		if (z != v.z)return true;
		return false;
	}
	AppVec3_t<Type> Cross(const AppVec3_t<Type>& a)const {
		AppVec3_t<Type> out;
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
		return out;
	}
	void Cross2(const AppVec3_t<Type>& a, AppVec3_t<Type>& out)const {
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
	}
	void	Normalize2()
	{
		Type len = std::sqrt(Dot());
		if (len > 0)
			len = 1.0f / len;
		x *= len; y *= len; z *= len;
	}
	Type	Dot(const AppVec3_t<Type>& V2)const { return (x * V2.x) + (y * V2.y) + (z * V2.z); }
	Type	Dot()const { return (x * x) + (y * y) + (z * z); }
	AppVec3_t<Type> operator-()const { AppVec3_t<Type> r; r.x = -x; r.y = -y; r.z = -z; return r; }
	Type Length() const { return std::sqrt(Length2()); }
	Type Length2() const { return Dot(*this); }

	//void add(const AppVec4_t<Type>&);
};

template<typename Type>
class AppVec4_t
{
public:
	AppVec4_t<Type>() {}
	AppVec4_t<Type>(Type _v) :x(_v), y(_v), z(_v), w(_v) {}
	AppVec4_t<Type>(Type _x, Type _y, Type _z, Type _w) : x(_x), y(_y), z(_z), w(_w) {}
	AppVec4_t<Type>(const AppVec3_t<float32_t>& _v) : x(_v.x), y(_v.y), z(_v.z), w(0) {}
	Type x = 0;
	Type y = 0;
	Type z = 0;
	Type w = 0;

	Type* Data() { return &x; }
	Type operator[](int32_t index)const { return (&x)[index]; }
	Type& operator[](int32_t index) {return (&x)[index]; }

	void Set(Type _x, Type _y, Type _z, Type _w) { x = _x; y = _y; z = _z; w = _w; }
	void Set(Type val) { x = y = z = w = val; }
	void operator+=(const AppVec4_t<Type>& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	void operator-=(const AppVec4_t<Type>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator*=(const AppVec4_t<Type>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
	void operator/=(const AppVec4_t<Type>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }

	AppVec4_t<Type> operator+(const AppVec4_t<Type>& v)const { AppVec4_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }
	AppVec4_t<Type> operator-(const AppVec4_t<Type>& v)const { AppVec4_t<Type> r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; r.w = w - v.w; return r; }
	AppVec4_t<Type> operator*(const AppVec4_t<Type>& v)const { AppVec4_t<Type> r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; r.w = w * v.w; return r; }
	AppVec4_t<Type> operator/(const AppVec4_t<Type>& v)const { AppVec4_t<Type> r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; r.w = w / v.w; return r; }

	AppVec4_t<Type> operator-()const { AppVec4_t<Type> r; r.x = -x; r.y = -y; r.z = -z; r.w = -w; return r; }

	//AppVec4_t<Type> operator+(const AppVec4_t<float64_t>& v)const { AppVec4_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }

	Type	Dot(const AppVec4_t<Type>& V2)const { return (x * V2.x) + (y * V2.y) + (z * V2.z) + (w * V2.w); }
	Type	Dot()const { return (x * x) + (y * y) + (z * z) + (w * w); }
	void	Normalize2()
	{
		Type len = std::sqrt(Dot());
		if (len > 0)
			len = 1.0f / len;
		x *= len; y *= len; z *= len; w *= len;
	}
	void Cross2(const AppVec4_t<Type>& a, AppVec4_t<Type>& out)const {
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
	}
	void Cross2(const AppVec4_t<Type>& a, AppVec3_t<Type>& out)const {
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
	}
	Type LengthSqrt()const { return(x * x) + (y * y) + (z * z); }
	Type Length()const { return std::sqrt(LengthSqrt()); }
	Type Distance(const AppVec4_t<Type>& from)const { return AppVec4_t<Type>(x - from.x, y - from.y, z - from.z, 1.f).Length(); }
	bool operator==(const AppVec4_t<Type>& v)const {
		if (x != v.x)return false;
		if (y != v.y)return false;
		if (z != v.z)return false;
		if (w != v.w)return false;
		return true;
	}
};
APP_FORCE_INLINE AppVec4_t<float32_t> operator*(const float32_t& s, const AppVec4_t<float32_t>& v) { return v * s; }
APP_FORCE_INLINE AppVec4_t<float64_t> operator*(const float64_t& s, const AppVec4_t<float64_t>& v) { return v * s; }
APP_FORCE_INLINE AppVec4_t<float64_t> operator+(const AppVec4_t<float64_t>& s, const AppVec4_t<float32_t>& v)
{
	AppVec4_t<float64_t> r;
	r.x = s.x + v.x;
	r.y = s.y + v.y;
	r.z = s.z + v.z;
	r.w = s.w + v.w;
	return r;
}
APP_FORCE_INLINE AppVec4_t<float32_t> operator-(const AppVec4_t<float32_t>& s, const AppVec4_t<float64_t>& v)
{
	AppVec4_t<float32_t> r;
	r.x = s.x - (float32_t)v.x;
	r.y = s.y - (float32_t)v.y;
	r.z = s.z - (float32_t)v.z;
	r.w = s.w - (float32_t)v.w;
	return r;
}

template<typename T>
AppVec3_t<T>::AppVec3_t(const AppVec4_t<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

using AppVec2f = AppVec2_t<float32_t>;
using AppVec2i = AppVec2_t<int32_t>;
using AppVec2u = AppVec2_t<uint32_t>;
using AppVec3f = AppVec3_t<float32_t>;
using AppVec3i = AppVec3_t<int32_t>;
using AppVec3u = AppVec3_t<uint32_t>;
using AppVec4f = AppVec4_t<float32_t>;
using AppVec4i = AppVec4_t<int32_t>;
using AppVec4u = AppVec4_t<uint32_t>;
using AppVec3 = AppVec3_t<float64_t>;
using AppVec4 = AppVec4_t<float64_t>;

const AppVec4 AppVec4FltMax = AppVec4(FLT_MAX);
const AppVec4 AppVec4FltMaxNeg = AppVec4(-FLT_MAX);
const AppVec4f AppVec4fFltMax = AppVec4f(FLT_MAX);
const AppVec4f AppVec4fFltMaxNeg = AppVec4f(-FLT_MAX);

#endif