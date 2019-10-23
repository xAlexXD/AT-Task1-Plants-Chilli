#include "..\includes\Pyramid.h"
#include <array>
#include "BindableBase.h"
#include "PrismPrim.h"

Pyramid::Pyramid(Graphics& gfx, 
	DirectX::XMFLOAT3 pos, 
	DirectX::XMFLOAT3 rot, 
	DirectX::XMFLOAT3 posDelta, 
	DirectX::XMFLOAT3 rotDelta, 
	DirectX::XMFLOAT3 matCol, 
	int tessalation) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta))
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
			std::array<char, 4> color;
			char padding;
		};
		auto model = PrismPrim::MakeTesselatedIndependentFaces<Vertex>(tessalation);
		// set vertex colors for mesh
		for (auto& v : model._vertices)
		{
			v.color = { (char)40,(char)40,(char)255 };
		}
		model._vertices.front().color = { (char)255,(char)20,(char)20 }; // very first vertex is the cone tip
		// squash mesh a bit in the z direction
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 0.7f));
		// add normals
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model._vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"BlendedPhongVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"BlendedPhongPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));
}

void Pyramid::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
	return _transform->GetGameObjectTransformXM();
}
