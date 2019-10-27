#include "GameObjectTransform.h"
#include "imgui.h"

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

GameObjectTransform::GameObjectTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta, DirectX::XMFLOAT3 worldRot, DirectX::XMFLOAT3 worldDelta) :
	_xPos(pos.x), _yPos(pos.y), _zPos(pos.z),
	_xRot(rot.x), _yRot(rot.y), _zRot(rot.z),
	_xPosDelta(posDelta.x), _yPosDelta(posDelta.y), _zPosDelta(posDelta.z),
	_xRotDelta(rotDelta.x), _yRotDelta(rotDelta.y), _zRotDelta(rotDelta.z),
	_xWorldRot(worldRot.x), _yWorldRot(worldRot.y), _zWorldRot(worldRot.z),
	_xWorldRotDelta(worldDelta.x), _yWorldRotDelta(worldDelta.y), _zWorldRotDelta(worldDelta.z)
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

DirectX::XMMATRIX GameObjectTransform::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&_modelTransform) *						//Takes the models transform as a modifer
		DirectX::XMMatrixRotationRollPitchYaw(_yRot, _zRot, _xRot) *		//Rotates around the cubes center
		DirectX::XMMatrixTranslation(_xPos, _yPos, _zPos);						//Translates the cube in that pos
}

DirectX::XMMATRIX GameObjectTransform::GetTransformWithWorldOffsetXM(DirectX::XMFLOAT3 parentPos) const noexcept
{
	//First transform offsets camera away from origin, then rotates around the origin in pitch and yaw
	const auto pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(_yWorldRot, -_zWorldRot, _xWorldRot)
	);

	//Applies the local rotation to the world rotation calc'd previously
	return DirectX::XMMatrixLookAtLH(
		pos, DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) *
		DirectX::XMMatrixRotationRollPitchYaw(_yRot, -_zRot, _xRot) *
		DirectX::XMMatrixTranslation(_xPos, _yPos, _zPos);
}

DirectX::XMFLOAT3X3& GameObjectTransform::GetModelTransform() noexcept
{
	return _modelTransform;
}

void GameObjectTransform::SpawnImGuiWindow() noexcept
{
	if (ImGui::Begin("Model Transform Info"))
	{
		//ImGui::Text("Position");
		//ImGui::SliderFloat("X", &_xPos, 0.1f, 20.0f, "%.1f");
		//ImGui::SliderFloat("Y", &_yPos, 0.1f, 20.0f, "%.1f");
		//ImGui::SliderFloat("Z", &_zPos, 0.1f, 20.0f, "%.1f");

		//ImGui::Text("Local Rotation");
		//ImGui::SliderAngle("X", &_xRot, -180.0f, 180.0f);
		//ImGui::SliderAngle("Y", &_yRot, -180.0f, 180.0f);
		//ImGui::SliderAngle("Z", &_zRot, -180.0f, 180.0f);

		ImGui::Text("World Rotation");
		ImGui::SliderAngle("X", &_xWorldRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Y", &_yWorldRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Z", &_zWorldRot, -180.0f, 180.0f);
	}
	ImGui::End();
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
