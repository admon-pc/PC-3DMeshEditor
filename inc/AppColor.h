#pragma once
#ifndef _ELCLR_H_
#define _ELCLR_H_


class AppColorBytes
{
public:
	AppColorBytes()
		:
		r(0),
		g(0),
		b(0),
		a(255)
	{}
	AppColorBytes(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
		:
		r(R),
		g(G),
		b(B),
		a(A)
	{}

	AppColorBytes(uint32_t uint_data) // 0xFF112233 ARGB
	{
		r = static_cast<uint8_t>(uint_data >> 16u);
		g = static_cast<uint8_t>(uint_data >> 8u);
		b = static_cast<uint8_t>(uint_data);
		a = static_cast<uint8_t>(uint_data >> 24u);
	}

	uint8_t r, g, b, a;

	bool	operator==(const AppColorBytes& v) const
	{
		if (r != v.r) return false;
		if (g != v.g) return false;
		if (b != v.b) return false;
		if (a != v.a) return false;
		return true;
	}

	bool	operator!=(const AppColorBytes& v) const
	{
		if (r != v.r) return true;
		if (g != v.g) return true;
		if (b != v.b) return true;
		if (a != v.a) return true;
		return false;
	}
};

class AppColor
{
public:

	float32_t	m_data[4u];

	AppColor()
	{
		m_data[0u] = m_data[1u] = m_data[2u] = 0.f;
		m_data[3u] = 1.f;
	}

	AppColor(float32_t v)
	{
		m_data[0u] = m_data[1u] = m_data[2u] = v;
		m_data[3u] = 1.f;
	}

	AppColor(float32_t r, float32_t g, float32_t b, float32_t a = 1.f)
	{
		m_data[0u] = r;
		m_data[1u] = g;
		m_data[2u] = b;
		m_data[3u] = a;
	}

	AppColor(int32_t r, int32_t g, int32_t b, int32_t a = 255)
	{
		this->SetAsByteAlpha(a);
		this->SetAsByteRed(r);
		this->SetAsByteGreen(g);
		this->setAsByteBlue(b);
	}

	AppColor(uint32_t uint_data) // 0xFF112233 ARGB
	{
		SetAsInteger(uint_data);
	}

	float32_t X() { return m_data[0]; }
	float32_t Y() { return m_data[1]; }
	float32_t Z() { return m_data[2]; }
	float32_t W() { return m_data[3]; }

	const float32_t* Data() const { return &m_data[0u]; }

	const float32_t GetRed() const { return m_data[0u]; }
	const float32_t GetGreen() const { return m_data[1u]; }
	const float32_t GetBlue() const { return m_data[2u]; }
	const float32_t GetAlpha() const { return m_data[3u]; }

	const uint8_t GetAsByteRed() const { return static_cast<uint8_t>(m_data[0u] * 255.); }
	const uint8_t GetAsByteGreen() const { return static_cast<uint8_t>(m_data[1u] * 255.); }
	const uint8_t GetAsByteBlue() const { return static_cast<uint8_t>(m_data[2u] * 255.); }
	const uint8_t GetAsByteAlpha() const { return static_cast<uint8_t>(m_data[3u] * 255.); }

	bool	operator==(const AppColor& v) const
	{
		if (m_data[0] != v.m_data[0]) return false;
		if (m_data[1] != v.m_data[1]) return false;
		if (m_data[2] != v.m_data[2]) return false;
		if (m_data[3] != v.m_data[3]) return false;
		return true;
	}

	bool	operator!=(const AppColor& v) const
	{
		if (m_data[0] != v.m_data[0]) return true;
		if (m_data[1] != v.m_data[1]) return true;
		if (m_data[2] != v.m_data[2]) return true;
		if (m_data[3] != v.m_data[3]) return true;
		return false;
	}

	void Normalize()
	{
		float32_t len = std::sqrt((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) + (m_data[2] * m_data[2]));
		if (len > 0)
		{
			len = 1.0f / len;
		}
		m_data[0] *= len;
		m_data[1] *= len;
		m_data[2] *= len;
	}

	// 0xff112233
	uint32_t GetAsInteger()
	{
		return APP_MAKEFOURCC(
			this->GetAsByteBlue(),
			this->GetAsByteGreen(),
			this->GetAsByteRed(),
			this->GetAsByteAlpha()
		);
	}

	void SetAlpha(float32_t v) { m_data[3u] = v; }
	void SetRed(float32_t v) { m_data[0u] = v; }
	void SetGreen(float32_t v) { m_data[1u] = v; }
	void SetBlue(float32_t v) { m_data[2u] = v; }

	void SetAsByteAlpha(int32_t v) { m_data[3u] = static_cast<float32_t>(v) * 0.00392156862745f; }
	void SetAsByteRed(int32_t v) { m_data[0u] = static_cast<float32_t>(v) * 0.00392156862745f; }
	void SetAsByteGreen(int32_t v) { m_data[1u] = static_cast<float32_t>(v) * 0.00392156862745f; }
	void setAsByteBlue(int32_t v) { m_data[2u] = static_cast<float32_t>(v) * 0.00392156862745f; }

	void SetAsInteger(uint32_t v)
	{
		this->SetAsByteRed(static_cast<uint8_t>(v >> 16u));
		this->SetAsByteGreen(static_cast<uint8_t>(v >> 8u));
		this->setAsByteBlue(static_cast<uint8_t>(v));
		this->SetAsByteAlpha(static_cast<uint8_t>(v >> 24u));
	}

	void Set(float32_t r, float32_t g, float32_t b, float32_t a = 1.)
	{
		SetAlpha(a);
		SetRed(r);
		SetGreen(g);
		SetBlue(b);
	}

	void Set(float32_t v, float32_t a = 1.)
	{
		SetAlpha(a);
		SetRed(v);
		SetGreen(v);
		SetBlue(v);
	}

	void Set(const AppColor& other)
	{
		*this = other;
	}
};

#endif
