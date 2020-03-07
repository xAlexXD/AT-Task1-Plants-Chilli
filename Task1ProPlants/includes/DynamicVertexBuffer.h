#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include <vector>
#include "imgui.h"


template<class V>
class DynamicVertexBuffer : public Bindable
{
protected:
	bool _vertDirty = false;
	UINT _stride;
	UINT _initialLength;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pCopyBuffer;
	std::vector<V> _vertices;
	std::vector<V> _originalVertices;

public:
	DynamicVertexBuffer(Graphics& gfx, std::vector<V>& verts) : _stride(sizeof(V)), _initialLength(verts.size())
	{
		HRESULT hr;

		//Set up description of buffer
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * 50u);			//Allocating a shit ton more space for safety of adding or removing verts
		bd.StructureByteStride = sizeof(V);

		D3D11_BUFFER_DESC bd2 = {};
		bd2.Usage = D3D11_USAGE_STAGING;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bd2.ByteWidth = UINT(sizeof(V) * 50u);
		bd2.StructureByteStride = sizeof(V);

		_vertices.reserve(1000);
		//Initialise the internal pointer to the stored vertex array
		_vertices.assign(verts.begin(), verts.end());
		_originalVertices.assign(verts.begin(), verts.end());

		//Set the pointer to the data itself in the subresouce
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = _vertices.data();

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &_pVertexBuffer));
		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd2, nullptr, &_pCopyBuffer));
	}

	void Bind(Graphics& gfx) noexcept override
	{
		if (_vertDirty)
		{
			RemapVertexBuffer(gfx);
			_vertDirty = false;
		}

		const UINT offset = 0u;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), &_stride, &offset);
	}
	void RemapVertexBuffer(Graphics& gfx)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(_pVertexBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, _vertices.data(), sizeof(V) * _vertices.size());
		GetContext(gfx)->Unmap(_pVertexBuffer.Get(), 0u);
	}

	void ReadVertsOut(Graphics& gfx)
	{
		GetContext(gfx)->CopyResource(_pCopyBuffer.Get(), _pVertexBuffer.Get());

		D3D11_MAPPED_SUBRESOURCE sr = {};
		GetContext(gfx)->Map(_pCopyBuffer.Get(), 0u, D3D11_MAP_READ, 0u, &sr);
		V* data = reinterpret_cast<V*>(sr.pData);
		_vertices.clear();
		_vertices.reserve(_initialLength);
		for (int i = 0; i < _initialLength; i++)
		{
			_vertices.push_back(data[i]);
		}
		GetContext(gfx)->Unmap(_pCopyBuffer.Get(), 0u);
	}

	std::vector<V> GetVerts()
	{
		return _vertices;
	}

	std::vector<V> GetOriginalVerts()
	{
		return _originalVertices;
	}

	void UpdateVerts(std::vector<V> verts)
	{
		_vertices.clear();
		_vertices.assign(verts.begin(), verts.end());
		_vertDirty = true;
	}

	void SpawnImguiControlWindow()
	{
		if (ImGui::Begin("Vertex Buffer"))
		{
		}
		ImGui::End();
	}

	void Reset() noexcept
	{
		_vertices.assign(_originalVertices.begin(), _originalVertices.end());
		_vertDirty = true;
	}
};

//template<class V>
//std::vector<V> DynamicVertexBuffer<V>::_vertices;
//
//template<class V>
//std::vector<V> DynamicVertexBuffer<V>::_originalVertices;