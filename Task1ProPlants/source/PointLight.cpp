#include "..\includes\PointLight.h"
#include "imgui.h"

PointLight::PointLight(Graphics& gfx, float sphereRadius) : 
	_mesh(gfx, sphereRadius), _cBuf(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	ImGui::Indent();
	ImGui::Text("Position:");
	ImGui::SliderFloat("X##Light", &_cbData.pos.x, -60.0f, 60.0f, "%.1f");
	ImGui::SliderFloat("Y##Light", &_cbData.pos.y, -60.0f, 60.0f, "%.1f");
	ImGui::SliderFloat("Z##Light", &_cbData.pos.z, -60.0f, 60.0f, "%.1f");

	ImGui::Text("Intensity/Colour:");
	ImGui::SliderFloat("Intensity##Light", &_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
	ImGui::ColorEdit3("Diffuse Colour##Light", &_cbData.diffuseColor.x);
	ImGui::ColorEdit3("Ambient Colour##Light", &_cbData.ambient.x);

	ImGui::Text("Falloff:");
	ImGui::SliderFloat("Const##Light", &_cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
	ImGui::SliderFloat("Lin##Light", &_cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
	ImGui::SliderFloat("Quad##Light", &_cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

	if (ImGui::Button("Reset Light##Light"))
	{
		Reset();
	}
	ImGui::Unindent();
}

void PointLight::Reset() noexcept
{
	_cbData =
	{
		{ 0.0f, -9.0f, -9.0f },		//Position
		{ 0.05f, 0.05f, 0.05f },	//Ambient
		{ 1.0f, 1.0f, 1.0f },		//diffuseCol
		2.0f,						//diffuseIntensity
		0.5f,						//Attenuation const
		0.035f,						//Attenuation linear
		0.0045f						//Attenuation Quadratic
	};
}

void PointLight::Draw(Graphics& gfx) const
{
	_mesh.SetPos(_cbData.pos);
	_mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = _cbData;
	const auto pos = DirectX::XMLoadFloat3(&_cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	_cBuf.Update(gfx, dataCopy);
	_cBuf.Bind(gfx);
}
