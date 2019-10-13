#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawablesBase : public Drawable
{
public:
	bool IsStaticInitialized() const noexcept
	{
		return !_staticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		assert("*MUST* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		_staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept
	{
		assert("Attempting to add index buffer for a second time" && _pIndexBuffer == nullptr);
		_pIndexBuffer = iBuf.get();
		_staticBinds.push_back(std::move(iBuf));
	}
	void SetIndexFromStatic() noexcept
	{
		assert("Attempting to add index buffer for a second time" && _pIndexBuffer == nullptr);
		for (const auto& b : _staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				_pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && _pIndexBuffer != nullptr);
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return _staticBinds;
	}
	static std::vector<std::unique_ptr<Bindable>> _staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawablesBase<T>::_staticBinds;