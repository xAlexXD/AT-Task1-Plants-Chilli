#include "..\includes\Bushes.h"
#include "ObjExporter.h"
#include "imgui.h"

Bushes::Bushes(Graphics& gfx, const char* planeTexture) : _basePlaneTexture(planeTexture)
{
	DirectX::XMFLOAT3 zero = {};
	_testBush = std::make_unique<Bush>(gfx, 4u, planeTexture, "TestuBush");
}

Bushes::~Bushes()
{
}

void Bushes::Update(float dt)
{
	_testBush->Update(dt);
}

void Bushes::Draw(Graphics& gfx)
{
	_testBush->Draw(gfx);
}

void Bushes::SpawnImgui(Graphics& gfx)
{
	_testBush->SpawnImGui(gfx);
}

void Bushes::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
