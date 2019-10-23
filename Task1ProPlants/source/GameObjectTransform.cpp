#include "GameObjectTransform.h"

GameObjectTransform::GameObjectTransform()
{
	DirectX::XMStoreFloat3x3(&_modelTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
}

GameObjectTransform::GameObjectTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta) :
	_xPos(pos.x), _yPos(pos.y), _zPos(pos.z),
	_xRot(rot.x), _yRot(rot.y), _zRot(rot.z),
	_xPosDelta(posDelta.x), _yPosDelta(posDelta.y), _zPosDelta(posDelta.z),
	_xRotDelta(rotDelta.x), _yRotDelta(rotDelta.y), _zRotDelta(rotDelta.z)
{
	DirectX::XMStoreFloat3x3(&_modelTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
}

GameObjectTransform::~GameObjectTransform()
{
}

void GameObjectTransform::Update(float dt) noexcept
{
	_xPos += _xPosDelta * dt;
	_yPos += _yPosDelta * dt;
	_zPos += _zPosDelta * dt;
	_xRot += _xRotDelta * dt;
	_yRot += _yRotDelta * dt;
	_zRot += _zRotDelta * dt;
}

DirectX::XMMATRIX GameObjectTransform::GetGameObjectTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&_modelTransform) *						//Takes the models transform as a modifer
		DirectX::XMMatrixRotationRollPitchYaw(_yRot, _zRot, _xRot) *		//Rotates around the cubes center
		DirectX::XMMatrixTranslation(_xPos, _yPos, _zPos);						//Translates the cube in that pos
}

DirectX::XMFLOAT3X3& GameObjectTransform::GetModelTransform() noexcept
{
	return _modelTransform;
}

void GameObjectTransform::SetPosition(DirectX::XMFLOAT3 pos)
{
	_xPos = pos.x;
	_yPos = pos.y;
	_zPos = pos.z;
}

void GameObjectTransform::SetLocalRotation(DirectX::XMFLOAT3 rot)
{
	_xRot = rot.x;
	_yRot = rot.y;
	_zRot = rot.z;
}

void GameObjectTransform::SetPositionDelta(DirectX::XMFLOAT3 posDelta)
{
	_xPosDelta = posDelta.x;
	_yPosDelta = posDelta.y;
	_zPosDelta = posDelta.z;
}

void GameObjectTransform::SetLocalRotationDelta(DirectX::XMFLOAT3 rotDelta)
{
	_xRotDelta = rotDelta.x;
	_yRotDelta = rotDelta.y;
	_zRotDelta = rotDelta.z;
}
