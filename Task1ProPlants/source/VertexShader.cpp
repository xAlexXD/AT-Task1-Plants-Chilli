#include "..\includes\VertexShader.h"
#include "GraphicsThrowMacros.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	HRESULT hr;

	//Create vertex shader
	GFX_THROW_FAILED(D3DReadFileToBlob(path.c_str(), &_pBytecodeBlob));
	GFX_THROW_FAILED(GetDevice(gfx)->CreateVertexShader(_pBytecodeBlob->GetBufferPointer(), _pBytecodeBlob->GetBufferSize(), nullptr, &_pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	//Bind vertex shader to pipeline
	GetContext(gfx)->VSSetShader(_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return _pBytecodeBlob.Get();
}
