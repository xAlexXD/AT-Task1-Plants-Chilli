#include "..\includes\TransConstBuffer.h"

TransConstBuffer::TransConstBuffer(Graphics& gfx, const Drawable& parent) : _vcBuf(gfx), _parent(parent)
{
}

void TransConstBuffer::Bind(Graphics& gfx) noexcept
{
	//_vcBuf.Update()
	_vcBuf.Bind(gfx);
}
