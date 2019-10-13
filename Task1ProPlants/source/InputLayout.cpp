#include "..\includes\InputLayout.h"
#include "GraphicsThrowMacros.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	HRESULT hr;
	GFX_THROW_FAILED(GetDevice(gfx)->CreateInputLayout(layout.data(), (UINT)layout.size(), pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &_pInputLayout));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(_pInputLayout.Get());
}
