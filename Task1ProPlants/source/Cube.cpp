#include "..\includes\Cube.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "CubePrim.h"

Cube::Cube(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& rDist,
	std::uniform_real_distribution<float>& localRotDelta,
	std::uniform_real_distribution<float>& worldRotDelta,
	std::uniform_real_distribution<float>& worldRot) :
	_transform(std::make_unique<GameObjectTransform>(rng, rDist, localRotDelta, worldRotDelta, worldRot))
{
	//If the static instances for the object have already been set up skip, otherwise create them
	if (!IsStaticInitialized())
	{
		//Create vertices and indices and bind to their respective buffers
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};

		auto model = CubePrim::Make<Vertex>();
		//READD LINE BELOW IF YOU WANT TO DO DEFORMATIONS ON A BASE OBJECT TO APPLY TO ALL INSTACES
		//model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model._vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

		//Create and bind the vertex shader
		auto pVs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pVsBc = pVs->GetBytecode();
		AddStaticBind(std::move(pVs));

		//Create and bind the pixel shader
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		//Create pixel Color constant buffer
		struct ColorConstantBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
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
				{1.0f, 1.0f, 1.0f},
				{0.0f, 0.0f, 0.0f},
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

	//Adjust this to change the per instance scaling
	DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
}

void Cube::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
 	return _transform->GetGameObjectTransformXM();
}
