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
	GameObjectTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta, DirectX::XMFLOAT3 worldRot, DirectX::XMFLOAT3 worldDelta);
	~GameObjectTransform();

	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMMATRIX GetTransformWithPivotOffsetXM(DirectX::XMFLOAT3 pivotOffset) const noexcept;
	DirectX::XMFLOAT3X3& GetModelTransform() noexcept;
	DirectX::XMFLOAT3 GetPosition() noexcept;
	
	void SpawnImGuiWindow() noexcept;

	void SetPosition(DirectX::XMFLOAT3 pos) noexcept;
	void SetLocalRotation(DirectX::XMFLOAT3 rot) noexcept;
	void SetPositionDelta(DirectX::XMFLOAT3 posDelta) noexcept;
	void SetLocalRotationDelta(DirectX::XMFLOAT3 rotDelta) noexcept;
	void SetWorldRotation(DirectX::XMFLOAT3 rot) noexcept;

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
	//World rotations
	float _xWorldRot = 0.0f;
	float _yWorldRot = 0.0f;
	float _zWorldRot = 0.0f;
	////Speed (delta/s)
	float _xPosDelta = 0.0f;
	float _yPosDelta = 0.0f;
	float _zPosDelta = 0.0f;
	float _xRotDelta = 0.0f;
	float _yRotDelta = 0.0f;
	float _zRotDelta = 0.0f;
	float _xWorldRotDelta = 0.0f;
	float _yWorldRotDelta = 0.0f;
	float _zWorldRotDelta = 0.0f;
};

