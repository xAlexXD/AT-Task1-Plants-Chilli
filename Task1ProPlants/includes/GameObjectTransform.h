#pragma once
#include <DirectXMath.h>
#include <random>

class GameObjectTransform 
{
public:
	GameObjectTransform();
	GameObjectTransform(const GameObjectTransform&) = delete;
	GameObjectTransform& operator=(const GameObjectTransform&) = delete;
	
	GameObjectTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta);
	~GameObjectTransform();

	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetGameObjectTransformXM() const noexcept;
	DirectX::XMFLOAT3X3& GetModelTransform() noexcept;

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetLocalRotation(DirectX::XMFLOAT3 rot);
	void SetPositionDelta(DirectX::XMFLOAT3 posDelta);
	void SetLocalRotationDelta(DirectX::XMFLOAT3 rotDelta);

private:
	//Model transformation, used for scaling
	DirectX::XMFLOAT3X3 _modelTransform;

	////Positional values
	float _xPos = 0.0f;
	float _yPos = 0.0f;
	float _zPos = 0.0f;
	//Local rotations
	float _xRot = 0.0f;
	float _yRot = 0.0f;
	float _zRot = 0.0f;

	////Speed (delta/s)
	float _xPosDelta = 0.0f;
	float _yPosDelta = 0.0f;
	float _zPosDelta = 0.0f;
	float _xRotDelta = 0.0f;
	float _yRotDelta = 0.0f;
	float _zRotDelta = 0.0f;
};

