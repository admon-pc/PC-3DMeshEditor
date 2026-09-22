#pragma once


class AppUnicodeConverterImpl : public AppUnicodeConverter
{
	void _find16From32();
	void _find8From32();

public:
	AppUnicodeConverterImpl();
	virtual ~AppUnicodeConverterImpl();

	virtual void Set(char32_t c) override;
	virtual void Set(char16_t c) override;
	virtual void Set(char8_t c) override;
	virtual void Set(char c) override;
	virtual void Set(wchar_t c) override;

	virtual uint32_t Set(const AppVec4u& c) override;
	virtual uint32_t Set(char16_t c1, char16_t c2) override;

	virtual void wchar_to_char(const wchar_t* str, size_t sz, std::string* out) override;
	virtual void char_to_wchar(const char* str, size_t sz, std::wstring* out) override;

	uint32_t m_32 = 0;
	uint16_t m_16[2] = { 0, 0 };
	uint8_t m_8[4] = { 0, 0, 0, 0 };

	uint32_t m_16Num = 0;
	uint32_t m_8Num = 0;
};

class AppStringImpl : public AppString
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
	AppStringImpl();
	AppStringImpl(const char*);
	AppStringImpl(const wchar_t*);
	AppStringImpl(const char8_t*);
	AppStringImpl(const char16_t*);
	AppStringImpl(const char32_t*);
	AppStringImpl(const AppStringImpl&);
	AppStringImpl(AppStringImpl&&) noexcept;
	virtual ~AppStringImpl();

	virtual void Assign(const char*) override;
	virtual void Assign(const wchar_t*) override;
	virtual void Assign(const char8_t*) override;
	virtual void Assign(const char16_t*) override;
	virtual void Assign(const char32_t*) override;
	virtual void Assign(AppString*) override;

	virtual size_t Size() override;
	virtual size_t Capacity() override;
	virtual void Reserve(size_t) override;
	virtual void Clear() override;
	virtual void PopBack() override;
	virtual void PushBack(char32_t) override;
	virtual char32_t* Data() override;
	virtual const char32_t* c_str() override;
	virtual void Flip() override;

	virtual void Insert(char32_t c, size_t where) override;

	virtual void Append(const char*) override;
	virtual void Append(const char8_t*) override;
	virtual void Append(const wchar_t*) override;
	virtual void Append(const char16_t*) override;
	virtual void Append(const char32_t*) override;
	virtual void Append(const char32_t*, size_t size) override;
	virtual void Append(AppString*) override;
	void Append(const AppStringImpl&);
	virtual void Append(char32_t) override;
	virtual void Append(uint32_t) override;
	virtual void Append(uint64_t) override;
	virtual void Append(int32_t) override;
	virtual void Append(int64_t) override;
	virtual void Append(float32_t) override;
	virtual void AppendFloat(float32_t) override;
	virtual void Append(float64_t) override;

	virtual int32_t ToInt() override;
	virtual uint32_t ToUint() override;
	virtual float32_t ToFloat() override;
	virtual float64_t ToFloat64() override;
	virtual void ToUTF8(std::string&) override;
	virtual void ToUTF16(std::wstring&) override;

	// return is encoding type
	// 1 - utf8
	// 2 - utf16
	// 3 - utf16 big endian
	// 4 - utf32
	// 5 - utf32 big endian
	virtual uint32_t ReadFromFile(const char*) override;
	virtual uint32_t ReadFromFile(const wchar_t*) override;

	virtual void SaveToFileUTF8(const char*, bool addBOM = true) override;
	virtual void SaveToFileUTF16(const char*, bool addBOM = true) override;
	virtual void SaveToFileUTF32(const char*, bool addBOM = true) override;
	virtual void SaveToFileUTF8(const wchar_t*, bool addBOM = true) override;
	virtual void SaveToFileUTF16(const wchar_t*, bool addBOM = true) override;
	virtual void SaveToFileUTF32(const wchar_t*, bool addBOM = true) override;

	AppStringImpl& operator=(const AppStringImpl&);
	AppStringImpl& operator=(AppStringImpl&&) noexcept;

	void operator+=(const char* str) { Append(str); }
	void operator+=(const char8_t* str) { Append(str); }
	void operator+=(const char16_t* str) { Append(str); }
	void operator+=(const char32_t* str) { Append(str); }

	//m_size = size;
	virtual void _set_size(size_t size) override;
};
