/*
 * <�ۼ� ��¥>
 * 2017-12-05
 *
 * <���� ����>
 * ���� ���Ǵ� ǥ�� ��� �����Դϴ�.
 */

#ifndef COMMON_STDHEDAER_H_
#define COMMON_STDHEDAER_H_

// C ��� �����Դϴ�.
#include <tchar.h>

// ����� ���� ��� �����Դϴ�.
#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#ifdef DEBUG_NEW
		#undef DEBUG_NEW
		#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif
#endif

// Win32 API ��� �����Դϴ�.
#ifdef WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#undef  WIN32_LEAN_AND_MEAN
#endif

// C++ ��� �����Դϴ�.
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>

#endif