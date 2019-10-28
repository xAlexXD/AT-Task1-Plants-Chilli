#include "..\includes\Leaves.h"
#include "MathClass.h"

Leaves::Leaves(Graphics& gfx, 
	DirectX::XMFLOAT3 pos, 
	DirectX::XMFLOAT3 rot, 
	DirectX::XMFLOAT3 posDelta, 
	DirectX::XMFLOAT3 rotDelta, 
	DirectX::XMFLOAT3 worldRot, 
	DirectX::XMFLOAT3 worldDelta, 
	unsigned int leafCount) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta))
{
}

void Leaves::Update(float dt) noexcept
{
	//Need to work out the leaf positions
	//Iterate over the leaves themselves using the offsets calculated by placement set their positions 
	const float leafSpread = (2 * PI) / _leafCount;
	const DirectX::XMFLOAT2 startOffset = { 0.0f, _leafOffset };
	const DirectX::XMFLOAT3 parentPos = _transform->GetPosition();

	//To place the leaves
	for (int i = 0; i < _leafVector.size(); i++)
	{
		//Calculate the position of the leaf
		DirectX::XMFLOAT2 pos = {};
		//First get the direction vector of origin to start pos Replace zeros with offset from origin aka plant stem center
		DirectX::XMFLOAT2 startDirVec = { startOffset.x - parentPos.x, startOffset.y - parentPos.z };
		//Get the vector of the result after rot
		DirectX::XMFLOAT2 nextDirVec = { 0.0f, 0.0f };
		nextDirVec.x = (startDirVec.x * cos(leafSpread * (i + 1))) - (startDirVec.y * sin(leafSpread * (i + 1)));
		nextDirVec.y = (startDirVec.x * sin(leafSpread * (i + 1))) + (startDirVec.y * cos(leafSpread * (i + 1)));
		//Get the new position change zeros here when we get the new plant pos
		pos.x = nextDirVec.x + parentPos.x;
		pos.y = nextDirVec.y + parentPos.z;
		//Set the position to the leaf
		_leafVector[i].SetPosition(DirectX::XMFLOAT3(pos.x, parentPos.y, pos.y));
	}
}

void Leaves::DrawLeaves(Graphics& gfx) noexcept
{
	if (_leafVector.size() > 0)
	{
		for (auto& leaf : _leafVector)
		{
			leaf.Draw(gfx);
		}
	}
}
