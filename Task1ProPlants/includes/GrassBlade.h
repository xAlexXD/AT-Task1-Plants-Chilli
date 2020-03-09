#pragma once
#include "DrawablesBase.h"
#include "DynamicVertexBuffer.h"
#include "GameObjectTransform.h"
#include "StructDefs.h"
#include <array>

class GrassBlade : public DrawablesBase<GrassBlade>
{
public:
	GrassBlade(Graphics& gfx,
		const char* name,
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

	GameObjectTransform* GetFaceTransform() noexcept;
	int GetVertCount()	{ return _vertCount; }
	int GetIndCount() { return _indCount; }

	void UpdateOutVertices(Graphics& gfx);

	void SpawnImGui(Graphics& gfx);

	std::vector<TexturedVertex> _vertOut;
	std::vector<int> _indexOut;

private:
	void CalcOffsetsAndSetBuffer();

	DirectX::XMFLOAT3 _pivotPos = { 0.0f, 0.0f, 0.0f };
	std::unique_ptr<GameObjectTransform> _transform = nullptr;
	DynamicVertexBuffer<TexturedVertex>* _vertexBuffer = nullptr;
	int _vertCount = 0;
	int _indCount = 0;

	std::string _name;

	DirectX::XMFLOAT3 _position = {};
	DirectX::XMFLOAT3 _rotation = {};
	DirectX::XMFLOAT3 _scale = {1.0f, 1.0f, 1.0f};
	DirectX::XMFLOAT3 _layerOffset = {};
	float _curveIntensity = 0.0f;
	float _curveHeightMultiplier = 0.0f;
	float _curveSpanOffset = 0.0f;
};

