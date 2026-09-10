#pragma once
#ifndef _PLUGINVEC_H_
#define _PLUGINVEC_H_


template<typename Type>
class PluginVec4_t;

template<typename Type>
class PluginVec2_t
{
public:
	PluginVec2_t<Type>() {}
	PluginVec2_t<Type>(Type _x, Type _y) :x(_x), y(_y) {}
	PluginVec2_t<Type>(Type val) : x(val), y(val) {}
	Type x = 0;
	Type y = 0;

	void Set(Type _x, Type _y) { x = _x; y = _y; }
	PluginVec2_t<Type> operator*(const PluginVec2_t<Type>& v)const { PluginVec2_t<Type> r; r.x = x * v.x; r.y = y * v.y; return r; }
	PluginVec2_t<Type> operator-(const PluginVec2_t<Type>& v)const { PluginVec2_t<Type> r; r.x = x - v.x; r.y = y - v.y; return r; }
	PluginVec2_t<Type> operator+(const PluginVec2_t<Type>& v)const { PluginVec2_t<Type> r; r.x = x + v.x; r.y = y + v.y; return r; }
	PluginVec2_t<Type> operator/(const PluginVec2_t<Type>& v)const { PluginVec2_t<Type> r; r.x = x / v.x; r.y = y / v.y; return r; }

	void operator+=(const PluginVec2_t<Type>& v) { x += v.x; y += v.y; }
	void operator-=(const PluginVec2_t<Type>& v) { x -= v.x; y -= v.y; }
	void operator*=(const PluginVec2_t<Type>& v) { x *= v.x; y *= v.y; }
	void operator/=(const PluginVec2_t<Type>& v) { x /= v.x; y /= v.y; }
	bool operator==(const PluginVec2_t<Type>& v)const { if (x != v.x)return false; if (y != v.y)return false; return true; }
	Type Distance(const PluginVec2_t<Type>& from)const { return PluginVec2_t<Type>(x - from.x, y - from.y).Length(); }
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
	PluginVec2_t<Type> operator*(Type v)const { PluginVec2_t<Type> r; r.x = x * v; r.y = y * v; return r; }
};

template<typename Type>
class PluginVec3_t
{
public:
	PluginVec3_t<Type>() {}
	PluginVec3_t<Type>(const PluginVec4_t<Type>& v);// : x(v.x), y(v.y), z(v.z) {}
	PluginVec3_t<Type>(Type _v) :x(_v), y(_v), z(_v) {}
	PluginVec3_t<Type>(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z) {}
	Type x = 0;
	Type y = 0;
	Type z = 0;

	void Set(Type _x, Type _y, Type _z) { x = _x; y = _y; z = _z; }
	void Set(Type val) { x = y = z = val; }
	Type* data() { return &x; }
	Type Distance(const PluginVec3_t<Type>& from)const { return PluginVec3_t<Type>(x - from.x, y - from.y, z - from.z).Length(); }
	//Type Distance(const PluginVec4_t<Type>& from)const { return PluginVec3_t<Type>(x - from.x, y - from.y, z - from.z).Length(); }
	void operator+=(const PluginVec3_t<Type>& v) { x += v.x; y += v.y; z += v.z; }
	void operator-=(const PluginVec3_t<Type>& v) { x -= v.x; y -= v.y; z -= v.z; }
	void operator*=(const PluginVec3_t<Type>& v) { x *= v.x; y *= v.y; z *= v.z; }
	void operator/=(const PluginVec3_t<Type>& v) { x /= v.x; y /= v.y; z /= v.z; }
	Type operator[](int32_t index)const { return (&x)[index]; }
	Type& operator[](int32_t index) { return (&x)[index]; }
	PluginVec3_t<Type> operator+(const PluginVec3_t<Type>& v)const { PluginVec3_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
	PluginVec3_t<Type> operator-(const PluginVec3_t<Type>& v)const { PluginVec3_t<Type> r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
	PluginVec3_t<Type> operator*(const PluginVec3_t<Type>& v)const { PluginVec3_t<Type> r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
	PluginVec3_t<Type> operator/(const PluginVec3_t<Type>& v)const { PluginVec3_t<Type> r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
	//PluginVec3_t<Type> operator+(const PluginVec4_t<Type>& v)const { PluginVec3_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
	//PluginVec3_t<Type>& operator=(const PluginVec4_t<Type>& v) { x = v.x; y = v.y; z = v.z; return *this; }
	//void operator+=(const PluginVec4_t<Type>& v) { x += v.x; y += v.y; z += v.z; }
	bool operator==(const PluginVec3_t<Type>& v)const {
		if (x != v.x)return false;
		if (y != v.y)return false;
		if (z != v.z)return false;
		return true;
	}
	bool operator!=(const PluginVec3_t<Type>& v)const {
		if (x != v.x)return true;
		if (y != v.y)return true;
		if (z != v.z)return true;
		return false;
	}
	PluginVec3_t<Type> Cross(const PluginVec3_t<Type>& a)const {
		PluginVec3_t<Type> out;
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
		return out;
	}
	void Cross2(const PluginVec3_t<Type>& a, PluginVec3_t<Type>& out)const {
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
	Type	Dot(const PluginVec3_t<Type>& V2)const { return (x * V2.x) + (y * V2.y) + (z * V2.z); }
	Type	Dot()const { return (x * x) + (y * y) + (z * z); }
	PluginVec3_t<Type> operator-()const { PluginVec3_t<Type> r; r.x = -x; r.y = -y; r.z = -z; return r; }
	Type Length() const { return std::sqrt(Length2()); }
	Type Length2() const { return Dot(*this); }

	//void add(const PluginVec4_t<Type>&);
};

template<typename Type>
class PluginVec4_t
{
public:
	PluginVec4_t<Type>() {}
	PluginVec4_t<Type>(Type _v) :x(_v), y(_v), z(_v), w(_v) {}
	PluginVec4_t<Type>(Type _x, Type _y, Type _z, Type _w) : x(_x), y(_y), z(_z), w(_w) {}
	PluginVec4_t<Type>(const PluginVec3_t<float32_t>& _v) : x(_v.x), y(_v.y), z(_v.z), w(0) {}
	Type x = 0;
	Type y = 0;
	Type z = 0;
	Type w = 0;

	Type* Data() { return &x; }
	Type operator[](int32_t index)const { return (&x)[index]; }
	Type& operator[](int32_t index) {return (&x)[index]; }

	void Set(Type _x, Type _y, Type _z, Type _w) { x = _x; y = _y; z = _z; w = _w; }
	void Set(Type val) { x = y = z = w = val; }
	void operator+=(const PluginVec4_t<Type>& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	void operator-=(const PluginVec4_t<Type>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator*=(const PluginVec4_t<Type>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
	void operator/=(const PluginVec4_t<Type>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }

	PluginVec4_t<Type> operator+(const PluginVec4_t<Type>& v)const { PluginVec4_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }
	PluginVec4_t<Type> operator-(const PluginVec4_t<Type>& v)const { PluginVec4_t<Type> r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; r.w = w - v.w; return r; }
	PluginVec4_t<Type> operator*(const PluginVec4_t<Type>& v)const { PluginVec4_t<Type> r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; r.w = w * v.w; return r; }
	PluginVec4_t<Type> operator/(const PluginVec4_t<Type>& v)const { PluginVec4_t<Type> r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; r.w = w / v.w; return r; }

	PluginVec4_t<Type> operator-()const { PluginVec4_t<Type> r; r.x = -x; r.y = -y; r.z = -z; r.w = -w; return r; }

	//PluginVec4_t<Type> operator+(const PluginVec4_t<float64_t>& v)const { PluginVec4_t<Type> r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }

	Type	Dot(const PluginVec4_t<Type>& V2)const { return (x * V2.x) + (y * V2.y) + (z * V2.z) + (w * V2.w); }
	Type	Dot()const { return (x * x) + (y * y) + (z * z) + (w * w); }
	void	Normalize2()
	{
		Type len = std::sqrt(Dot());
		if (len > 0)
			len = 1.0f / len;
		x *= len; y *= len; z *= len; w *= len;
	}
	void Cross2(const PluginVec4_t<Type>& a, PluginVec4_t<Type>& out)const {
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
	}
	void Cross2(const PluginVec4_t<Type>& a, PluginVec3_t<Type>& out)const {
		out.x = (y * a.z) - (z * a.y);
		out.y = (z * a.x) - (x * a.z);
		out.z = (x * a.y) - (y * a.x);
	}
	Type LengthSqrt()const { return(x * x) + (y * y) + (z * z); }
	Type Length()const { return std::sqrt(LengthSqrt()); }
	Type Distance(const PluginVec4_t<Type>& from)const { return PluginVec4_t<Type>(x - from.x, y - from.y, z - from.z, 1.f).Length(); }
	bool operator==(const PluginVec4_t<Type>& v)const {
		if (x != v.x)return false;
		if (y != v.y)return false;
		if (z != v.z)return false;
		if (w != v.w)return false;
		return true;
	}
};
PLUGIN_FORCE_INLINE PluginVec4_t<float32_t> operator*(const float32_t& s, const PluginVec4_t<float32_t>& v) { return v * s; }
PLUGIN_FORCE_INLINE PluginVec4_t<float64_t> operator*(const float64_t& s, const PluginVec4_t<float64_t>& v) { return v * s; }
PLUGIN_FORCE_INLINE PluginVec4_t<float64_t> operator+(const PluginVec4_t<float64_t>& s, const PluginVec4_t<float32_t>& v)
{
	PluginVec4_t<float64_t> r;
	r.x = s.x + v.x;
	r.y = s.y + v.y;
	r.z = s.z + v.z;
	r.w = s.w + v.w;
	return r;
}
PLUGIN_FORCE_INLINE PluginVec4_t<float32_t> operator-(const PluginVec4_t<float32_t>& s, const PluginVec4_t<float64_t>& v)
{
	PluginVec4_t<float32_t> r;
	r.x = s.x - (float32_t)v.x;
	r.y = s.y - (float32_t)v.y;
	r.z = s.z - (float32_t)v.z;
	r.w = s.w - (float32_t)v.w;
	return r;
}

template<typename T>
PluginVec3_t<T>::PluginVec3_t(const PluginVec4_t<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

using plVec2f = PluginVec2_t<float32_t>;
using plVec2i = PluginVec2_t<int32_t>;
using plVec2u = PluginVec2_t<uint32_t>;
using plVec3f = PluginVec3_t<float32_t>;
using plVec3i = PluginVec3_t<int32_t>;
using plVec3u = PluginVec3_t<uint32_t>;
using plVec4f = PluginVec4_t<float32_t>;
using plVec4i = PluginVec4_t<int32_t>;
using plVec4u = PluginVec4_t<uint32_t>;
using plVec3 = PluginVec3_t<float64_t>;
using plVec4 = PluginVec4_t<float64_t>;


#endif