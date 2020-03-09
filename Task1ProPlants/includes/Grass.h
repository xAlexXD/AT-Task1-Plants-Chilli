#pragma once
#include "PlantBase.h"
#include "GrassBlade.h"

class Grass : public PlantBase
{
public:
	Grass() = delete;
	Grass(Graphics& gfx, const char* grassTexture);
	~Grass() override;

	void Update(float dt) override;
	void Draw(Graphics& gfx) override;
	void SpawnImgui(Graphics& gfx) override;
	void Export(Graphics& gfx, ObjExporter& exporter, const char* outDir) override;

private:
	const char* _baseGrassTexture;
	std::vector<std::unique_ptr<GrassBlade>> _bladeVector;
};

