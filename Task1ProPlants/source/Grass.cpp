#include "..\includes\Grass.h"
#include "ObjExporter.h"
#include "imgui.h"

Grass::Grass(Graphics& gfx, const char* grassTexture) : _baseGrassTexture(grassTexture)
{
}

Grass::~Grass()
{
}

void Grass::Update(float dt)
{
}

void Grass::Draw(Graphics& gfx)
{
}

void Grass::SpawnImgui(Graphics& gfx)
{
	ImGui::Text("Grass Selected!");
}

void Grass::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
