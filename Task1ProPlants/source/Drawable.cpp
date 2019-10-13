#include "..\includes\Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept
{
	for (auto& b : _binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	assert("*MUST* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	_binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept
{
	assert("Attempting to add index buffer for a second time" && _pIndexBuffer == nullptr);
	_pIndexBuffer = iBuf.get();
	_binds.push_back(std::move(iBuf));
}
