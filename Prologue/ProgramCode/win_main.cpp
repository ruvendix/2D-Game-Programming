/*
 * <생성 날짜>
 * 2017-12-02
 *
 * <파일 내용>
 * Win32 API를 이용한 Hello World 프로그램입니다.
 */

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#undef  WIN32_LEAN_AND_MEAN
#else
	#include <windows.h>
#endif

#include <cstdlib>

// 멀티바이트 문자 집합과 유니코드 문자 집합 중 선택하게 하기 위한 헤더파일
#include <tchar.h>

static INT32 SCREEN_WIDTH  = 800;
static INT32 SCREEN_HEIGHT = 600;

static const INT32 INIT_INSTANCE_FAILURE = 0;

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow);
bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance);
LRESULT WINAPI WinProc(HWND hWnd, UINT32 msg, WPARAM wParam, LPARAM lParam);

// HINSTANCE(Handle Instance) => void 포인터(모든 형식을 받을 수 있는 포인터), 주로 접두어로 h 사용
// LPSTR(Long Pointer String) => char 포인터(문자열을 의미함), 주로 접두어로 lp 사용(str 또는 sz라고 해도 상관없음)
// LPCSTR(Long Pointer Const String) => LPSTR의 const 버전
// LPWSTR(Long Pointer Wide String)  => LPSTR의 wchar 버전
// LPCWSTR(Long Pointer Const Wide String) => LPCSTR의 wchar 버전
// LPTSTR(Long Pointer Transformation String)        => LPSTR과 LPWSTR중 하나를 선택(TCHAR)
// LPCTSTR(Long Pointer Const Transformation String) => LPCSTR과 LPCWSTR중 하나를 선택(const TCHAR)
// LRESULT(Long Result) => long을 의미함
//
// hInstance     => 프로그램 포인터(어떤 프로그램이 실행 중인지 판단할 때 사용)
// hPrevInstance => 항상 nullptr, 이전 버전 윈도우 OS와의 호환성을 위한 변수(16비트 윈도우 시절...)
// lpCmdLine     => 프로그램이 실행될 때 전달되는 내용(argv라고 생각하면 됨)
// cmdShow       => 프로그램 창을 어떻게 보여줄 것인지 설정(일반, 최소화, 최대화 등)
//
// WM_QUIT 메시지를 받을 때까지 루프를 돌리는 함수
// 프로그램 창을 보여주려면 WNDCLASSEX를 생성하고 등록해야함(말이 클래스지 C언어이므로 구조체임)
// _tWinMain()은 멀티바이트냐 유니코드냐에 따라 WinMain()과 wWinMain()중 하나를 선택함
// TEXT()와 _T()도 같은 건데 TEXT()는 Win32 API에서 쓰던 것, _T()는 MFC에서 쓰는 건데 기능은 동일함
INT32 APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT32 cmdShow)
{
	// cmdShow의 값들
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
	// SW_HIDE(백그라운드 프로세스)
	// SW_NORMAL(포그라운드 프로세스, 일반)
	// SW_MAXIMIZE(최대화)
	// SW_MINIMIZE(최소화)
	// 이 정도만 알면됨

	cmdShow = SW_NORMAL;

	// 안 쓰는 매개변수라고 표시함
	UNREFERENCED_PARAMETER(hPrevInstance);

	// WNDCLASSEX 생성 및 등록과 프로그램 창 생성 및 출력
	if (InitProgramInstance(hInstance, cmdShow) == false)
	{
		return INIT_INSTANCE_FAILURE;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// 메시지 루프
	bool bDrive = true;
	while (bDrive)
	{
		// GetMessage() 또는 PeekMessage()에 HWND를 nullptr로 설정하면
		// 현재 스레드를 포함한 현재 스레드에 속한 모든 메시지를 처리함
		// HWND를 -1로 설정하면 현재 스레드의 메시지만 처리함(PostMessage() 또는 PostThreadMessage())
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDrive = false;
			}

			TranslateMessage(&msg); // 가상 키 메시지를 문자 메시지로 바꿔줌
			DispatchMessage(&msg);  // 윈도우 프로시저에게 메시지를 보내줌
		}
	}

	// main()과는 달리 WinMain()은 메시지값을 반환해야함!
	// 그래야 좀비 프로세스가 되지 않고 정상 종료됨.
	return static_cast<INT32>(msg.wParam);
}

bool InitProgramInstance(HINSTANCE hInstance, INT32 cmdShow)
{
	WNDCLASSEX wcx;
	ZeroMemory(&wcx, sizeof(wcx));
	InitWindowClass(&wcx, hInstance);

	// 윈도우 OS에 설정한 WNDCLASSEX를 등록하는 함수
	// WNDCLASSEX 등록에 실패하면 0을 반환
	if (RegisterClassEx(&wcx) == 0)
	{
		return false;
	}

	// 프로그램 창 생성
	// 각 매개변수의 의미는 이러함
	// _In_ DWORD dwExStyle         => 확장 윈도우 스타일(별로 의미 없음)
	// _In_opt_ LPCSTR lpClassName  => 등록한 WNDCLASSEX 이름
	// _In_opt_ LPCSTR lpWindowName => 프로그램 제목 표시줄에 출력될 문자열
	// _In_ DWORD dwStyle           => 프로그램 창 스타일
	// _In_ int X                   => 프로그램 창 왼쪽 위 좌표
	// _In_ int Y                   => 프로그램 창 왼쪽 위 좌표
	// _In_ int nWidth              => 프로그램 창 가로 길이
	// _In_ int nHeight             => 프로그램 창 세로 길이
	// _In_opt_ HWND hWndParent     => 부모 프로그램 창(별로 의미 없음)
	// _In_opt_ HMENU hMenu         => 프로그램 창 메뉴
	// _In_opt_ HINSTANCE hInstance => 프로그램을 식별할 인스턴스
	// _In_opt_ LPVOID lpParam      => 추가 매개변수(별로 의미 없음)
	//
	// 게임에서는 일반적으로 창모드일 경우 WS_OVERLAPPED를
	// 전체화면일 경우 WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP(창 테두리 미출력)을 사용
	// WS_OVERLAPPEDWINDOW는 프로그램 메뉴(컨트롤이라고도 함)가 있고 크기 조절이 가능한 스타일
	// WS_OVERLAPPED는 프로그램 메뉴가 없고 크기 조절이 불가능한 스타일
	// CW_USEDEFAULT는 기본 위치를 사용한다는 표시
	HWND hWnd = CreateWindow(_T("WinMain"), _T("Hello World"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	// 프로그램 창 생성 실패 시 nullptr이 반환됨
	if (hWnd == nullptr)
	{
		return false;
	}

	ShowWindow(hWnd, cmdShow); // 프로그램 창 표시
	UpdateWindow(hWnd);        // WM_PAINT 메시지를 윈도우 프로시저에게 보냄

	return true;
}

bool InitWindowClass(WNDCLASSEX* pWCX, HINSTANCE hInstance)
{
	// 약자 및 자세한 정보는 아래를 참고해주세요.
	// cb => Class byte
	// cs => Class styles
	// Redraw => 프로그램 창의 크기를 변경했을 때, 프로그램 내용을 새로 변경된 창에 맞춰서 재출력하는 것.
	// lpfnWndProc의 형식 => typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	//                       CALLBACK은 함수 호출 규약, LRESULT를 반환하고 HWND, UINT, WPARAM, LPARAM이 매개변수인 함수의 포인터
	//
	// <WNDCLASSEX 데이터 정보>
	// UINT cbSize;
	//      WNDCLASSEX의 크기를 저장, sizeof()를 활용
	//
	// /* Win 3.x */
	// UINT style;
	//      프로그램 창을 갱신하는 방법을 포함한 스타일 설정
	//      비트 OR 연산자 |으로 결합할 수 있음(스타일값은 비트 플래그로 제공됨)
	//
	// WNDPROC lpfnWndProc;
	//         윈도우 OS에 보낸 메시지를 처리하는 함수를 가리키는 포인터
	//         윈도우 메시지를 처리하는 함수를 따로 작성해야함
	//
	// int cbClsExtra;
	//     WNDCLASSEX를 위해 확보해야할 여분의 메모리, 지금은 필요 없음
	//
	// int cbWndExtra;
	//     WNDCLASSEX가 소유하고 있는 프로그램 창을 위해 확보해야할 여분의 메모리, 지금은 필요 없음
	//
	// HINSTANCE hInstance;
	//           WNDCLASSEX를 등록한 프로그램을 등록, 다른 프로그램과 구분하기 위해 사용함
	//
	// HICON hIcon;
	//       프로그램에서 사용할 큰 아이콘을 설정(프로그램 아이콘을 의미)
	//
	// HCURSOR hCursor;
	//         프로그램에서 사용할 마우스 커서 모양을 설정(디폴트 커서 모양은 화살표)
	//
	// HBRUSH hbrBackground;
	//        프로그램의 배경색을 설정(일반적으로는 흰색 또는 검은색을 사용)
	//
	// LPCWSTR lpszMenuName;
	//         프로그램에서 사용할 기본 메뉴 설정, 사용하지 않으려면 nullptr
	//
	// LPCWSTR lpszClassName;
	//         이미 등록되어있는 다른 WNDCLASSEX와 구분할 식별자
	//         프로그램 창을 생성할 때 사용함
	//
	// /* Win 4.0 */
	// HICON hIconSm;
	//       윈도우 OS의 작업 표시줄과 시작 메뉴에 사용할 작은 아이콘을 설정

	// WNDCLASSEX 데이터 초기화입니다.
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
// wParam은 UINT_PTR, lParam은 LONG_PTR
// UINT_PTR은 unsigned int, LONG_PTR은 long
// 이름이 PTR인 이유는 32비트 시스템과 64비트 시스템의 포인터 크기가 다르다 보니
// 호환성을 위해 시스템에 맞는 포인터 크기를 담을 수 있는 형식이라는 의도가 있기 때문.
//
// Word는 2바이트를 뜻하고 Long은 4바이트를 뜻함.
// 하지만 32비트 시스템으로 넘어오면서 int는 4바이트로 인식되므로
// wParam과 lParam의 크기는 동일하다고 보면됨.
//
// wParam에는 주로 값이 저장되고, lParam에는 값뿐만 아니라 포인터도 저장됨(주소값)
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