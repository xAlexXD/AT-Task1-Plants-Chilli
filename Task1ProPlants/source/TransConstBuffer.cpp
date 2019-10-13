#include "..\includes\TransConstBuffer.h"

TransConstBuffer::TransConstBuffer(Graphics& gfx, const Drawable& parent) : _parent(parent)
{
	if (!_pVcBuf)
	{
		_pVcBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransConstBuffer::Bind(Graphics& gfx) noexcept
{
	_pVcBuf->Update(
		gfx,
		DirectX::XMMatrixTranspose(_parent.GetTransformXM() * gfx.GetProjection())
	);
	_pVcBuf->Bind(gfx);
}

//Declaration for private static variable, needed so the linker can find where it is ?
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransConstBuffer::_pVcBuf;
