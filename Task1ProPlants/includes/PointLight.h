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
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;

private:
	//Allign is added because the shader expects float 3s in 16 bytes with the last value aka W in the xyzw as 0;
	//Since we packed together multiple float 3s back to back they need to be spaced out as 16 bytes so this error doesnt throw shader side
	struct PointLightConstBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
	
	PointLightConstBuffer _cbData;
	mutable SolidSphere _mesh;
	mutable PixelConstantBuffer<PointLightConstBuffer> _cBuf;
};

