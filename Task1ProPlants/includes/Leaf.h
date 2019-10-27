#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"

class Leaf : public DrawablesBase<Leaf>
{
public:
	Leaf(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 worldRot,
		DirectX::XMFLOAT3 worldDelta,
		DirectX::XMFLOAT3* parentPos
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	void SpawnImGuiWindow() noexcept;

private:
	struct TexturedVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tc;
	};

	DirectX::XMFLOAT3* _parentsPos = nullptr;
	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<TexturedVertex>* _vertexBuffer = nullptr;
};

