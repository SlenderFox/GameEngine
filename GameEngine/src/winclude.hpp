#pragma once

#if defined(WIN32) || defined(_WIN32)

	#define WIN32_LEAN_AND_MEAN
	#ifndef UNICODE
		#define UNICODE
	#endif
	#ifndef STRICT
		#define STRICT
	#endif

	#ifdef NOMINMAX
		#include <Windows.h>
	#else
		#define NOMINMAX
		#include <Windows.h>
	#endif
#endif
