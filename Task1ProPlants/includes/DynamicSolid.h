#pragma once
#include "DrawablesBase.h"
#include "GameObjectTransform.h"
#include "DynamicVertexBuffer.h"


class DynamicSolid : public DrawablesBase<DynamicSolid>
{
public:
	DynamicSolid(Graphics& gfx,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 matCol
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnImguiVerticesWindow() noexcept;

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};

	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<Vertex>* _vertexBuffer = nullptr;
};

