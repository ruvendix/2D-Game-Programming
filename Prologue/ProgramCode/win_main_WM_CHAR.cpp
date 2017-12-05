/*
 * <생성 날짜>
 * 2017-12-03
 *
 * <파일 내용>
 * Win32 API를 이용한 WM_CHAR 활용 프로그램입니다.
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

static TCHAR text = 'A';

static const INT32 INIT_INSTANCE_FAILURE = 0;

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow);
bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance);
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam);
void InputText(HWND hWnd, WPARAM wParam);
void PrintText(HWND hWnd, HDC hdc);

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

	HWND hWnd = CreateWindow(_T("WinMain"), _T("WM_CHAR Test"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
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
	pWCX->cbSize = sizeof(*pWCX);
	pWCX->style = CS_HREDRAW | CS_VREDRAW;
	pWCX->lpfnWndProc = WinProc;
	pWCX->cbClsExtra = 0;
	pWCX->cbWndExtra = 0;
	pWCX->hInstance = hInstance;
	pWCX->hIcon = nullptr;
	pWCX->hCursor = LoadCursor(nullptr, IDC_ARROW);
	pWCX->hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	pWCX->lpszMenuName = nullptr;
	pWCX->lpszClassName = _T("WinMain");
	pWCX->hIconSm = nullptr;

	return true;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		InputText(hWnd, wParam);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			PrintText(hWnd, hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam); // Default Window Procedure
	}

	return 0;
}

void InputText(HWND hWnd, WPARAM wParam)
{
	switch (wParam)
	{
	case 0x08: // 백스페이스
	case 0x09: // 탭
	case 0x0A: // 개행
	case 0x0D: // 캐리지 리턴
	case 0x1B: // ESC
		MessageBox(hWnd, _T("출력 불가능한 문자!"), _T("메시지 박스"), MB_OK);
		break;
	default:
		text = static_cast<TCHAR>(wParam);   // 4바이트 -> 1바이트 또는 2바이트 변환
		InvalidateRect(hWnd, nullptr, true); // 강제로 WM_PAINT 메시지 발생시킴, nullptr은 디폴트 설정
		break;
	}
}

void PrintText(HWND hWnd, HDC hdc)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect); // 프로그램의 실제 출력 영역을 가져옴
	TextOut(hdc, clientRect.right / 2, clientRect.bottom / 2, &text, 1);
}