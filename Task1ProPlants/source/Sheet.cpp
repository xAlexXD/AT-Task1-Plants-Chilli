#include "..\includes\Sheet.h"
#include "BindableBase.h"
#include "PlanePrim.h"

Sheet::Sheet(Graphics& gfx, 
	std::mt19937& rng, 
	std::uniform_real_distribution<float>& rDist, 
	std::uniform_real_distribution<float>& localRotDelta, 
	std::uniform_real_distribution<float>& worldRotDelta, 
	std::uniform_real_distribution<float>& worldRot) :
	_transform(std::make_unique<GameObjectTransform>(rng, rDist, localRotDelta, worldRotDelta, worldRot))
{
	Init(gfx);
}

Sheet::Sheet(Graphics& gfx) : _transform(std::make_unique<GameObjectTransform>())
{
	Init(gfx);
}

void Sheet::Init(Graphics& gfx)
{
	if (!IsStaticInitialized())
	{
		//Vertex buffer with struct for tex uvs
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tex;
		};

		auto model = PlanePrim::Make<Vertex>();
		model._vertices[0].tex = { 0.0f, 0.0f };
		model._vertices[1].tex = { 1.0f, 0.0f };
		model._vertices[2].tex = { 0.0f, 1.0f };
		model._vertices[3].tex = { 1.0f, 1.0f };

		AddStaticBind(std::make_unique<Texture>(gfx, "./textures/sheb.tga"));
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model._vertices));
		AddStaticBind(std::make_unique<Sampler>(gfx));

		//Create and bind the vertex shader
		auto pVs = std::make_unique<VertexShader>(gfx, L"TexturedVertexShader.cso");
		auto pVsBc = pVs->GetBytecode();
		AddStaticBind(std::move(pVs));

		//Create and bind the pixel shader
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturedPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

		//Create and bind Input layout -- uses bytecode from vertex shader
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
	DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f));
}

void Sheet::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	return _transform->GetGameObjectTransformXM();
}
