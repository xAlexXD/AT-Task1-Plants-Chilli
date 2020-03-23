#pragma once
#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	DirectX::XMMATRIX GetExportMatrix() const noexcept;
	void SpawnImguiControlWindow() noexcept;
	void Reset() noexcept;

private:
	float r = 6.5f;
	float theta = -0.0f;
	float phi = -1.0472f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};

