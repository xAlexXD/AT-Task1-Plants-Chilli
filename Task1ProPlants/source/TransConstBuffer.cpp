#include "..\includes\TransConstBuffer.h"

TransConstBuffer::TransConstBuffer(Graphics& gfx, const Drawable& parent, UINT slot) : _parent(parent)
{
	if (!_pVcBuf)
	{
		_pVcBuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransConstBuffer::Bind(Graphics& gfx) noexcept
{
	const auto modelView = _parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView), 
		DirectX::XMMatrixTranspose(modelView * gfx.GetProjection())

	};

	_pVcBuf->Update(gfx, tf);
	_pVcBuf->Bind(gfx);
}

//Declaration for private static variable, needed so the linker can find where it is ?
std::unique_ptr<VertexConstantBuffer<TransConstBuffer::Transforms>> TransConstBuffer::_pVcBuf;
