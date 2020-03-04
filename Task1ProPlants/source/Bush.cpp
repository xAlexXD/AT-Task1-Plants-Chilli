#include "..\includes\Bush.h"
#include "ObjExporter.h"
#include "imgui.h"

Bush::Bush(Graphics& gfx, const char* planeTexture) : _basePlaneTexture(planeTexture)
{
}

Bush::~Bush()
{
}

void Bush::Update(float dt)
{
}

void Bush::Draw(Graphics& gfx)
{
}

void Bush::SpawnImgui(Graphics& gfx)
{
	ImGui::Text("Bush Selected!");
}

void Bush::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
