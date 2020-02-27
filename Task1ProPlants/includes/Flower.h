#pragma once
#include "Leaves.h"
#include "Stem.h"

class Flower
{
public:
	Flower() = delete;
	Flower(Graphics& gfx, )

		Leaves(Graphics& gfx,
			unsigned int leafCount,
			const char* textureName,
			const char* bunchName
		);

	Stem(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 worldRot,
		DirectX::XMFLOAT3 worldDelta
	);

private:

};

