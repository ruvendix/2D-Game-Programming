/*
 * <���� ��¥>
 * 2017-12-02
 *
 * <���� ����>
 * Win32 API�� �̿��� Hello World ���α׷��Դϴ�.
 */

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#undef  WIN32_LEAN_AND_MEAN
#else
	#include <windows.h>
#endif

#include <cstdlib>

// ��Ƽ����Ʈ ���� ���հ� �����ڵ� ���� ���� �� �����ϰ� �ϱ� ���� �������
#include <tchar.h>

static INT32 SCREEN_WIDTH  = 800;
static INT32 SCREEN_HEIGHT = 600;

static const INT32 INIT_INSTANCE_FAILURE = 0;

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow);
bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance);
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam);

// HINSTANCE(Handle Instance) => void ������(��� ������ ���� �� �ִ� ������), �ַ� ���ξ�� h ���
// LPSTR(Long Pointer String) => char ������(���ڿ��� �ǹ���), �ַ� ���ξ�� lp ���(str �Ǵ� sz��� �ص� �������)
// LPCSTR(Long Pointer Const String) => LPSTR�� const ����
// LPWSTR(Long Pointer Wide String)  => LPSTR�� wchar ����
// LPCWSTR(Long Pointer Const Wide String) => LPCSTR�� wchar ����
// LPTSTR(Long Pointer Transformation String)        => LPSTR�� LPWSTR�� �ϳ��� ����(TCHAR)
// LPCTSTR(Long Pointer Const Transformation String) => LPCSTR�� LPCWSTR�� �ϳ��� ����(const TCHAR)
// LRESULT(Long Result) => long�� �ǹ���
//
// hInstance     => ���α׷� ������(� ���α׷��� ���� ������ �Ǵ��� �� ���)
// hPrevInstance => �׻� nullptr, ���� ���� ������ OS���� ȣȯ���� ���� ����(16��Ʈ ������ ����...)
// lpCmdLine     => ���α׷��� ����� �� ���޵Ǵ� ����(argv��� �����ϸ� ��)
// cmdShow       => ���α׷� â�� ��� ������ ������ ����(�Ϲ�, �ּ�ȭ, �ִ�ȭ ��)
//
// WM_QUIT �޽����� ���� ������ ������ ������ �Լ�
// ���α׷� â�� �����ַ��� WNDCLASSEX�� �����ϰ� ����ؾ���(���� Ŭ������ C����̹Ƿ� ����ü��)
// _tWinMain()�� ��Ƽ����Ʈ�� �����ڵ�Ŀ� ���� WinMain()�� wWinMain()�� �ϳ��� ������
// TEXT()�� _T()�� ���� �ǵ� TEXT()�� Win32 API���� ���� ��, _T()�� MFC���� ���� �ǵ� ����� ������
INT32 APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT32 cmdShow)
{
	// cmdShow�� ����
	//
	// SW_HIDE             0
	// SW_SHOWNORMAL       1
	// SW_NORMAL           1
	// SW_SHOWMINIMIZED    2
	// SW_SHOWMAXIMIZED    3
	// SW_MAXIMIZE         3
	// SW_SHOWNOACTIVATE   4
	// SW_SHOW             5
	// SW_MINIMIZE         6
	// SW_SHOWMINNOACTIVE  7
	// SW_SHOWNA           8
	// SW_RESTORE          9
	// SW_SHOWDEFAULT      10
	// SW_FORCEMINIMIZE    11
	// SW_MAX              11
	// 
	// SW_HIDE(��׶��� ���μ���)
	// SW_NORMAL(���׶��� ���μ���, �Ϲ�)
	// SW_MAXIMIZE(�ִ�ȭ)
	// SW_MINIMIZE(�ּ�ȭ)
	// �� ������ �˸��

	cmdShow = SW_NORMAL;

	// �� ���� �Ű�������� ǥ����
	UNREFERENCED_PARAMETER(hPrevInstance);

	// WNDCLASSEX ���� �� ��ϰ� ���α׷� â ���� �� ���
	if (InitProgramInstance(hInstance, cmdShow) == false)
	{
		return INIT_INSTANCE_FAILURE;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// �޽��� ����
	bool bDrive = true;
	while (bDrive)
	{
		// GetMessage() �Ǵ� PeekMessage()�� HWND�� nullptr�� �����ϸ�
		// ���� �����带 ������ ���� �����忡 ���� ��� �޽����� ó����
		// HWND�� -1�� �����ϸ� ���� �������� �޽����� ó����(PostMessage() �Ǵ� PostThreadMessage())
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDrive = false;
			}

			TranslateMessage(&msg); // ���� Ű �޽����� ���� �޽����� �ٲ���
			DispatchMessage(&msg);  // ������ ���ν������� �޽����� ������
		}
	}

	// main()���� �޸� WinMain()�� �޽������� ��ȯ�ؾ���!
	// �׷��� ���� ���μ����� ���� �ʰ� ���� �����.
	return static_cast<INT32>(msg.wParam);
}

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow)
{
	WNDCLASSEX wcx;
	ZeroMemory(&wcx, sizeof(wcx));
	InitWindowClass(&wcx, hInstance);

	// ������ OS�� ������ WNDCLASSEX�� ����ϴ� �Լ�
	// WNDCLASSEX ��Ͽ� �����ϸ� 0�� ��ȯ
	if (RegisterClassEx(&wcx) == 0)
	{
		return false;
	}

	// ���α׷� â ����
	// �� �Ű������� �ǹ̴� �̷���
	// _In_ DWORD dwExStyle         => Ȯ�� ������ ��Ÿ��(���� �ǹ� ����)
	// _In_opt_ LPCSTR lpClassName  => ����� WNDCLASSEX �̸�
	// _In_opt_ LPCSTR lpWindowName => ���α׷� ���� ǥ���ٿ� ��µ� ���ڿ�
	// _In_ DWORD dwStyle           => ���α׷� â ��Ÿ��
	// _In_ int X                   => ���α׷� â ���� �� ��ǥ
	// _In_ int Y                   => ���α׷� â ���� �� ��ǥ
	// _In_ int nWidth              => ���α׷� â ���� ����
	// _In_ int nHeight             => ���α׷� â ���� ����
	// _In_opt_ HWND hWndParent     => �θ� ���α׷� â(���� �ǹ� ����)
	// _In_opt_ HMENU hMenu         => ���α׷� â �޴�
	// _In_opt_ HINSTANCE hInstance => ���α׷��� �ĺ��� �ν��Ͻ�
	// _In_opt_ LPVOID lpParam      => �߰� �Ű�����(���� �ǹ� ����)
	//
	// ���ӿ����� �Ϲ������� â����� ��� WS_OVERLAPPED��
	// ��üȭ���� ��� WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP(â �׵θ� �����)�� ���
	// WS_OVERLAPPEDWINDOW�� ���α׷� �޴�(��Ʈ���̶�� ��)�� �ְ� ũ�� ������ ������ ��Ÿ��
	// WS_OVERLAPPED�� ���α׷� �޴��� ���� ũ�� ������ �Ұ����� ��Ÿ��
	// CW_USEDEFAULT�� �⺻ ��ġ�� ����Ѵٴ� ǥ��
	HWND hWnd = CreateWindow(_T("WinMain"), _T("Hello World"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	// ���α׷� â ���� ���� �� nullptr�� ��ȯ��
	if (hWnd == nullptr)
	{
		return false;
	}

	ShowWindow(hWnd, cmdShow); // ���α׷� â ǥ��
	UpdateWindow(hWnd);        // WM_PAINT �޽����� ������ ���ν������� ����

	return true;
}

bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance)
{
	// ���� �� �ڼ��� ������ �Ʒ��� �������ּ���.
	// cb => Class byte
	// cs => Class styles
	// Redraw => ���α׷� â�� ũ�⸦ �������� ��, ���α׷� ������ ���� ����� â�� ���缭 ������ϴ� ��.
	// lpfnWndProc�� ���� => typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	//                       CALLBACK�� �Լ� ȣ�� �Ծ�, LRESULT�� ��ȯ�ϰ� HWND, UINT, WPARAM, LPARAM�� �Ű������� �Լ��� ������
	//
	// <WNDCLASSEX ������ ����>
	// UINT cbSize;
	//      WNDCLASSEX�� ũ�⸦ ����, sizeof()�� Ȱ��
	//
	// /* Win 3.x */
	// UINT style;
	//      ���α׷� â�� �����ϴ� ����� ������ ��Ÿ�� ����
	//      ��Ʈ OR ������ |���� ������ �� ����(��Ÿ�ϰ��� ��Ʈ �÷��׷� ������)
	//
	// WNDPROC lpfnWndProc;
	//         ������ OS�� ���� �޽����� ó���ϴ� �Լ��� ����Ű�� ������
	//         ������ �޽����� ó���ϴ� �Լ��� ���� �ۼ��ؾ���
	//
	// int cbClsExtra;
	//     WNDCLASSEX�� ���� Ȯ���ؾ��� ������ �޸�, ������ �ʿ� ����
	//
	// int cbWndExtra;
	//     WNDCLASSEX�� �����ϰ� �ִ� ���α׷� â�� ���� Ȯ���ؾ��� ������ �޸�, ������ �ʿ� ����
	//
	// HINSTANCE hInstance;
	//           WNDCLASSEX�� ����� ���α׷��� ���, �ٸ� ���α׷��� �����ϱ� ���� �����
	//
	// HICON hIcon;
	//       ���α׷����� ����� ū �������� ����(���α׷� �������� �ǹ�)
	//
	// HCURSOR hCursor;
	//         ���α׷����� ����� ���콺 Ŀ�� ����� ����(����Ʈ Ŀ�� ����� ȭ��ǥ)
	//
	// HBRUSH hbrBackground;
	//        ���α׷��� ������ ����(�Ϲ������δ� ��� �Ǵ� �������� ���)
	//
	// LPCWSTR lpszMenuName;
	//         ���α׷����� ����� �⺻ �޴� ����, ������� �������� nullptr
	//
	// LPCWSTR lpszClassName;
	//         �̹� ��ϵǾ��ִ� �ٸ� WNDCLASSEX�� ������ �ĺ���
	//         ���α׷� â�� ������ �� �����
	//
	// /* Win 4.0 */
	// HICON hIconSm;
	//       ������ OS�� �۾� ǥ���ٰ� ���� �޴��� ����� ���� �������� ����

	// WNDCLASSEX ������ �ʱ�ȭ�Դϴ�.
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
	pWCX->hIconSm = nullptr;

	return true;
}

// wParam => Word Parameter
// lParam => Long Parameter
//
// wParam�� UINT_PTR, lParam�� LONG_PTR
// UINT_PTR�� unsigned int, LONG_PTR�� long
// �̸��� PTR�� ������ 32��Ʈ �ý��۰� 64��Ʈ �ý����� ������ ũ�Ⱑ �ٸ��� ����
// ȣȯ���� ���� �ý��ۿ� �´� ������ ũ�⸦ ���� �� �ִ� �����̶�� �ǵ��� �ֱ� ����.
//
// Word�� 2����Ʈ�� ���ϰ� Long�� 4����Ʈ�� ����.
// ������ 32��Ʈ �ý������� �Ѿ���鼭 int�� 4����Ʈ�� �νĵǹǷ�
// wParam�� lParam�� ũ��� �����ϴٰ� �����.
//
// wParam���� �ַ� ���� ����ǰ�, lParam���� ���Ӹ� �ƴ϶� �����͵� �����(�ּҰ�)
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); // Default Window Procedure
}