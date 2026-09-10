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

extern "C"
{
	PLUGIN_API void* PluginMalloc(size_t);
	PLUGIN_API void* PluginCalloc(size_t);
	PLUGIN_API void PluginFree(void*);
	PLUGIN_API FILE* PluginFopenA(const char*, const char* mode);
	PLUGIN_API FILE* PluginFopenW(const wchar_t*, const wchar_t* mode);
}

#include "plugin/plugin_vec.h"
#include "plugin/plugin_unicodeConverter.h"
#include "plugin/plugin_string.h"
#include "plugin/plugin_list.h"
#include "plugin/plugin_array.h"

#endif
