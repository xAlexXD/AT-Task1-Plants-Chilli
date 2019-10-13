#include "..\includes\VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), &_stride, &offset);
}
