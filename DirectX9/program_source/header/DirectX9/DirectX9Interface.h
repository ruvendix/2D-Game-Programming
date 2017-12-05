/*
 * <작성 날짜>
 * 2017-12-05
 *
 * <파일 설명>
 * DirectX9 인터페이스 래퍼 클래스 헤더 파일입니다.
 */

#ifndef DIRECTX9INTERFACE_H_
#define DIRECTX9INTERFACE_H_

#include "common/macro.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace d3d9
{
	
	class DirectX9Interface
	{
		DEFINE_SINGLETON_CLASS(DirectX9Interface);

	public:
		HRESULT InitDirectX9();
		HRESULT ReleaseDirectX9();

		LPDIRECT3DDEVICE9 getDevice() const noexcept
		{
			return m_pDevice;
		}

	private:
		IDirect3D9*       m_pD3D;
		IDirect3DDevice9* m_pDevice;
	};

} // namespace d3d9 end

#endif