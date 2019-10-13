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
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufDesc.CPUAccessFlags = 0u;
		vertexBufDesc.MiscFlags = 0u;
		vertexBufDesc.ByteWidth = UINT(sizeof(V) * verts.size());
		vertexBufDesc.StructureByteStride = sizeof(V);

		//Set the pointer to the data itself in the subresouce
		D3D11_SUBRESOURCE_DATA sd = {};
		verDataDesc.pSysMem = verts.data();

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &_pVertexBuffer));
	}

	void Bind(Graphics& gfx) noexcept override;

protected:
	UINT _stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
};
