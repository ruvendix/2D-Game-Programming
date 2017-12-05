/*
 * <작성 날짜>
 * 2017-12-05
 *
 * <파일 설명>
 * Win32 API의 메인 함수입니다.
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

	// 다중 인스턴스 체크
	if (FAILED(CheckMultipleInstance()))
	{
		return 0;
	}

	// 메모리 릭 트레이서 활성화
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// 윈도우 클래스 생성 및 등록
	if (FAILED(InitInstance(hInstance)))
	{
		return 0;
	}

	// 프로그램 창 생성 및 출력
	if (FAILED(CreateGameWindow(hInstance, cmdShow)))
	{
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	GET_SINGLE(d3d9::DirectX9Interface).InitDirectX9();

	// 메시지 루프
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
			// 게임 루틴
			
		}
	}

	GET_SINGLE(d3d9::DirectX9Interface).ReleaseDirectX9();
	return static_cast<INT32>(msg.message);
}

HRESULT CheckMultipleInstance()
{
	HANDLE hInstanceMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, GAME_INSTANCE_NAME);

	if (hInstanceMutex == nullptr) // 생성된 뮤텍스가 없다면
	{
		hInstanceMutex = CreateMutex(nullptr, TRUE, GAME_INSTANCE_NAME);
	}
	else
	{
		MSGBOX_OK("중복 실행 불가!");
		return E_FAIL;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MSGBOX_OK("중복 실행 불가!");
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
		MSGBOX_OK("WNDCLASSEX 등록 실패!");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CreateGameWindow(HINSTANCE hInstance, INT32 cmdShow)
{
	// 전체화면인지 아닌지에 따라 판단
	// 해상도나 그런 건 Get으로 가져와서 판단
	// 실제 해상도는 더 작으므로 MoveWindow() 활용

	HWND hWnd = g_hWnd = CreateWindow(WINDOW_CLASS_NAME, GAME_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == nullptr)
	{
		MSGBOX_OK("프로그램 창 생성 실패!");
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