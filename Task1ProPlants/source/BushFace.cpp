#include "..\includes\BushFace.h"
#include <sstream>
#include "BindableBase.h"
#include "SemiCirclePrim.h"

BushFace::BushFace(Graphics& gfx, const char* textureName, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta, DirectX::XMFLOAT3 worldRot, DirectX::XMFLOAT3 worldDelta) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta))
{
	//Grab the base form of the model and apply some starting transforms to get it how we want the initial object to be
	auto model = SemiCirclePrim::MakeTextured<TexturedVertex>();
	model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	model.SetNormalsIndependentFlat();
	//Stupid normal fix TODO double the verts and fix the normals for the backside
	DirectX::XMFLOAT3 oldNormFront = model._vertices[2].n;
	DirectX::XMFLOAT3 oldNormBack = model._vertices[8].n;
	model._vertices[2].n = DirectX::XMFLOAT3(oldNormFront.x * -1, oldNormFront.y * -1, oldNormFront.z * -1);
	model._vertices[8].n = DirectX::XMFLOAT3(oldNormBack.x * -1, oldNormBack.y * -1, oldNormBack.z * -1);

	_vertCount = model._vertices.size();
	_indCount = model._indices.size();

	_vertOut.reserve(_vertCount);
	_indexOut.reserve(_indCount);

	//Cache the inds
	for (const auto& index : model._indices)
	{
		_indexOut.push_back(index);
	}

	AddBind(std::make_unique<DynamicVertexBuffer<TexturedVertex>>(gfx, model._vertices));
	_vertexBuffer = reinterpret_cast<DynamicVertexBuffer<TexturedVertex>*>(GetPointerToLastBindable());

	std::ostringstream path;
	path << "./textures/" << textureName;

	AddBind(std::make_unique<Texture>(gfx, path.str().c_str()));
	_textureBindable = dynamic_cast<Texture*>(GetPointerToLastBindable());
	AddBind(std::make_unique<Sampler>(gfx));

	auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedPhongVertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"TexturedPhongPixelShader.cso"));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//Create transform constant buffer bind -- this one slighly special, need a reference to this class to be able to call the transform function and use that data
	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));

	//Adjust this to change the per instance scaling
	//DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(0.9f, 1.5f, 0.9f));
}

void BushFace::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX BushFace::GetTransformXM() const noexcept
{
	return _transform->GetTransformWithPivotOffsetXM(_pivotPos);
}

GameObjectTransform* BushFace::GetFaceTransform() noexcept
{
	return _transform.get();
}

void BushFace::UpdateOutVertices(Graphics& gfx)
{
	//Get buffer from dynamic vertex buffer
	std::vector<TexturedVertex> vert = _vertexBuffer->GetOriginalVerts();
	_vertOut.clear();
	_vertOut.reserve(vert.size());
	for (size_t i = 0; i < vert.size(); i++)
	{
		_vertOut.push_back(vert[i]);
	}

	const auto modelView = GetTransformXM() * gfx.GetCamera();
	const auto modelViewProj = modelView * gfx.GetProjection();

	//Apply the transforms the vertex shader would
	for (auto& vertex : _vertOut)
	{
		//Multiply the positions by the model view projection
		DirectX::XMFLOAT4 temp = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f };
		auto tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelViewProj);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.pos.x = -temp.x;
		vertex.pos.y = temp.y;
		vertex.pos.z = temp.z;

		//Adjust the normals based on the modelView
		temp = { vertex.n.x, vertex.n.y, vertex.n.z, 1.0f };
		tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelView);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.n.x = temp.x;
		vertex.n.y = temp.y;
		vertex.n.z = temp.z;
	}
}

void BushFace::UpdateTexture(Graphics& gfx, const char* filePath)
{
	_textureBindable->ChangeTextureView(gfx, filePath);
}
