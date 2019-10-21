#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransConstBuffer : public Bindable
{
public:
	TransConstBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;

private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProjection;
		DirectX::XMMATRIX model;
	};

	static std::unique_ptr<VertexConstantBuffer<Transforms>> _pVcBuf;
	const Drawable& _parent;
};