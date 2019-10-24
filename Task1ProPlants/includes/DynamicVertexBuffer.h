#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include <vector>
#include "imgui.h"


template<class V>
class DynamicVertexBuffer : public Bindable
{
public:
	DynamicVertexBuffer(Graphics& gfx, const std::vector<V>& verts) : _stride(sizeof(V))
	{
		HRESULT hr;

		//Set up description of buffer
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * 1000u);			//Allocating a shit ton more space for safety of adding or removing verts
		bd.StructureByteStride = sizeof(V);

		//Initialise the internal pointer to the stored vertex array
		_vertices.assign(verts.begin(), verts.end());
		_originalVertices.assign(verts.begin(), verts.end());

		//Set the pointer to the data itself in the subresouce
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = _vertices.data();

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &_pVertexBuffer));
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

	void SpawnImguiControlWindow()
	{
		if (ImGui::Begin("Vertex Buffer"))
		{
			ImGui::Text("Vertices");
			if (ImGui::Button("Reset"))
			{
				Reset();
			}

			assert("Not a modulus of 3 in vertex buffer" && _vertices.size() % 3 == 0);

			for (int i = 0; i < _vertices.size(); i += 3)
			{
				std::vector<float> triangle(3u);
				triangle[0] = _vertices[i].pos.x;
				triangle[1] = _vertices[i + 1].pos.y;
				triangle[2] = _vertices[i + 2].pos.z;
				_vertDirty =  ImGui::InputFloat3("Jef", triangle.data(), "%.1f") || _vertDirty;
			}
		}
		ImGui::End();
	}

	void Reset() noexcept
	{
		_vertices.assign(_originalVertices.begin(), _originalVertices.end());
		_vertDirty = true;
	}

protected:
	bool _vertDirty = false;
	UINT _stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
	std::vector<V> _vertices;
	std::vector<V> _originalVertices;
};