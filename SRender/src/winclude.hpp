#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

	#define WIN32_LEAN_AND_MEAN

	#ifndef WIN32
		#define WIN32
	#endif
	#ifndef _WIN32
		#define _WIN32
	#endif
	#ifndef __WIN32__
		#define __WIN32__
	#endif
	#ifndef __WINDOWS__
		#define __WINDOWS__
	#endif
	#ifndef UNICODE
		#define UNICODE
	#endif
	#ifndef STRICT
		#define STRICT
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <Windows.h>
#endif
