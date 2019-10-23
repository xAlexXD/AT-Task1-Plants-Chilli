#include "TexturedCube.h"
#include "BindableBase.h"
#include "CubePrim.h"
#include <sstream>

TexturedCube::TexturedCube(Graphics& gfx,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3 rot,
	DirectX::XMFLOAT3 posDelta,
	DirectX::XMFLOAT3 rotDelta,
	const char* textureName) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta))
{
	Init(gfx, textureName);
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

void TexturedCube::Init(Graphics& gfx, const char* textureName)
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
			DirectX::XMFLOAT2 tc;
		};
		auto model = CubePrim::MakeIndependentTextured<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model._vertices));

		AddStaticBind(std::make_unique<Sampler>(gfx));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedPhongVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturedPhongPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
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

	//They can have unique textures depending on the file name
	std::ostringstream filepath;
	filepath << "./textures/" << (textureName != nullptr ? textureName : "cubeFlippedVert.tga");
	std::string strFilepath = filepath.str();
	AddBind(std::make_unique<Texture>(gfx, strFilepath.c_str()));
}
