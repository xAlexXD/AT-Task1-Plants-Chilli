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
		0.045f,						//Attenuation lin
		0.0075f						//Attenuation Quad
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
