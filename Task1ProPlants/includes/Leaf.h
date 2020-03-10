#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"
#include "StructDefs.h"

class Leaf : public DrawablesBase<Leaf>
{
public:
	Leaf(Graphics& gfx,
		const char* textureName,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 posDelta,
		DirectX::XMFLOAT3 rotDelta,
		DirectX::XMFLOAT3 worldRot,
		DirectX::XMFLOAT3 worldDelta
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	GameObjectTransform* GetLeafTransform() noexcept;

	void UpdateOutVertices(Graphics& gfx);

	std::vector<TexturedVertex> _vertOut;
	std::vector<int> _indexOut;

	void UpdateTexture(Graphics& gfx, const char* filePath);
private:
	DirectX::XMFLOAT3 _pivotPos = {0.0f, 1.5f, 0.0f};
	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<TexturedVertex>* _vertexBuffer = nullptr;
	Texture* _textureBindable = nullptr;
};

