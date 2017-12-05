/*
 * <���� ��¥>
 * 2017-12-03
 *
 * <���� ����>
 * Win32 API�� �̿��� Hello World ���α׷��Դϴ�.
 * ���� �ν��Ͻ��� �����Ǿ��ֽ��ϴ�. ������ �̹� ��Ŀ���� �˰� �ִ� ����Դϴ�.
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

HWND g_hWnd;

bool CheckMultipleProgramInstance(HANDLE hInstance);
bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow);
bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance);
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam);

INT32 APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT32 cmdShow)
{
	// ���� �ν��Ͻ����� Ȯ���մϴ�.
	HANDLE hUniqueInstance = nullptr;
	if (CheckMultipleProgramInstance(hUniqueInstance) == true)
	{
		MessageBox(nullptr, _T("�ߺ� ������ �Ұ����մϴ�!"), _T("����"), MB_OK);
		return false;
	}

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

	ReleaseMutex(hUniqueInstance);
	CloseHandle(hUniqueInstance);

	return static_cast<INT32>(msg.wParam);
}

bool CheckMultipleProgramInstance(HANDLE hInstance)
{
	// nullptr�� �����ϸ� ����Ʈ ��ť��Ƽ�� �۵��մϴ�.
	HANDLE hUniqueInstance = CreateMutex(nullptr, true, _T("This is Unique Instance"));

	// MessageId   : ERROR_ALREADY_EXISTS
	// MessageText : Cannot create a file when that file already exists.
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return true;
	}

	return false;
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

	HWND hWnd = CreateWindow(_T("WinMain"), _T("DirectX9 Init"),
		        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
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
	pWCX->cbSize        = sizeof(*pWCX);
	pWCX->style         = CS_HREDRAW | CS_VREDRAW;
	pWCX->lpfnWndProc   = WinProc;
	pWCX->cbClsExtra    = 0;
	pWCX->cbWndExtra    = 0;
	pWCX->hInstance     = hInstance;
	pWCX->hIcon         = nullptr;
	pWCX->hCursor       = LoadCursor(nullptr, IDC_ARROW);
	pWCX->hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	pWCX->lpszMenuName  = nullptr;
	pWCX->lpszClassName = _T("WinMain");
	pWCX->hIconSm       = nullptr;

	return true;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}