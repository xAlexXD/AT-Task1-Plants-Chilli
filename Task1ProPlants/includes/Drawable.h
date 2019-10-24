#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable
{
	template<class T>
	friend class DrawablesBase;

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual void Update(float dt) noexcept = 0;
	void Draw(Graphics& gfx) const noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

protected:
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;
	Bindable* GetPointerToLastBindable() noexcept;

	//template<class B>
	//bool GetBindablePointer(B* ref)
	//{
	//	for (const auto& b : _binds)
	//	{
	//		if (const auto p = dynamic_cast<B*>(b.get()))
	//		{
	//			ref = p;
	//			return true;
	//		}
	//	}
	//	return false;
	//}

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

	const class IndexBuffer* _pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> _binds;
};
