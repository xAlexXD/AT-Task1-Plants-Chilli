#include "GameObjectTransform.h"

GameObjectTransform::GameObjectTransform(std::mt19937& rng, std::uniform_real_distribution<float>& rDist, std::uniform_real_distribution<float>& localRotDelta, std::uniform_real_distribution<float>& worldRotDelta, std::uniform_real_distribution<float>& worldRot) :
	_r(rDist(rng)), _theta(worldRot(rng)), _phi(worldRot(rng)), _chi(worldRot(rng)), _dRoll(localRotDelta(rng)), _dPitch(localRotDelta(rng)), _dYaw(localRotDelta(rng)), _dTheta(worldRotDelta(rng)), _dPhi(worldRotDelta(rng)), _dChi(worldRotDelta(rng))
{
}

GameObjectTransform::~GameObjectTransform()
{
}

void GameObjectTransform::Update(float dt) noexcept
{
	_roll += _dRoll * dt;
	_pitch += _dPitch * dt;
	_yaw += _dYaw * dt;
	_theta += _dTheta * dt;
	_chi += _dChi * dt;
	_phi += _dPhi * dt;
}

DirectX::XMMATRIX GameObjectTransform::GetGameObjectTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&_modelTransform) *						//Takes the models transform as a modifer
		DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll) *		//Rotates around the cubes center
		DirectX::XMMatrixTranslation(_r, 0.0f, 0.0f) *						//Translates the cube away from the origin of scene
		DirectX::XMMatrixRotationRollPitchYaw(_theta, _phi, _chi) *			//Rotates cube around the origin of the scene
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);					//Offsets the cube from the camera to give it some space
}

DirectX::XMFLOAT3X3& GameObjectTransform::GetModelTransform() noexcept
{
	return _modelTransform;
}
