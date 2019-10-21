#include "..\includes\TransConstBuffer.h"

TransConstBuffer::TransConstBuffer(Graphics& gfx, const Drawable& parent) : _parent(parent)
{
	if (!_pVcBuf)
	{
		_pVcBuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
}

void TransConstBuffer::Bind(Graphics& gfx) noexcept
{
	const auto model = _parent.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model), 
		DirectX::XMMatrixTranspose(model * gfx.GetCamera() * gfx.GetProjection())

	};

	_pVcBuf->Update(gfx, tf);
	_pVcBuf->Bind(gfx);
}

//Declaration for private static variable, needed so the linker can find where it is ?
std::unique_ptr<VertexConstantBuffer<TransConstBuffer::Transforms>> TransConstBuffer::_pVcBuf;
