#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"

class Sheet : public DrawablesBase<Sheet>
{
public:
	Sheet(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& rDist,
		std::uniform_real_distribution<float>& localRotDelta,
		std::uniform_real_distribution<float>& worldRotDelta,
		std::uniform_real_distribution<float>& worldRot
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	std::unique_ptr<GameObjectTransform> _transform;
};

