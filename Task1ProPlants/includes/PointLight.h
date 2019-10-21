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
		DirectX::XMFLOAT3 materialColor;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
	
	PointLightConstBuffer _cbData;
	mutable SolidSphere _mesh;
	mutable PixelConstantBuffer<PointLightConstBuffer> _cBuf;
};

