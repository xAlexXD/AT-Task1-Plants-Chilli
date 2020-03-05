#include "GameObjectTransform.h"
#include "imgui.h"
#include <string>

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
	return DirectX::XMLoadFloat3x3(&_modelTransform) *								//Takes the models transform as a modifer
		DirectX::XMMatrixScaling(_xScale, _yScale, _zScale) *						//Scales by scalefactor
		DirectX::XMMatrixRotationRollPitchYaw(_yRot, _zRot, _xRot) *				//Rotates around the cubes center
		DirectX::XMMatrixTranslation(_xPos, _yPos, _zPos);							//Translates the cube in that pos
}

DirectX::XMMATRIX GameObjectTransform::GetTransformWithPivotOffsetXM(DirectX::XMFLOAT3 pivotOffset) const noexcept
{
	return DirectX::XMLoadFloat3x3(&_modelTransform) *
		DirectX::XMMatrixScaling(_xScale, _yScale, _zScale) *
		DirectX::XMMatrixRotationRollPitchYaw(_yRot, -_zRot, _xRot) *
		DirectX::XMMatrixTranslation(pivotOffset.x, pivotOffset.y, pivotOffset.z) *
		DirectX::XMMatrixRotationRollPitchYaw(_yWorldRot, -_zWorldRot, _xWorldRot) *
		DirectX::XMMatrixTranslation(_xPos, _yPos, _zPos);
}

DirectX::XMFLOAT3X3& GameObjectTransform::GetModelTransform() noexcept
{
	return _modelTransform;
}

DirectX::XMFLOAT3 GameObjectTransform::GetPosition() noexcept
{
	return DirectX::XMFLOAT3(_xPos, _yPos, _zPos);
}

DirectX::XMFLOAT3 GameObjectTransform::GetLocalRotation() noexcept
{
	return DirectX::XMFLOAT3(_xRot, _yRot, _zRot);
}

DirectX::XMFLOAT3 GameObjectTransform::GetWorldRotation() noexcept
{
	return DirectX::XMFLOAT3(_xWorldRot, _yWorldRot, _zWorldRot);
}

void GameObjectTransform::SpawnImGuiWindow() noexcept
{
	if (ImGui::Begin("Model Transform Info"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X Pos", &_xPos, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Y Pos", &_yPos, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("Z Pos", &_zPos, -20.0f, 20.0f, "%.1f");

		ImGui::Text("Local Rotation");
		ImGui::SliderAngle("X LRot", &_xRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Y LRot", &_yRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Z LRot", &_zRot, -180.0f, 180.0f);

		ImGui::Text("World Rotation");
		ImGui::SliderAngle("X WRot", &_xWorldRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Y WRot", &_yWorldRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Z WRot", &_zWorldRot, -180.0f, 180.0f);

		if (ImGui::Button("Reset To Zero"))
		{
			ResetToZero();
		}
	}
	ImGui::End();
}

void GameObjectTransform::ResetToZero() noexcept
{
	_xPos = 0.0f;
	_yPos = 0.0f;
	_zPos = 0.0f;
	_xRot = 0.0f;
	_yRot = 0.0f;
	_zRot = 0.0f;
	_xWorldRot = 0.0f;
	_yWorldRot = 0.0f;
	_zWorldRot = 0.0f;
	_xPosDelta = 0.0f;
	_yPosDelta = 0.0f;
	_zPosDelta = 0.0f;
	_xRotDelta = 0.0f;
	_yRotDelta = 0.0f;
	_zRotDelta = 0.0f;
	_xWorldRotDelta = 0.0f;
	_yWorldRotDelta = 0.0f;
	_zWorldRotDelta = 0.0f;
	_xScale = 1.0f;
	_yScale = 1.0f;
	_zScale = 1.0f;
}

void GameObjectTransform::SetPosition(DirectX::XMFLOAT3 pos) noexcept
{
	_xPos = pos.x;
	_yPos = pos.y;
	_zPos = pos.z;
}

void GameObjectTransform::SetLocalRotation(DirectX::XMFLOAT3 rot) noexcept
{
	_xRot = rot.x;
	_yRot = rot.y;
	_zRot = rot.z;
}

void GameObjectTransform::SetPositionDelta(DirectX::XMFLOAT3 posDelta) noexcept
{
	_xPosDelta = posDelta.x;
	_yPosDelta = posDelta.y;
	_zPosDelta = posDelta.z;
}

void GameObjectTransform::SetLocalRotationDelta(DirectX::XMFLOAT3 rotDelta) noexcept
{
	_xRotDelta = rotDelta.x;
	_yRotDelta = rotDelta.y;
	_zRotDelta = rotDelta.z;
}

void GameObjectTransform::SetWorldRotation(DirectX::XMFLOAT3 rot) noexcept
{
	_xWorldRot = rot.x;
	_yWorldRot = rot.y;
	_zWorldRot = rot.z;
}

void GameObjectTransform::SetScale(DirectX::XMFLOAT3 scaleFactor) noexcept
{
	_xScale = scaleFactor.x;
	_yScale = scaleFactor.y;
	_zScale = scaleFactor.z;
}
