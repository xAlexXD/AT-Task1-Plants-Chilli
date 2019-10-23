#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"

class Pyramid : public DrawablesBase<Pyramid>
{
public:
	Pyramid(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 matCol,
		int tessalation
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	std::unique_ptr<GameObjectTransform> _transform;
};