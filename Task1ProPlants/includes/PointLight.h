#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffer.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float sphereRadius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx) const noexcept;

private:
	struct PointLightConstBuffer
	{
		DirectX::XMFLOAT3 pos;
		float padding;			////Padding to make it a float4 for simd transformations
	};
	
	DirectX::XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f };
	mutable SolidSphere _mesh;
	mutable PixelConstantBuffer<PointLightConstBuffer> _cBuf;
};

