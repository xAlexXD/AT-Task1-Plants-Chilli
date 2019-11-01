#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"

class Stem : public DrawablesBase<Stem>
{
public:
	struct TexturedVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tc;
	};

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

private:

	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<TexturedVertex>* _vertexBuffer = nullptr;

	std::vector<Stem::TexturedVertex> _vertOut;
	std::vector<int> _indexOut;
};

