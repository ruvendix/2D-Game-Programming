/*
 * <생성 날짜>
 * 2017-12-03
 *
 * <파일 내용>
 * Win32 API를 이용한 WM_KEYDOWN과 WM_KEYUP 활용 프로그램입니다.
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef  WIN32_LEAN_AND_MEAN
#else
#include <windows.h>
#endif

#include <cstdlib>

#include <tchar.h>

static INT32 SCREEN_WIDTH  = 800;
static INT32 SCREEN_HEIGHT = 600;

static const INT32 INIT_INSTANCE_FAILURE = 0;

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow);
bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance);
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam);
void KeyDown(HWND hWnd, WPARAM wParam);
void KeyUp(HWND hWnd, WPARAM wParam);

INT32 APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT32 cmdShow)
{
	cmdShow = SW_NORMAL;
	UNREFERENCED_PARAMETER(hPrevInstance);

	if (InitProgramInstance(hInstance, cmdShow) == false)
	{
		return INIT_INSTANCE_FAILURE;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	bool bDrive = true;
	while (bDrive)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDrive = false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<INT32>(msg.wParam);
}

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow)
{
	WNDCLASSEX wcx;
	ZeroMemory(&wcx, sizeof(wcx));
	InitWindowClass(&wcx, hInstance);

	if (RegisterClassEx(&wcx) == 0)
	{
		return false;
	}

	HWND hWnd = CreateWindow(_T("WinMain"), _T("Keyboard Test"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == nullptr)
	{
		return false;
	}

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	return true;
}

bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance)
{
	pWCX->cbSize         = sizeof(*pWCX);
	pWCX->style          = CS_HREDRAW | CS_VREDRAW;
	pWCX->lpfnWndProc    = WinProc;
	pWCX->cbClsExtra     = 0;
	pWCX->cbWndExtra     = 0;
	pWCX->hInstance      = hInstance;
	pWCX->hIcon          = nullptr;
	pWCX->hCursor        = LoadCursor(nullptr, IDC_ARROW);
	pWCX->hbrBackground  = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	pWCX->lpszMenuName   = nullptr;
	pWCX->lpszClassName  = _T("WinMain");
	pWCX->hIconSm        = nullptr;

	return true;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
{
	// 키보드 제조업체마다 스캔코드라는 전기신호값이 다른데
	// 이점을 보완하기 위해 윈도우 OS가 가상키라는 걸 이용해서 통일함

	switch (msg)
	{
	case WM_KEYDOWN:
		KeyDown(hWnd, wParam);
		break;
	case WM_KEYUP:
		KeyUp(hWnd, wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam); // Default Window Procedure
	}

	return 0;
}

// 키보드 상태 판단
// 0x8000은 키가 눌린 상태
// 0x8001은 키가 뗀 상태

void KeyDown(HWND hWnd, WPARAM wParam)
{
	// 쉬프트와 컨트롤의 위치는 가상키로만 판단함
	// 실제로는 다른 방법을 이용해서 판단함
	// 현재는 가상키로만 판단해서 오류가 있음

	switch (wParam)
	{
	case VK_SPACE:
		MessageBox(hWnd, _T("스페이스 누름"), _T("메시지 박스"), MB_OK);
		break;
	case VK_CONTROL:
		if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			MessageBox(hWnd, _T("왼쪽 컨트롤 누름"), _T("메시지 박스"), MB_OK);
		}
		else if (GetKeyState(VK_RCONTROL) & 0x8002)
		{
			MessageBox(hWnd, _T("오른쪽 컨트롤 누름"), _T("메시지 박스"), MB_OK);
		}
		break;
	}
}

void KeyUp(HWND hWnd, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_RETURN:
		MessageBox(hWnd, _T("엔터 뗌"), _T("메시지 박스"), MB_OK);
		break;
	case VK_SHIFT:
		if (GetKeyState(VK_LSHIFT) & 0x8001)
		{
			MessageBox(hWnd, _T("왼쪽 쉬프트 뗌"), _T("메시지 박스"), MB_OK);
		}
		else if (GetKeyState(VK_RSHIFT) & 0x8001)
		{
			MessageBox(hWnd, _T("오른쪽 쉬프트 뗌"), _T("메시지 박스"), MB_OK);
		}
		break;
	}
}