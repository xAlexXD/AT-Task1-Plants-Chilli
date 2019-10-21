#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"

class SolidSphere : public DrawablesBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	std::unique_ptr<GameObjectTransform> _transform;
};

