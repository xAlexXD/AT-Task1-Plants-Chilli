#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"
#include "StructDefs.h"

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

	void SpawnImGuiWindow(Graphics& gfx) noexcept;

	void UpdateLocalVertsAndInds(Graphics& gfx);

	std::vector<TexturedVertex> _vertOut;
	std::vector<int> _indexOut;

private:

	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<TexturedVertex>* _vertexBuffer = nullptr;
};

