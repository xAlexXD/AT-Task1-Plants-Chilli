#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	//2 constructors one for defining the data filling a const buffer and one to just create an empty one
	ConstantBuffer(Graphics& gfx, const C& consts)
	{
		//Set up const buffer description
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		//Setup the data the const buffer will use
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;

		//Create the const buffer
		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &_pConstBuffer));
	}

	ConstantBuffer(Graphics& gfx)
	{
		//Set up const buffer description
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		//Create the const buffer
		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &_pConstBuffer));
	}

	//Constant buffers tend to need to be updated frequently for example const buffers that hold positional matrixes
	void Update(graphics& gfx, const C& consts)
	{
		//Map the buffer so that it cant be accessed whilst we write to it and update the buffer with the new content
		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_FAILED(GetContext(gfx)->Map(_pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(_pConstBuffer.Get(), 0u);
	}


protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pConstBuffer;
};

//Custom class variations for the vertex and pixel const buffer 
//because the base class can take any types of structs for their buffers but depending on whether its VS or PS it gets set differently
template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::_pConstBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, _pConstBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::_pConstBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, _pConstBuffer.GetAddressOf());
	}
};