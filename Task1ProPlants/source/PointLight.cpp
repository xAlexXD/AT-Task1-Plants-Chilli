#include "..\includes\PointLight.h"
#include "imgui.h"

PointLight::PointLight(Graphics& gfx, float sphereRadius) : 
	_mesh(gfx, sphereRadius), _cBuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &_pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &_pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &_pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	_pos = { 0.0f, 0.0f, 0.0f };
}

void PointLight::Draw(Graphics& gfx) const
{
	_mesh.SetPos(_pos);
	_mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	const PointLightConstBuffer buf = { _pos };
	_cBuf.Update(gfx, buf);
	_cBuf.Bind(gfx);
}
