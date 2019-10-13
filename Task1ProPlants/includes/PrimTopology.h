#pragma once
#include "Bindable.h"

class PrimTopology : public Bindable
{
public:
	PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx) noexcept override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY _type;
};

