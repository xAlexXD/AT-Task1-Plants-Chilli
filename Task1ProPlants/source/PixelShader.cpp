#include "..\includes\PixelShader.h"
#include "GraphicsThrowMacros.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_FAILED(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_FAILED(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &_pPixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	//Bind pixel shader to pipeline
	GetContext(gfx)->PSSetShader(_pPixelShader.Get(), nullptr, 0u);
}
