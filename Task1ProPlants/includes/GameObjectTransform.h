#pragma once
#include <DirectXMath.h>
#include <random>

class GameObjectTransform 
{
public:
	GameObjectTransform() = delete;
	GameObjectTransform(const GameObjectTransform&) = delete;
	GameObjectTransform& operator=(const GameObjectTransform&) = delete;
	
	GameObjectTransform(std::mt19937& rng,
		std::uniform_real_distribution<float>& rDist,
		std::uniform_real_distribution<float>& localRotDelta,
		std::uniform_real_distribution<float>& worldRotDelta,
		std::uniform_real_distribution<float>& worldRot
	);
	~GameObjectTransform();

	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetGameObjectTransformXM() const noexcept;

	DirectX::XMFLOAT3X3 GetModelTransform() noexcept;

private:
	//Model transformation, used for scaling
	DirectX::XMFLOAT3X3 _modelTransform;

	////Positional values
	float _r;
	//Local rotations
	float _roll = 0.0f;			//Rotation along the x axis, where Z axis is down
	float _pitch = 0.0f;		//Rotation along the y axis, where Z axis is down
	float _yaw = 0.0f;			//Rotation along the z axis, where Z axis is down
	//World space rotations
	float _theta = 0.0f;		//Equiv to pitch but in world space
	float _phi = 0.0f;			//Equiv to yaw but in world space
	float _chi = 0.0f;			//Equiv to roll but in world space

	////Speed (delta/s)
	float _dRoll;
	float _dPitch;
	float _dYaw;
	float _dTheta;
	float _dPhi;
	float _dChi;
};

