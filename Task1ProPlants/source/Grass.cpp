#include "..\includes\Grass.h"
#include "ObjExporter.h"
#include "imgui.h"

Grass::Grass(Graphics& gfx, const char* grassTexture) : _baseGrassTexture(grassTexture)
{
	DirectX::XMFLOAT3 zero = {};
	_testBlade = std::make_unique<GrassBlade>(gfx, "Blade 0", grassTexture, zero, zero, zero, zero, zero, zero);
}

Grass::~Grass()
{
}

void Grass::Update(float dt)
{
	_testBlade->Update(dt);
}

void Grass::Draw(Graphics& gfx)
{
	_testBlade->Draw(gfx);
}

void Grass::SpawnImgui(Graphics& gfx)
{
	_testBlade->SpawnImGui(gfx);
}

void Grass::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
