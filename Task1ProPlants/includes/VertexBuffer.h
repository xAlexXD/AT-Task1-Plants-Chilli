#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& verts) : _stride(sizeof(V))
	{
		HRESULT hr;

		//Set up description of buffer
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * verts.size());
		bd.StructureByteStride = sizeof(V);

		//Set the pointer to the data itself in the subresouce
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = verts.data();

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &_pVertexBuffer));
	}

	void Bind(Graphics& gfx) noexcept override;

protected:
	UINT _stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
};
