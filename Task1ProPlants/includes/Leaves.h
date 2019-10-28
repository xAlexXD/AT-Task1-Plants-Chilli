#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"
#include "Leaf.h"


class Leaves
{
public:
	Leaves(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 worldRot,
		DirectX::XMFLOAT3 worldDelta,
		unsigned int leafCount
	);

	void Update(float dt) noexcept;
	void DrawLeaves(Graphics& gfx) noexcept;

private:
	std::unique_ptr<GameObjectTransform> _transform;
	std::vector<Leaf> _leafVector;
	unsigned int _leafCount = 0.0f;
	float _leafPitch = 0.0f;
	float _leafYaw = 0.0f;
	float _leafOffset = 1.0f;
};

