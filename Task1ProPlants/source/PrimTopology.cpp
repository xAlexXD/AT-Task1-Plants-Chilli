#include "..\includes\PrimTopology.h"

PrimTopology::PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type) : _type(type)
{
}

void PrimTopology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(_type);
}
