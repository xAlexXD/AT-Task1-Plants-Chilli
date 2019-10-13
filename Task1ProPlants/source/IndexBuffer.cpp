#include "..\includes\IndexBuffer.h"
#include "GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
{
	HRESULT hr;

	//Set up buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(sizeof(unsigned short) * indices.size());
	ibd.StructureByteStride = sizeof(unsigned short);

	//Setup the data reference the buffer is going to use
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data;

	//Create the buffer itself
	GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &_pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	//Bind indices buffer to pipeline
	GetContext(gfx)->IASetIndexBuffer(_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return _count;
}
