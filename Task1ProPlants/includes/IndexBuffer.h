#pragma once
#include "Bindable.h"


class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetCount() const noexcept;

protected:
	UINT _count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pIndexBuffer;
};

