#pragma once
#ifndef _PLUGINSTR_H_
#define _PLUGINSTR_H_

class PLUGIN_API PluginString
{
	size_t m_size = 0;
	size_t m_allocated = 0;
	char32_t* m_data = 0;

	void _reallocate(size_t new_allocated);
	void _free();
	uint32_t _readFromFile(FILE*);
	void _saveToFileUTF8(FILE*, bool addBOM = true);
	void _saveToFileUTF16(FILE*, bool addBOM = true);
	void _saveToFileUTF32(FILE*, bool addBOM = true);
public:
	PluginString();
	PluginString(const char*);
	PluginString(const wchar_t*);
	PluginString(const char8_t*);
	PluginString(const char16_t*);
	PluginString(const char32_t*);
	PluginString(const PluginString&);
	PluginString(PluginString&&) noexcept;
	~PluginString();

	void Assign(const char*);
	void Assign(const wchar_t*);
	void Assign(const char8_t*);
	void Assign(const char16_t*);
	void Assign(const char32_t*);
	void Assign(const PluginString&);

	size_t Size();
	size_t Capacity();
	void Reserve(size_t);
	void Clear();
	void PopBack();
	void PushBack(char32_t);
	char32_t* Data();
	const char32_t* c_str();
	void Flip();

	void Insert(char32_t c, size_t where);

	void Append(const char*);
	void Append(const char8_t*);
	void Append(const wchar_t*);
	void Append(const char16_t*);
	void Append(const char32_t*);
	void Append(const char32_t*, size_t size);
	void Append(const PluginString&);
	void Append(char32_t);
	void Append(uint32_t);
	void Append(uint64_t);
	void Append(int32_t);
	void Append(int64_t);
	void Append(float32_t);
	void AppendFloat(float32_t);
	void Append(float64_t);

	int32_t ToInt();
	uint32_t ToUint();
	float32_t ToFloat();
	float64_t ToFloat64();
	void ToUTF8(std::string&);
	void ToUTF16(std::wstring&);

	// return is encoding type
	// 1 - utf8
	// 2 - utf16
	// 3 - utf16 big endian
	// 4 - utf32
	// 5 - utf32 big endian
	uint32_t ReadFromFile(const char*);
	uint32_t ReadFromFile(const wchar_t*);

	void SaveToFileUTF8(const char*, bool addBOM = true);
	void SaveToFileUTF16(const char*, bool addBOM = true);
	void SaveToFileUTF32(const char*, bool addBOM = true);
	void SaveToFileUTF8(const wchar_t*, bool addBOM = true);
	void SaveToFileUTF16(const wchar_t*, bool addBOM = true);
	void SaveToFileUTF32(const wchar_t*, bool addBOM = true);

	PluginString& operator=(const PluginString&);
	PluginString& operator=(PluginString&&) noexcept;

	void operator+=(const char* str) { Append(str); }
	void operator+=(const char8_t* str) { Append(str); }
	void operator+=(const char16_t* str) { Append(str); }
	void operator+=(const char32_t* str) { Append(str); }

	//m_size = size;
	void _set_size(size_t size);
};

#endif
