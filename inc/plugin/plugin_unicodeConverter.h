#pragma once
#ifndef _PLUGINUC_H_
#define _PLUGINUC_H_

class PLUGIN_API PluginUnicodeConverter
{
	void _find16From32();
	void _find8From32();

public:
	PluginUnicodeConverter();
	~PluginUnicodeConverter();

	void Set(char32_t c);
	void Set(char16_t c);
	void Set(char8_t c);
	void Set(char c);
	void Set(wchar_t c);

	uint32_t Set(const plVec4u& c);
	uint32_t Set(char16_t c1, char16_t c2);

	void wchar_to_char(const wchar_t* str, size_t sz, std::string* out);
	void char_to_wchar(const char* str, size_t sz, std::wstring* out);

	uint32_t m_32 = 0;
	uint16_t m_16[2] = { 0, 0 };
	uint8_t m_8[4] = { 0, 0, 0, 0 };

	uint32_t m_16Num = 0;
	uint32_t m_8Num = 0;
};


#endif
