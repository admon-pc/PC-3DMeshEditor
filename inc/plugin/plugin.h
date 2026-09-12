#pragma once
#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cmath>

typedef float float32_t;
typedef double float64_t;

#ifdef PLUGIN_DLL
#define PLUGIN_API _declspec(dllexport)
#else
#define PLUGIN_API _declspec(dllimport)
#endif

#define PLUGIN_FORCE_INLINE __forceinline
#define PLUGIN_MAKEFOURCC( ch0, ch1, ch2, ch3 )\
	((uint32_t)(uint8_t)(ch0)|((uint32_t)(uint8_t)(ch1)<<8)|\
	((uint32_t)(uint8_t)(ch2)<<16)|((uint32_t)(uint8_t)(ch3)<<24))

extern "C"
{
	PLUGIN_API void* PluginMalloc(size_t);
	PLUGIN_API void* PluginCalloc(size_t);
	PLUGIN_API void PluginFree(void*);
	PLUGIN_API FILE* PluginFopenA(const char*, const char* mode);
	PLUGIN_API FILE* PluginFopenW(const wchar_t*, const wchar_t* mode);
}


#include "plugin/plugin_color.h"
#include "plugin/plugin_math.h"


#include "plugin/plugin_ray.h"
#include "plugin/plugin_aabb.h"
#include "plugin/plugin_unicodeConverter.h"
#include "plugin/plugin_string.h"
#include "plugin/plugin_list.h"
#include "plugin/plugin_array.h"

#endif
