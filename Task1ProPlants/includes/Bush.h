#pragma once
#include "PlantBase.h"
#include "Sheet.h"

class Bush : public PlantBase
{
public:
	Bush() = delete;
	Bush(Graphics& gfx, const char* planeTexture);
	~Bush() override;

	void Update(float dt) override;
	void Draw(Graphics& gfx) override;
	void SpawnImgui(Graphics& gfx) override;
	void Export(Graphics& gfx, ObjExporter& exporter, const char* outDir) override;

private:
	const char* _basePlaneTexture;
};

