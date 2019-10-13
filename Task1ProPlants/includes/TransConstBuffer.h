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
	VertexConstantBuffer<DirectX::XMMATRIX> _vcBuf;
	const Drawable& _parent;
};

