#include <sstream>
#include <vector>
#include <stdio.h>
#include "StructDefs.h"

class ObjExporter
{
public:
	ObjExporter() = default;
	~ObjExporter() = default;


	void ExportToObj(const char* exportName, const char* outDir, std::vector<std::vector<TexturedVertex>> vecVertVec, std::vector<std::vector<int>> vecIndVec, std::vector<std::string> texNames) noexcept;

private:
};