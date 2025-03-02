#include "..\includes\Camera.h"
#include "imgui.h"

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	//First transform offsets camera away from origin, then rotates around the origin in pitch and yaw
	const auto pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);

	//Applies the local rotation to the world rotation calc'd previously
	return DirectX::XMMatrixLookAtLH(
		pos, DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * DirectX::XMMatrixRotationRollPitchYaw(
		pitch, -yaw, roll
	);
}

DirectX::XMMATRIX Camera::GetExportMatrix() const noexcept
{
	//First transform offsets camera away from origin, then rotates around the origin in pitch and yaw
	const auto pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(-1.570796326794897f, 0.0f, 0.0f)
	);

	//Applies the local rotation to the world rotation calc'd previously
	return DirectX::XMMatrixLookAtLH(
		pos, DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * DirectX::XMMatrixRotationRollPitchYaw(
		0.0f, 0.0f, 0.0f
	);
}

void Camera::SpawnImguiControlWindow() noexcept
{
	ImGui::Indent();
	ImGui::Text("Position:");
	ImGui::SliderFloat("R##Camera", &r, 0.1f, 80.0f, "%.1f");
	ImGui::SliderAngle("Theta##Camera", &theta, -180.0f, 180.0f);
	ImGui::SliderAngle("Phi##Camera", &phi, -89.0f, 89.0f);
	ImGui::Text("Orientation:");
	ImGui::SliderAngle("Roll##Camera", &roll, -180.0f, 180.0f);
	ImGui::SliderAngle("Pitch##Camera", &pitch, -180.0f, 180.0f);
	ImGui::SliderAngle("Yaw##Camera", &yaw, -180.0f, 180.0f);
	if (ImGui::Button("Reset Camera##Camera"))
	{
		Reset();
	}
	ImGui::Unindent();
}

void Camera::Reset() noexcept
{
	r = 6.5f;
	theta = 0.0f;
	phi = -1.0472f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}
