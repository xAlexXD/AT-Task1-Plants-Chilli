#include "..\includes\Leaf.h"

Leaf::Leaf(Graphics& gfx, 
	DirectX::XMFLOAT3 pos, 
	DirectX::XMFLOAT3 rot, 
	DirectX::XMFLOAT3 posDelta, 
	DirectX::XMFLOAT3 rotDelta, 
	DirectX::XMFLOAT3 worldRot, 
	DirectX::XMFLOAT3 worldDelta,
	DirectX::XMFLOAT3& parentPos) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta)), _parentsPos(parentPos)
{
}

void Leaf::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Leaf::GetTransformXM() const noexcept
{
	return _transform->GetTransformWithWorldOffsetXM(_parentsPos);
}
