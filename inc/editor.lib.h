#pragma once
#ifndef _EDITORLIB_H_
#define _EDITORLIB_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cmath>

typedef float float32_t;
typedef double float64_t;

#ifdef EDITORLIB_DLL
#define EDITORLIB_API _declspec(dllexport)
#else
#define EDITORLIB_API _declspec(dllimport)
#endif

#define APP_FORCE_INLINE __forceinline
#define APP_MAKEFOURCC( ch0, ch1, ch2, ch3 )\
	((uint32_t)(uint8_t)(ch0)|((uint32_t)(uint8_t)(ch1)<<8)|\
	((uint32_t)(uint8_t)(ch2)<<16)|((uint32_t)(uint8_t)(ch3)<<24))


class AppString;

extern "C"
{
	EDITORLIB_API void* AppMalloc(size_t);
	EDITORLIB_API void* AppCalloc(size_t);
	EDITORLIB_API void AppFree(void*);

	EDITORLIB_API FILE* AppFopenA(const char*, const char* mode);
	EDITORLIB_API FILE* AppFopenW(const wchar_t*, const wchar_t* mode);

	// Create new char32_t string
	// Use AppDestroyObject for destroying.
	EDITORLIB_API AppString* AppCreateString();
}

//AppPolygonImpl* newPolygon = AppCreate<AppPolygonImpl>();
template<typename Type, typename... Args>
Type* AppCreate(Args&&... args)
{
	Type* o = (Type*)AppMalloc(sizeof(Type));
	if (o)
		new(o)Type(std::forward<Args>(args)...);
	return o;
}

template<typename Type>
void AppDestroyObject(Type* p)
{
	if (p)
	{
		p->~Type();
		AppFree(p);
	}
}



template<typename T1, typename T2>
struct AppPair
{
	AppPair() {}
	AppPair(const T1& _t1, const T2& _t2) :m_first(_t1), m_second(_t2) {}
	T1 m_first;
	T2 m_second;
};


#include "AppBaseObject.h"
#include "AppColor.h"
#include "AppMath.h"
#include "AppMesh.h"

#include "AppRay.h"
#include "AppAabb.h"
#include "AppUnicodeConverter.h"
#include "AppString.h"
#include "AppList.h"
#include "AppArray.h"

#endif
