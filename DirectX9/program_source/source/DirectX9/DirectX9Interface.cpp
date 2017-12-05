/*
 * <작성 날짜>
 * 2017-12-05
 *
 * <파일 설명>
 * DirectX9 인터페이스 래퍼 클래스 소스 파일입니다.
 */

#include "DirectX9/DirectX9Interface.h"

extern HWND g_hWnd;

INIT_SINGLETON_INSTANCE(d3d9::DirectX9Interface);

namespace d3d9
{

	HRESULT DirectX9Interface::InitDirectX9()
	{
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (m_pD3D == nullptr)
		{
			MSGBOX_OK("DirectX9 객체 생성 실패!");
			return E_FAIL;
		}

		D3DPRESENT_PARAMETERS param;
		ZeroMemory(&param, sizeof(param));

		param.BackBufferWidth            = 800;
		param.BackBufferHeight           = 600;
		param.BackBufferFormat           = D3DFMT_X8R8G8B8;
		param.BackBufferCount            = 1;
		param.MultiSampleType            = D3DMULTISAMPLE_NONE;
		param.MultiSampleQuality         = 0;
		param.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
		param.hDeviceWindow              = g_hWnd;
		param.Windowed                   = TRUE;
		param.EnableAutoDepthStencil     = FALSE;
		param.AutoDepthStencilFormat     = D3DFMT_D24S8;
		param.Flags                      = 0;
		param.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		param.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;

		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &param, &m_pDevice)))
		{
			MSGBOX_OK("DirectX9 디바이스 생성 실패!");
			return E_FAIL;
		}

		m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);
		m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);

		return S_OK;
	}

	HRESULT DirectX9Interface::ReleaseDirectX9()
	{
		m_pDevice->Release();
		m_pD3D->Release();

		return S_OK;
	}

} // namespace d3d9 end