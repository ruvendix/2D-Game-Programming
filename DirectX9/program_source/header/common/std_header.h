/*
 * <작성 날짜>
 * 2017-12-05
 *
 * <파일 설명>
 * 자주 사용되는 표준 헤더 파일입니다.
 */

#ifndef COMMON_STDHEDAER_H_
#define COMMON_STDHEDAER_H_

// C 헤더 파일입니다.
#include <tchar.h>

// 디버그 전용 헤더 파일입니다.
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#ifdef DEBUG_NEW
		#undef DEBUG_NEW
		#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif
#endif

// Win32 API 헤더 파일입니다.
#ifdef WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#undef  WIN32_LEAN_AND_MEAN
#endif

// C++ 헤더 파일입니다.
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>

#endif