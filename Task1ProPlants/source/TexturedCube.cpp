#include "TexturedCube.h"
#include "BindableBase.h"
#include "CubePrim.h"

TexturedCube::TexturedCube(Graphics& gfx,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3 rot,
	DirectX::XMFLOAT3 posDelta,
	DirectX::XMFLOAT3 rotDelta) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta))
{
	Init(gfx);
}

TexturedCube::TexturedCube(Graphics& gfx) : _transform(std::make_unique<GameObjectTransform>())
{
	Init(gfx);
}

void TexturedCube::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX TexturedCube::GetTransformXM() const noexcept
{
	return _transform->GetGameObjectTransformXM();
}

void TexturedCube::Init(Graphics& gfx)
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

		auto model = CubePrim::MakeSkinned<Vertex>();

		AddStaticBind(std::make_unique<Texture>(gfx, "./textures/cubeFlippedVert.tga"));
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
