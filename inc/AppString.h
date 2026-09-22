#pragma once
#ifndef _APPSTR_H_
#define _APPSTR_H_

class AppString : public AppBaseObject
{
public:
	virtual void Assign(const char*) = 0;
	virtual void Assign(const wchar_t*) = 0;
	virtual void Assign(const char8_t*) = 0;
	virtual void Assign(const char16_t*) = 0;
	virtual void Assign(const char32_t*) = 0;
	virtual void Assign(const AppString&) = 0;

	virtual size_t Size() = 0;
	virtual size_t Capacity() = 0;
	virtual void Reserve(size_t) = 0;
	virtual void Clear() = 0;
	virtual void PopBack() = 0;
	virtual void PushBack(char32_t) = 0;
	virtual char32_t* Data() = 0;
	virtual const char32_t* c_str() = 0;
	virtual void Flip() = 0;

	virtual void Insert(char32_t c, size_t where) = 0;

	virtual void Append(const char*) = 0;
	virtual void Append(const char8_t*) = 0;
	virtual void Append(const wchar_t*) = 0;
	virtual void Append(const char16_t*) = 0;
	virtual void Append(const char32_t*) = 0;
	virtual void Append(const char32_t*, size_t size) = 0;
	virtual void Append(const AppString&) = 0;
	virtual void Append(char32_t) = 0;
	virtual void Append(uint32_t) = 0;
	virtual void Append(uint64_t) = 0;
	virtual void Append(int32_t) = 0;
	virtual void Append(int64_t) = 0;
	virtual void Append(float32_t) = 0;
	virtual void AppendFloat(float32_t) = 0;
	virtual void Append(float64_t) = 0;

	virtual int32_t ToInt() = 0;
	virtual uint32_t ToUint() = 0;
	virtual float32_t ToFloat() = 0;
	virtual float64_t ToFloat64() = 0;
	virtual void ToUTF8(std::string&) = 0;
	virtual void ToUTF16(std::wstring&) = 0;

	// return is encoding type
	// 1 - utf8
	// 2 - utf16
	// 3 - utf16 big endian
	// 4 - utf32
	// 5 - utf32 big endian
	virtual uint32_t ReadFromFile(const char*) = 0;
	virtual uint32_t ReadFromFile(const wchar_t*) = 0;

	virtual void SaveToFileUTF8(const char*, bool addBOM = true) = 0;
	virtual void SaveToFileUTF16(const char*, bool addBOM = true) = 0;
	virtual void SaveToFileUTF32(const char*, bool addBOM = true) = 0;
	virtual void SaveToFileUTF8(const wchar_t*, bool addBOM = true) = 0;
	virtual void SaveToFileUTF16(const wchar_t*, bool addBOM = true) = 0;
	virtual void SaveToFileUTF32(const wchar_t*, bool addBOM = true) = 0;

	//m_size = size;
	virtual void _set_size(size_t size) = 0;
};

#endif
