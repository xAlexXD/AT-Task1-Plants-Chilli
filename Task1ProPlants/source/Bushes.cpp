#include "..\includes\Bushes.h"
#include "ObjExporter.h"
#include "imgui.h"

Bushes::Bushes(Graphics& gfx, const char* planeTexture) : _basePlaneTexture(planeTexture)
{
}

Bushes::~Bushes()
{
}

void Bushes::Update(float dt)
{
}

void Bushes::Draw(Graphics& gfx)
{
}

void Bushes::SpawnImgui(Graphics& gfx)
{
	ImGui::Text("Bushes Selected!");
}

void Bushes::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
