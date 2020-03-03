#include "ObjExporter.h"
#include <fstream>
#include <sstream>

void ObjExporter::ExportToObj(const char* exportName, const char* outDir, std::vector<std::vector<TexturedVertex>> vecVertVec, std::vector<std::vector<int>> vecIndVec, std::vector<std::string> texNames) noexcept
{
	//Set up names of the textures without the extension
	std::vector<std::string> texNamesNoExtension(texNames.size(), "");
	for (size_t i = 0; i < texNames.size(); i++)
	{
		texNamesNoExtension[i] = texNames[i].substr(0u, texNames[i].length() - 4);
	}

	//Open a file stream to output to obj
	std::ostringstream objFilePath;
	objFilePath << outDir << "/" << exportName << ".obj";
	std::ofstream outFileObj(objFilePath.str().c_str());
	if (outFileObj.is_open())
	{
		//Loop over all the vectors of verts adding their verts to the file, keeping track of the offset
		int currentIndOffset = 0;

		//Add the header to include the mtl file with this obj
		outFileObj << "mtllib " << exportName << ".mtl\n\n";

		for (size_t i = 0; i < vecVertVec.size(); i++)
		{
			if (vecVertVec[i].size() > 0)
			{
				outFileObj << "# Object: " << texNamesNoExtension[i] << "\n";
				outFileObj << "o " << texNamesNoExtension[i] << "\n";

				//Start with verts
				outFileObj << "# Vertices: \n";
				for (int j = 0; j < vecVertVec[i].size(); j++)
				{
					outFileObj << "v" << " " << vecVertVec[i][j].pos.x << " " << vecVertVec[i][j].pos.y << " " << vecVertVec[i][j].pos.z << "\n";
				}

				outFileObj << "\n";
				
				//Next the Tex cords
				outFileObj << "# Tex Cords: \n";
				for (int j = 0; j < vecVertVec[i].size(); j++)
				{
					outFileObj << "vt" << " " << vecVertVec[i][j].tc.x << " " << -vecVertVec[i][j].tc.y << "\n";
				}

				outFileObj << "\n";
				
				//Next the normals
				outFileObj << "# Normals: \n";
				for (int j = 0; j < vecVertVec[i].size(); j++)
				{
					outFileObj << "vn" << " " << vecVertVec[i][j].n.x << " " << vecVertVec[i][j].n.y << " " << vecVertVec[i][j].n.z << " \n";
				}

				outFileObj << "\n";
				
				outFileObj << "g " << texNamesNoExtension[i] << "\n";
				outFileObj << "usemtl " << texNamesNoExtension[i] << "\n";
				//Finally the faces For some reason it starts at 1 instead of zero so add 1 to each ind
				outFileObj << "# Faces: \n";
				for (int j = 0; j < vecIndVec[i].size(); j+=3)
				{
					outFileObj << "f" << " "
						<< (vecIndVec[i][j] + 1) << "/" << (vecIndVec[i][j] + 1) << "/" << (vecIndVec[i][j] + 1) << " "
						<< (vecIndVec[i][j + 1] + 1) << "/" << (vecIndVec[i][j + 1] + 1) << "/" << (vecIndVec[i][j + 1] + 1) << " "
						<< (vecIndVec[i][j + 2] + 1) << "/" << (vecIndVec[i][j + 2] + 1) << "/" << (vecIndVec[i][j + 2] + 1) << "\n";
				
					//Add extra triangles to draw the backfaces too
					outFileObj << "f" << " "
						<< (vecIndVec[i][j + 2] + 1) << "/" << (vecIndVec[i][j + 2] + 1) << "/" << (vecIndVec[i][j + 2] + 1) << " "
						<< (vecIndVec[i][j + 1] + 1) << "/" << (vecIndVec[i][j + 1] + 1) << "/" << (vecIndVec[i][j + 1] + 1) << " "
						<< (vecIndVec[i][j] + 1) << "/" << (vecIndVec[i][j] + 1) << "/" << (vecIndVec[i][j] + 1) << "\n";
				}

				outFileObj << "\n";

				currentIndOffset += vecVertVec[i].size();
			}
		}
	}
	outFileObj.close();

	std::ostringstream mtlFilePath;
	mtlFilePath << outDir << "/" << exportName << ".mtl";
	std::ofstream outFileMtl(mtlFilePath.str().c_str());
	if (outFileMtl.is_open())
	{
		for (int i = 0; i < texNames.size(); i++)
		{
			//first define the name of the material
			outFileMtl << "newmtl " << texNamesNoExtension[i] << "\n";
			//Next the ambient of the material
			outFileMtl << "Ka " << "1.000 " << "1.000 " << "1.000\n";
			//Diffuse of material
			outFileMtl << "Kd " << "1.000 " << "1.000 " << "1.000\n";
			//Specular of material
			outFileMtl << "Ks " << "0.000 " << "0.000 " << "0.000\n";
			//Dissolve
			outFileMtl << "d 1.0\n";
			//Illumination settings
			outFileMtl << "illum 2\n";
			//Amibient texturemap
			outFileMtl << "map_Ka " << texNames[i] << "\n";
			//diffuse texturemap
			outFileMtl << "map_Kd " << texNames[i] << "\n";
			outFileMtl << "\n";
		}
	}
	outFileMtl.close();
}