#pragma once
#ifndef _APPUC_H_
#define _APPUC_H_

class AppUnicodeConverter : public AppBaseObject
{
public:
	virtual void Set(char32_t c) = 0;
	virtual void Set(char16_t c) = 0;
	virtual void Set(char8_t c) = 0;
	virtual void Set(char c) = 0;
	virtual void Set(wchar_t c) = 0;

	virtual uint32_t Set(const AppVec4u& c) = 0;
	virtual uint32_t Set(char16_t c1, char16_t c2) = 0;

	virtual void wchar_to_char(const wchar_t* str, size_t sz, std::string* out) = 0;
	virtual void char_to_wchar(const char* str, size_t sz, std::wstring* out) = 0;
};


#endif
