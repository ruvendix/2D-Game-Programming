/*
 * <�ۼ� ��¥>
 * 2017-12-05
 *
 * <���� ����>
 * Win32 API�� ���� �Լ��Դϴ�.
 */

#pragma warning(disable:4302)

#include "common/std_header.h"
#include "common/macro.h"

#include "DirectX9/DirectX9Interface.h"

LPCTSTR WINDOW_CLASS_NAME  = _T("DirectX9 Window");
LPCTSTR GAME_TITLE         = _T("DirectX9 Test");
LPCTSTR GAME_INSTANCE_NAME = _T("DirectX9 Instance");

HWND g_hWnd;

HRESULT CheckMultipleInstance();
HRESULT InitInstance(HINSTANCE hInstance);
HRESULT CreateGameWindow(HINSTANCE hInstance, INT32 cmdShow);
LRESULT CALLBACK WindowProcedure(HWND hInstance, UINT msg, WPARAM wParam, LPARAM lParam);

INT32 APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR strCmdLine, INT32 cmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	cmdShow = SW_NORMAL;

	// ���� �ν��Ͻ� üũ
	if (FAILED(CheckMultipleInstance()))
	{
		return 0;
	}

	// �޸� �� Ʈ���̼� Ȱ��ȭ
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// ������ Ŭ���� ���� �� ���
	if (FAILED(InitInstance(hInstance)))
	{
		return 0;
	}

	// ���α׷� â ���� �� ���
	if (FAILED(CreateGameWindow(hInstance, cmdShow)))
	{
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	GET_SINGLE(d3d9::DirectX9Interface).InitDirectX9();

	// �޽��� ����
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
		else
		{
			// ���� ��ƾ
			
		}
	}

	GET_SINGLE(d3d9::DirectX9Interface).ReleaseDirectX9();
	return static_cast<INT32>(msg.message);
}

HRESULT CheckMultipleInstance()
{
	HANDLE hInstanceMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, GAME_INSTANCE_NAME);

	if (hInstanceMutex == nullptr) // ������ ���ؽ��� ���ٸ�
	{
		hInstanceMutex = CreateMutex(nullptr, TRUE, GAME_INSTANCE_NAME);
	}
	else
	{
		MSGBOX_OK("�ߺ� ���� �Ұ�!");
		return E_FAIL;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MSGBOX_OK("�ߺ� ���� �Ұ�!");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT InitInstance(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(wcex);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WindowProcedure;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor       = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName  = nullptr;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wcex) == 0)
	{
		MSGBOX_OK("WNDCLASSEX ��� ����!");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CreateGameWindow(HINSTANCE hInstance, INT32 cmdShow)
{
	// ��üȭ������ �ƴ����� ���� �Ǵ�
	// �ػ󵵳� �׷� �� Get���� �����ͼ� �Ǵ�
	// ���� �ػ󵵴� �� �����Ƿ� MoveWindow() Ȱ��

	HWND hWnd = g_hWnd = CreateWindow(WINDOW_CLASS_NAME, GAME_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == nullptr)
	{
		MSGBOX_OK("���α׷� â ���� ����!");
		return E_FAIL;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);

	INT32 screenWidth  = 800 + (800 - rect.right);
	INT32 screenHeight = 600 + (600 - rect.bottom);
	MoveWindow(hWnd, 0, 0, screenWidth, screenHeight, TRUE);

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	return S_OK;
}

LRESULT CALLBACK WindowProcedure(HWND hInstance, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hInstance, msg, wParam, lParam);
}