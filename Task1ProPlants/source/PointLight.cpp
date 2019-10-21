#include "..\includes\PointLight.h"
#include "imgui.h"

PointLight::PointLight(Graphics& gfx, float sphereRadius) : 
	_mesh(gfx, sphereRadius), _cBuf(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &_cbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &_cbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &_cbData.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Colour");
		ImGui::SliderFloat("Intensity", &_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Colour", &_cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient Colour", &_cbData.ambient.x);
		ImGui::ColorEdit3("Material Colour", &_cbData.materialColor.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &_cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &_cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &_cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	_cbData =
	{
		{ 0.0f, 0.0f, 0.0f },		//Position
		{ 0.7f, 0.7f, 0.9f },		//Mat col
		{ 0.05f, 0.05f, 0.05f },	//Ambient
		{ 1.0f, 1.0f, 1.0f },		//diffuseCol
		1.0f,						//diffuseIntensity
		1.0f,						//Attenuation const
		0.045f,						//Attenuation linear
		0.0075f						//Attenuation Quadratic
	};
}

void PointLight::Draw(Graphics& gfx) const
{
	_mesh.SetPos(_cbData.pos);
	_mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	_cBuf.Update(gfx, _cbData);
	_cBuf.Bind(gfx);
}
