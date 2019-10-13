#include "..\includes\Cube.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"

Cube::Cube(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& rDist, std::uniform_real_distribution<float>& localRotDelta, std::uniform_real_distribution<float>& worldRotDelta, std::uniform_real_distribution<float>& worldRot) :
	_r(rDist(rng)), _theta(worldRot(rng)), _phi(worldRot(rng)), _chi(worldRot(rng)), _dRoll(localRotDelta(rng)), _dPitch(localRotDelta(rng)), _dYaw(localRotDelta(rng)), _dTheta(worldRotDelta(rng)), _dPhi(worldRotDelta(rng)), _dChi(worldRotDelta(rng))
{
	//If the static instances for the object have already been set up skip, otherwise create them
	if (!IsStaticInitialized())
	{
		//Create vertices and bind to vertex buffer
		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
			} pos;
		};

		const std::vector<Vertex> vertices =
		{
			{-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		//Create and bind the vertex shader
		auto pVs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pVsBc = pVs->GetBytecode();
		AddStaticBind(std::move(pVs));

		//Create and bind the pixel shader
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		//Create and bind the index buffer
		const std::vector<unsigned short> indices =
		{
			0,2,1,	2,3,1,
			1,3,5,	3,7,5,
			2,6,3,	3,6,7,
			4,5,7,	4,7,6,
			0,4,2,	2,4,6,
			0,1,4,	1,5,4
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		//Create pixel Color constant buffer
		struct ColorConstantBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};

		const ColorConstantBuffer ccb =
		{
			{
				{1.0f, 0.0f, 1.0f},
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 1.0f},
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<ColorConstantBuffer>>(gfx, ccb));

		//Create and bind Input layout -- uses bytecode from vertex shader
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVsBc));

		//Create primitive topology
		AddStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	////This gets initialised normally because this needs to be unique per cube, for different transforms
	//Create transform constant buffer bind -- this one slighly special, need a reference to this class to be able to call the transform function and use that data
	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));
}

void Cube::Update(float dt) noexcept
{
	_roll += _dRoll * dt;
	_pitch += _dPitch * dt;
	_yaw += _dYaw * dt;
	_theta += _dTheta * dt;
	_chi += _dChi * dt;
	_phi += _dPhi * dt;
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll) *		//Rotates around the cubes center
		DirectX::XMMatrixTranslation(_r, 0.0f, 0.0f) *						//Translates the cube away from the origin of scene
		DirectX::XMMatrixRotationRollPitchYaw(_theta, _phi, _chi) *			//Rotates cube around the origin of the scene
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);					//Offsets the cube from the camera to give it some space
}
