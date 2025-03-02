#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"

class Sheet : public DrawablesBase<Sheet>
{
public:
	Sheet(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta
	);
	Sheet(Graphics& gfx);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	void Init(Graphics& gfx);

	std::unique_ptr<GameObjectTransform> _transform;
};