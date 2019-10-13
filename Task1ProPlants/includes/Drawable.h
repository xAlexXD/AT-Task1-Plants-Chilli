#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual void Update(float dt) noexcept = 0;
	void Draw(Graphics& gfx) const noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;

private:
	const IndexBuffer* _pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> _binds;
};

