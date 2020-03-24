#pragma once
#include "PlantBase.h"
#include "Bush.h"

class Bushes : public PlantBase
{
public:
	Bushes() = delete;
	Bushes(Graphics& gfx, const char* planeTexture);
	~Bushes() override;

	void Update(float dt) override;
	void Draw(Graphics& gfx) override;
	void SpawnImgui(Graphics& gfx) override;
	void Export(Graphics& gfx, ObjExporter& exporter, const char* outDir, const char* outName) override;

private:
	const char* _basePlaneTexture;
	std::vector<std::unique_ptr<Bush>> _bushVector;
};

