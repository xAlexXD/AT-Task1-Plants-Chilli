#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"

class Stem : public DrawablesBase<Stem>
{
public:
	Stem(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 worldRot,
		DirectX::XMFLOAT3 worldDelta
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	void SpawnImGuiWindow() noexcept;

private:
	struct StemVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};

	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<StemVertex>* _vertexBuffer = nullptr;
};

