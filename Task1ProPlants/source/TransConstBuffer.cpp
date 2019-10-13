#include "..\includes\TransConstBuffer.h"

TransConstBuffer::TransConstBuffer(Graphics& gfx, const Drawable& parent) : _vcBuf(gfx), _parent(parent)
{
}

void TransConstBuffer::Bind(Graphics& gfx) noexcept
{
	_vcBuf.Update(
		gfx,
		DirectX::XMMatrixTranspose(_parent.GetTransformXM() * gfx.GetProjection())
	);
	_vcBuf.Bind(gfx);
}
