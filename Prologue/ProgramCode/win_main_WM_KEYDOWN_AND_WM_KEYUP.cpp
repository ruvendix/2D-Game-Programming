/*
 * <���� ��¥>
 * 2017-12-03
 *
 * <���� ����>
 * Win32 API�� �̿��� WM_KEYDOWN�� WM_KEYUP Ȱ�� ���α׷��Դϴ�.
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
	// Ű���� ������ü���� ��ĵ�ڵ��� �����ȣ���� �ٸ���
	// ������ �����ϱ� ���� ������ OS�� ����Ű��� �� �̿��ؼ� ������

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

// Ű���� ���� �Ǵ�
// 0x8000�� Ű�� ���� ����
// 0x8001�� Ű�� �� ����

void KeyDown(HWND hWnd, WPARAM wParam)
{
	// ����Ʈ�� ��Ʈ���� ��ġ�� ����Ű�θ� �Ǵ���
	// �����δ� �ٸ� ����� �̿��ؼ� �Ǵ���
	// ����� ����Ű�θ� �Ǵ��ؼ� ������ ����

	switch (wParam)
	{
	case VK_SPACE:
		MessageBox(hWnd, _T("�����̽� ����"), _T("�޽��� �ڽ�"), MB_OK);
		break;
	case VK_CONTROL:
		if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			MessageBox(hWnd, _T("���� ��Ʈ�� ����"), _T("�޽��� �ڽ�"), MB_OK);
		}
		else if (GetKeyState(VK_RCONTROL) & 0x8002)
		{
			MessageBox(hWnd, _T("������ ��Ʈ�� ����"), _T("�޽��� �ڽ�"), MB_OK);
		}
		break;
	}
}

void KeyUp(HWND hWnd, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_RETURN:
		MessageBox(hWnd, _T("���� ��"), _T("�޽��� �ڽ�"), MB_OK);
		break;
	case VK_SHIFT:
		if (GetKeyState(VK_LSHIFT) & 0x8001)
		{
			MessageBox(hWnd, _T("���� ����Ʈ ��"), _T("�޽��� �ڽ�"), MB_OK);
		}
		else if (GetKeyState(VK_RSHIFT) & 0x8001)
		{
			MessageBox(hWnd, _T("������ ����Ʈ ��"), _T("�޽��� �ڽ�"), MB_OK);
		}
		break;
	}
}