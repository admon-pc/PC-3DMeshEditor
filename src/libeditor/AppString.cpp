#include "editor.lib.h"

template<typename other_type>
size_t Getlen(const other_type* str)
{
	uint32_t len = 0;
	if (str[0] != 0)
	{
		const other_type* p = &str[0];
		while ((size_t)*p++)
			len++;
	}
	return len;
}

namespace al_internal
{
	const size_t string_wordSize = 16;
	const float32_t string_to_float_table[17] =
	{
		0.f,
		0.1f,
		0.01f,
		0.001f,
		0.0001f,
		0.00001f,
		0.000001f,
		0.0000001f,
		0.00000001f,
		0.000000001f,
		0.0000000001f,
		0.00000000001f,
		0.000000000001f,
		0.0000000000001f,
		0.00000000000001f,
		0.000000000000001f,
		0.0000000000000001f,
	};
	const float64_t string_to_float_table_d[17] =
	{
		0.,
		0.1,
		0.01,
		0.001,
		0.0001,
		0.00001,
		0.000001,
		0.0000001,
		0.00000001,
		0.000000001,
		0.0000000001,
		0.00000000001,
		0.000000000001,
		0.0000000000001,
		0.00000000000001,
		0.000000000000001,
		0.0000000000000001,
	};
}
AppUnicodeConverter::AppUnicodeConverter(){}
AppUnicodeConverter::~AppUnicodeConverter() {}
void AppUnicodeConverter::Set(char32_t c)
{
	m_32 = c;
	_find16From32();
	_find8From32();
}

void AppUnicodeConverter::Set(char16_t c)
{
	m_32 = c;
	m_16[0] = c;
	m_16[1] = 0;
	m_16Num = 1;
	_find8From32();
}

void AppUnicodeConverter::Set(char8_t c)
{
	m_32 = c;
	m_16[0] = c;
	m_16[1] = 0;
	m_16Num = 1;
	m_8[0] = c;
	m_8[1] = 0;
	m_8[2] = 0;
	m_8[3] = 0;
	m_8Num = 1;
}

void AppUnicodeConverter::Set(char c)
{
	m_32 = c;
	m_16[0] = c;
	m_16[1] = 0;
	m_16Num = 1;
	m_8[0] = c;
	m_8[1] = 0;
	m_8[2] = 0;
	m_8[3] = 0;
	m_8Num = 1;
}

void AppUnicodeConverter::Set(wchar_t c)
{
	Set((char32_t)c);
}
void AppUnicodeConverter::_find16From32()
{
	if (m_32 > 0xFFFF)
	{
		char32_t _u = m_32 - 0x10000;
		m_16[0] = 0xD800 + (_u >> 10);
		m_16[1] = 0xDC00 + (_u % 0x400);
		m_16Num = 2;
	}
	else
	{
		m_16[0] = m_32;
		m_16[1] = 0;
		m_16Num = 1;
	}
}

void AppUnicodeConverter::_find8From32()
{
	// u vvvv    wwww xxxx    yyyy zzzz
	char32_t _u = m_32;

	if (m_32 > 0xFFFF)
	{

		uint32_t vvvv = (_u & 0xF0000) >> 16;
		uint32_t wwww = (_u & 0xF000) >> 12;
		uint32_t xxxx = (_u & 0xF00) >> 8;
		uint32_t yyyy = (_u & 0xF0) >> 4;
		uint32_t zzzz = (_u & 0xF);

		m_8[0] = 0xF0 + ((_u & 0x100000) >> 18);
		m_8[0] |= vvvv >> 2;

		m_8[1] = 0x80 + wwww;
		m_8[1] |= (vvvv & 0x3) << 4;

		m_8[2] = 0x80 + (xxxx << 2);
		m_8[2] |= yyyy >> 2;

		m_8[3] = 0x80 + (zzzz);
		m_8[3] |= (yyyy & 0x3) << 4;

		m_8Num = 4;
	}
	else if (m_32 > 0x7FF)
	{
		uint32_t wwww = (_u & 0xF000) >> 12;
		uint32_t xxxx = (_u & 0xF00) >> 8;
		uint32_t yyyy = (_u & 0xF0) >> 4;
		uint32_t zzzz = (_u & 0xF);

		m_8[0] = 0xE0 + wwww;

		m_8[1] = 0x80 + (xxxx << 2);
		m_8[1] |= yyyy >> 2;

		m_8[2] = 0x80 + (zzzz);
		m_8[2] |= (yyyy & 0x3) << 4;

		m_8[3] = 0;

		m_8Num = 3;
	}
	else if (m_32 > 0x7F)
	{
		uint32_t xxxx = (_u & 0xF00) >> 8;
		uint32_t yyyy = (_u & 0xF0) >> 4;
		uint32_t zzzz = (_u & 0xF);

		m_8[0] = 0xC0 + ((xxxx & 0x7) << 2);
		m_8[0] |= (yyyy >> 2);

		m_8[1] = 0x80 + (zzzz);
		m_8[1] |= (yyyy & 0x3) << 4;

		m_8[2] = 0;
		m_8[3] = 0;

		m_8Num = 2;
	}
	else
	{
		uint32_t yyyy = (_u & 0xF0) >> 4;
		uint32_t zzzz = (_u & 0xF);

		m_8[0] = zzzz;
		m_8[0] |= (yyyy & 0x7) << 4;

		m_8[1] = 0;
		m_8[2] = 0;
		m_8[3] = 0;

		m_8Num = 1;
	}
}

uint32_t AppUnicodeConverter::Set(const AppVec4u& c)
{
	uint32_t c1 = c.x;
	uint32_t c2 = c.y;
	uint32_t c3 = c.z;
	uint32_t c4 = c.w;

	m_32 = 0;
	m_16[0] = 0;
	m_16[1] = 0;
	m_8[0] = 0;
	m_8[1] = 0;
	m_8[2] = 0;
	m_8[3] = 0;
	m_16Num = 0;
	m_8Num = 0;
	// u vvvv    wwww xxxx    yyyy zzzz

	if ((c1 & 0xF8) == 0xF0)
	{
		// this is MUST BE 4 bytes utf8
		if ((c2 & 0xC0) == 0x80)
		{
			if ((c3 & 0xC0) == 0x80)
			{
				if ((c4 & 0xC0) == 0x80)
				{
					m_32 = (c1 & 0x7) << 18;
					m_32 |= (c2 & 0x3F) << 12;
					m_32 |= (c3 & 0x3F) << 6;
					m_32 |= (c3 & 0x3F);
					m_8[0] = c1;
					m_8[1] = c2;
					m_8[2] = c3;
					m_8[3] = c4;
					m_8Num = 4;
				}
			}
		}
	}
	else if ((c1 & 0xF0) == 0xE0)
	{
		// this is MUST BE 3 bytes utf8
		if ((c2 & 0xC0) == 0x80)
		{
			if ((c3 & 0xC0) == 0x80)
			{
				m_32 = (c1 & 0xF) << 12;
				m_32 |= (c2 & 0x3F) << 6;
				m_32 |= (c3 & 0x3F);
				m_8[0] = c1;
				m_8[1] = c2;
				m_8[2] = c3;
				m_8Num = 3;
			}
		}
	}
	else if ((c1 & 0xE0) == 0xC0)
	{
		// this is MUST BE 2 bytes utf8
		if ((c2 & 0xC0) == 0x80)
		{
			m_32 = (c1 & 0x1F) << 6;
			m_32 |= (c2 & 0x3F);
			m_8[0] = c1;
			m_8[1] = c2;
			m_8Num = 2;
		}
	}
	else
	{
		m_32 = c1;
		m_8[0] = c1;
		m_8Num = 1;
	}

	_find16From32();

	return m_8Num;
}

uint32_t AppUnicodeConverter::Set(char16_t c1, char16_t c2)
{
	m_32 = 0;
	m_16[0] = 0;
	m_16[1] = 0;
	m_8[0] = 0;
	m_8[1] = 0;
	m_8[2] = 0;
	m_8[3] = 0;
	m_16Num = 0;
	m_8Num = 0;

	if ((c1 & 0xFC00) == 0xD800)
	{
		if ((c2 & 0xFC00) == 0xDC00)
		{
			// 2 char16_t
			m_32 = (c1 & 0x3FF) << 10;
			m_32 |= (c2 & 0x3FF);
			m_16Num = 2;
		}
	}
	else
	{
		m_32 = c1;
		m_16Num = 1;
	}

	_find8From32();

	return m_16Num;
}

void AppUnicodeConverter::wchar_to_char(const wchar_t* str, size_t sz, std::string* out)
{
	AppUnicodeConverter uc;
	if (str && sz && out)
	{
		out->clear();
		for (size_t i = 0; i < sz; )
		{
			wchar_t c1 = str[i];
			wchar_t c2 = 0;
			if (i + 1 < sz)
				c2 = str[i + 1];

			auto n = uc.Set(c1, c2);
			if (uc.m_8Num)
			{
				for (size_t o = 0; o < uc.m_8Num; ++o)
				{
					out->push_back(uc.m_8[o]);
				}
			}
			i += n;
		}
	}
}
void AppUnicodeConverter::char_to_wchar(const char* str, size_t sz, std::wstring* out)
{
	AppUnicodeConverter uc;
	if (str && sz && out)
	{
		out->clear();
		for (size_t i = 0; i < sz; )
		{
			char c1 = str[i];
			char c2 = 0;
			char c3 = 0;
			char c4 = 0;

			if (i + 1 < sz) c2 = str[i + 1];
			if (i + 2 < sz) c3 = str[i + 2];
			if (i + 3 < sz) c4 = str[i + 3];

			auto n = uc.Set(AppVec4u(c1, c2, c3, c4));
			if (uc.m_16Num)
			{
				for (size_t o = 0; o < uc.m_16Num; ++o)
				{
					out->push_back(uc.m_16[o]);
				}
			}
			i += n;
		}
	}
}



void AppString::_reallocate(size_t new_allocated)
{
	char32_t* new_data = (char32_t*)AppMalloc(new_allocated * sizeof(char32_t));
	if (m_data)
	{
		std::memcpy(new_data, m_data, m_size * sizeof(char32_t));
		AppFree(m_data);
	}
	else
	{
		std::memset(new_data, 0, new_allocated);
	}
	m_data = new_data;
	m_allocated = new_allocated;
}

AppString::AppString()
{
	_reallocate(al_internal::string_wordSize);
}

AppString::AppString(const char8_t* str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(const char* str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(const wchar_t* str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(const char16_t* str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(const char32_t* str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(const AppString& str)
{
	_reallocate(al_internal::string_wordSize);
	Append(str);
}

AppString::AppString(AppString&& str) noexcept
{
	m_allocated = str.m_allocated;
	m_data = str.m_data;
	m_size = str.m_size;
	str.m_allocated = 0;
	str.m_data = 0;
	str.m_size = 0;
}

AppString::~AppString()
{
	_free();
}

void AppString::_free()
{
	if (m_data)
	{
		AppFree(m_data);
		m_data = 0;
	}
}

void AppString::Assign(const char* v)
{
	Clear();
	Append(v);
}

void AppString::Assign(const char8_t* v)
{
	Clear();
	Append(v);
}

void AppString::Assign(const char16_t* v)
{
	Clear();
	Append(v);
}

void AppString::Assign(const wchar_t* v)
{
	Clear();
	Append(v);
}

void AppString::Assign(const char32_t* v)
{
	Clear();
	Append(v);
}

void AppString::Assign(const AppString& v)
{
	Clear();
	Append(v);
}

size_t AppString::Size()
{
	return m_size;
}

size_t AppString::Capacity()
{
	return m_allocated;
}

void AppString::Reserve(size_t s)
{
	if (s > m_allocated)
	{
		_reallocate((s + 1) + al_internal::string_wordSize);
	}
}

void AppString::Clear()
{
	m_size = 0;
	m_data[m_size] = 0;
}

void AppString::PopBack()
{
	if (m_size)
	{
		--m_size;
		m_data[m_size] = 0;
	}
}

void AppString::PushBack(char32_t c)
{
	size_t new_size = m_size + 1;
	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));
	m_data[m_size] = c;
	m_size = new_size;
	m_data[m_size] = 0;
}

char32_t* AppString::Data()
{
	return m_data;
}

const char32_t* AppString::c_str()
{
	return m_data;
}

void AppString::Append(const char8_t* str)
{
	size_t new_size = Getlen(str) + m_size;

	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));

	AppUnicodeConverter uc;

	const char8_t* p = str;
	while (*p)
	{
		auto p_begin = p;

		AppVec4u vec;
		vec.x = *p;
		++p;
		if (*p)
		{
			vec.y = *p;
			++p;
			if (*p)
			{
				vec.z = *p;
				++p;
				if (*p)
					vec.w = *p;
			}
		}

		auto ret = uc.Set(vec);
		if (!ret)
			break;

		PushBack(uc.m_32);

		p = p_begin + ret;
	}
}

void AppString::Append(const char* str)
{
	Append((const char8_t*)str);
}

void AppString::Append(const wchar_t* str)
{
	Append((const char16_t*)str);
}

void AppString::Append(const char16_t* str)
{
	size_t new_size = Getlen(str) + m_size;

	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));

	AppUnicodeConverter uc;

	const char16_t* p = str;
	while (*p)
	{
		auto p_begin = p;

		char16_t c1 = 0;
		char16_t c2 = 0;

		c1 = *p;

		++p;
		if (*p)
			c2 = *p;

		auto ret = uc.Set(c1, c2);
		if (!ret)
			break;

		PushBack(uc.m_32);

		p = p_begin + ret;
	}
}

// it can be that str will not have zero at the end
void AppString::Append(const char32_t* str, size_t size)
{
	size_t new_size = size + m_size;

	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));
	const char32_t* p = str;
	//while (*p)
	for (size_t i = 0; i < size; ++i)
	{
		PushBack(*p);
		++p;
	}
}

void AppString::Append(const char32_t* str)
{
	Append(str, Getlen(str));
}

void AppString::Append(const AppString& str)
{
	size_t new_size = str.m_size + m_size;
	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));
	const char32_t* p = str.m_data;
	while (*p)
	{
		PushBack(*p);
		++p;
	}
}

void AppString::Append(char32_t v)
{
	PushBack(v);
}

void AppString::Append(uint32_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%u", v);
	Append(buf);
}

void AppString::Append(uint64_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%llu", v);
	Append(buf);
}

void AppString::Append(int32_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%i", v);
	Append(buf);
}

void AppString::Append(int64_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%lli", v);
	Append(buf);
}

void AppString::Append(float32_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%f", v);
	Append(buf);
}

void AppString::AppendFloat(float32_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%.7f", v);
	Append(buf);
}

void AppString::Append(float64_t v)
{
	char buf[20];
	sprintf_s(buf, 20, "%.14f", v);
	Append(buf);
}

int32_t AppString::ToInt()
{
	size_t len = m_size;
	int32_t result = 0;
	int32_t mul_val = 1;
	bool is_neg = m_data[0] == U'-';
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int32_t char_value = (int32_t)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	if (is_neg)
		result = -result;
	return result;
}

uint32_t AppString::ToUint()
{
	if (m_data[0] == U'-')
		return 0;
	size_t len = m_size;
	int32_t result = 0;
	int32_t mul_val = 1;
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int32_t char_value = (int32_t)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	return result;
}

float32_t AppString::ToFloat()
{
	float32_t result = 0.f;
	auto ptr = m_data;
	bool is_negative = *ptr == U'-';

	if (is_negative)
		++ptr;

	int64_t i = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
	}
	result = (float32_t)i;

	i = 0;

	if (*ptr == U'.')
		++ptr;

	int64_t part_2_count = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
		++part_2_count;
	}
	result += (float32_t)i * al_internal::string_to_float_table[part_2_count];

	return is_negative ? -result : result;
}

float64_t AppString::ToFloat64()
{
	float64_t result = 0.;
	auto ptr = m_data;
	bool is_negative = *ptr == U'-';

	if (is_negative)
		++ptr;

	int64_t i = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += (*ptr) - U'0';
		++ptr;
	}
	result = (float64_t)i;

	i = 0;

	if (*ptr == U'.')
		++ptr;

	int32_t part_2_count = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
		++part_2_count;
	}
	result += (float64_t)i * al_internal::string_to_float_table_d[part_2_count];

	return is_negative ? -result : result;
}

void AppString::ToUTF8(std::string& str)
{
	str.clear();
	AppUnicodeConverter uc;
	for (size_t i = 0; i < m_size; ++i)
	{
		uc.Set(m_data[i]);

		if (!uc.m_8Num)
			break;

		for (size_t o = 0; o < uc.m_8Num; ++o)
		{
			str.push_back(uc.m_8[o]);
		}
	}
}

void AppString::ToUTF16(std::wstring& str)
{
	str.clear();
	AppUnicodeConverter uc;
	for (size_t i = 0; i < m_size; ++i)
	{
		uc.Set(m_data[i]);

		if (!uc.m_16Num)
			break;

		str.push_back(uc.m_16[0]);
		if (uc.m_16Num == 2)
			str.push_back(uc.m_16[1]);
	}
}
uint32_t AppString::_readFromFile(FILE* f)
{
	uint32_t ret = 1;
	if (f)
	{
		fseek(f, 0, SEEK_END);
		auto fsz = ftell(f);
		if (fsz)
			Reserve(fsz);
		fseek(f, 0, SEEK_SET);

		uint8_t bom[4] = { 0,0,0,0 };
		fread_s(bom, 4, 1, 4, f);
		fseek(f, 0, SEEK_SET);

		enum
		{
			type_utf8,
			type_utf16,
			type_utf32,
		};
		bool isBE = false;
		uint32_t type = type_utf8;

		if ((bom[0] == 0xEF)
			&& (bom[1] == 0xBB)
			&& (bom[2] == 0xBF))
		{
			fseek(f, 3, SEEK_SET);
			ret = 1;
		}
		else if ((bom[0] == 0x00)
			&& (bom[1] == 0x00)
			&& (bom[2] == 0xFE)
			&& (bom[3] == 0xFF))
		{
			fseek(f, 4, SEEK_SET);
			type = type_utf32;
			isBE = true;
			ret = 5;
		}
		else if ((bom[0] == 0xFF)
			&& (bom[1] == 0xFE)
			&& (bom[2] == 0x00)
			&& (bom[3] == 0x00))
		{
			fseek(f, 4, SEEK_SET);
			type = type_utf32;
			ret = 4;
		}
		else if ((bom[0] == 0xFF)
			&& (bom[1] == 0xFE))
		{
			fseek(f, 2, SEEK_SET);
			type = type_utf16;
			ret = 2;
		}
		else if ((bom[0] == 0xFE)
			&& (bom[1] == 0xFF))
		{
			fseek(f, 2, SEEK_SET);
			type = type_utf16;
			isBE = true;
			ret = 3;
		}


		union
		{
			uint32_t m_32;
			uint16_t m_16[2];
			uint8_t m_8[4];
		}
		uint_union;

		AppUnicodeConverter uc;
		while (true)
		{
			uint8_t buf[4] = { 0,0,0,0 };

			if (type == type_utf8)
			{
				long t = ftell(f);
				size_t r = fread_s(buf, 4, 1, 4, f);
				if (!r)
					break;

				uint32_t b = uc.Set(AppVec4u(buf[0], buf[1], buf[2], buf[3]));
				if (!uc.m_32)
					break;

				PushBack(uc.m_32);

				fseek(f, t + b, SEEK_SET);
			}
			else if (type == type_utf16)
			{
				long t = ftell(f);
				size_t r = fread_s(uint_union.m_8, 4, 1, 4, f);
				if (!r)
					break;


				if (isBE)
				{
					auto _0 = uint_union.m_8[0];
					auto _1 = uint_union.m_8[1];
					uint_union.m_8[0] = uint_union.m_8[3];
					uint_union.m_8[3] = _0;
					uint_union.m_8[1] = uint_union.m_8[2];
					uint_union.m_8[2] = _1;
				}

				uint32_t b = uc.Set(uint_union.m_16[0], uint_union.m_16[1]);
				if (!uc.m_32)
					break;

				PushBack(uc.m_32);

				fseek(f, t + (b * 2), SEEK_SET);
			}
			else if (type == type_utf32)
			{
				long t = ftell(f);
				size_t r = fread_s(uint_union.m_8, 4, 1, 4, f);
				if (!r)
					break;


				if (isBE)
				{
					auto _0 = uint_union.m_8[0];
					auto _1 = uint_union.m_8[1];
					uint_union.m_8[0] = uint_union.m_8[3];
					uint_union.m_8[3] = _0;
					uint_union.m_8[1] = uint_union.m_8[2];
					uint_union.m_8[2] = _1;
				}

				PushBack(uint_union.m_32);

				fseek(f, t + 4, SEEK_SET);
			}
			else
			{
				break;
			}
		}

		fclose(f);
	}
	return ret;
}

uint32_t AppString::ReadFromFile(const wchar_t* fn)
{
	FILE* f = AppFopenW(fn, L"rb");
	return _readFromFile(f);
}

uint32_t AppString::ReadFromFile(const char* fn)
{
	FILE* f = AppFopenA(fn, "rb");
	return _readFromFile(f);
}

void AppString::SaveToFileUTF8(const char* fn, bool addBOM)
{
	_saveToFileUTF8(AppFopenA(fn, "wb"), addBOM);
}

void AppString::SaveToFileUTF16(const char* fn, bool addBOM)
{
	_saveToFileUTF16(AppFopenA(fn, "wb"), addBOM);
}

void AppString::SaveToFileUTF32(const char* fn, bool addBOM)
{
	_saveToFileUTF32(AppFopenA(fn, "wb"), addBOM);
}

void AppString::SaveToFileUTF8(const wchar_t* fn, bool addBOM)
{
	_saveToFileUTF8(AppFopenW(fn, L"wb"), addBOM);
}

void AppString::SaveToFileUTF16(const wchar_t* fn, bool addBOM)
{
	_saveToFileUTF16(AppFopenW(fn, L"wb"), addBOM);
}

void AppString::SaveToFileUTF32(const wchar_t* fn, bool addBOM)
{
	_saveToFileUTF32(AppFopenW(fn, L"wb"), addBOM);
}


void AppString::_saveToFileUTF8(FILE* f, bool addBOM)
{
	if (f && m_data && m_size)
	{
		if (addBOM)
		{
			uint8_t bom[4] = { 0xEF, 0xBB, 0xBF, 0 };
			fwrite(bom, 1, 3, f);
		}

		AppUnicodeConverter uc;
		for (size_t i = 0; i < m_size; ++i)
		{
			uc.Set(m_data[i]);
			for (size_t o = 0; o < uc.m_8Num; ++o)
			{
				fwrite(&uc.m_8[o], 1, 1, f);
			}
		}
	}

	if (f)
		fclose(f);
}

void AppString::_saveToFileUTF16(FILE* f, bool addBOM)
{
	if (f && m_data && m_size)
	{
		if (addBOM)
		{
			uint8_t bom[4] = { 0xFF, 0xFE, 0, 0 };
			fwrite(bom, 1, 2, f);
		}

		AppUnicodeConverter uc;
		for (size_t i = 0; i < m_size; ++i)
		{
			uc.Set(m_data[i]);
			if (uc.m_16Num)
			{
				fwrite(&uc.m_16[0], sizeof(char16_t), 1, f);
				if (uc.m_16Num > 1)
					fwrite(&uc.m_16[1], sizeof(char16_t), 1, f);
			}
		}
	}

	if (f)
		fclose(f);
}

void AppString::_saveToFileUTF32(FILE* f, bool addBOM)
{
	if (f && m_data && m_size)
	{
		if (addBOM)
		{
			uint8_t bom[4] = { 0xFF, 0xFE, 0, 0 };
			fwrite(bom, 1, 4, f);
		}

		fwrite(m_data, sizeof(char32_t), m_size, f);
	}
	if (f)
		fclose(f);
}

AppString& AppString::operator=(const AppString& str)
{
	_reallocate(al_internal::string_wordSize);
	Clear();
	Append(str);
	return *this;
}

AppString& AppString::operator=(AppString&& str) noexcept
{
	_free();
	m_allocated = str.m_allocated;
	m_data = str.m_data;
	m_size = str.m_size;
	str.m_allocated = 0;
	str.m_data = 0;
	str.m_size = 0;
	return *this;
}

void AppString::Flip()
{
	if (!m_size)
		return;
	AppString flippedStr;
	for (size_t i = m_size - 1; i >= 0; --i)
	{
		flippedStr.Append(m_data[i]);
		if (!i)
			break;
	}
	*this = std::move(flippedStr);
}

void AppString::Insert(char32_t c, size_t where)
{
	size_t new_size = m_size + 1;
	if ((new_size + 1u) > m_allocated)
		_reallocate((new_size + 1) + (1 + (size_t)(m_size * 0.5f)));

	size_t i = m_size;
	while (i >= where)
	{
		auto next = i + 1;
		if (next < m_allocated)
			m_data[next] = m_data[i];

		if (i == 0)
			break;
		--i;
	}
	m_data[where] = c;
	m_size = new_size;
	m_data[m_size] = 0;
}

void AppString::_set_size(size_t size)
{
	m_size = size;
}
