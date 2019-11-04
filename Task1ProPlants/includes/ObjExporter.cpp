#include "ObjExporter.h"
#include <fstream>
#include <sstream>

void ObjExporter::ExportToObj(const char* exportName, std::vector<std::vector<TexturedVertex>> vecVertVec, std::vector<std::vector<int>> vecIndVec, std::vector<std::string> texNames) noexcept
{
	//Set up names of the textures without the extension
	std::vector<std::string> texNamesNoExtension(texNames.size(), "");
	for (size_t i = 0; i < texNames.size(); i++)
	{
		texNamesNoExtension[i] = texNames[i].substr(0u, texNames[i].length() - 4);
	}

	//Open a file stream to output to obj

	//the size of all 3 are going to be the same so start a for loop with the size of one of them
		//First check if there are any verts stored in this vec, if so continue, else break out
			//Tag the name of the Vert struct here with the tex name at the same index
			//Loop and output the first vecVertVec v vn and vt
			//Take the inds at the appropriate index and add 1 to all inds as obj start at 1 not 0
			//Use this modified ind array to build f, remembering to also put back faces on 
	//Once the loop is finished for all ones in the vector, just finish by closing the file for now
	//Open another file for the mtl creating the material using the texNames as names for the mats and using the tex names to define texs in the mats

	std::ostringstream mtlFilePath;
	mtlFilePath << "./exports/" << exportName << ".mtl";
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

	/*
	newmtl Textured
   Ka 1.000 1.000 1.000
   Kd 1.000 1.000 1.000
   Ks 0.000 0.000 0.000
   d 1.0
   illum 2
   map_Ka lemur.tga           # the ambient texture map
   map_Kd lemur.tga           # the diffuse texture map (most of the time, it will
	*/

	//std::ofstream outFile("./exports/Flower.obj");
	//if (outFile.is_open())
	//{
	//	//Start with verts
	//	outFile << "# Vertices: \n";
	//	for (auto& v : verts)
	//	{
	//		outFile << "v" << " " << v.pos.x << " " << v.pos.y << " " << v.pos.z << "\n";
	//	}

	//	//Next the Tex cords
	//	outFile << "# Tex Cords: \n";
	//	for (auto& tc : verts)
	//	{
	//		outFile << "vt" << " " << tc.tc.x << " " << -tc.tc.y << "\n";
	//	}

	//	//Next the normals
	//	outFile << "# Normals: \n";
	//	for (auto& n : verts)
	//	{
	//		outFile << "vn" << " " << n.n.x << " " << n.n.y << " " << n.n.z << " \n";
	//	}

	//	//Finally the faces For some reason it starts at 1 instead of zero
	//	outFile << "# Faces: \n";
	//	std::vector<int> objModifedInds;
	//	objModifedInds.reserve(inds.size());
	//	
	//	//Obj inds start at 1 instead of zero
	//	for (auto& index : inds)
	//	{
	//		objModifedInds.push_back(index + 1);
	//	}

	//	//Add extra triangles to draw the backfaces too
	//	for (size_t i = 0; i < objModifedInds.size(); i+=3)
	//	{
	//		outFile << "f" << " "
	//			<< objModifedInds[i] << "/" << objModifedInds[i] << "/" << objModifedInds[i] << " "
	//			<< objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << " "
	//			<< objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "\n";

	//		//outFile << "f" << " "
	//		//	<< objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << " "
	//		//	<< objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << " "
	//		//	<< objModifedInds[i] << "/" << objModifedInds[i] << "/" << objModifedInds[i] << "\n";
	//	}

	//	outFile.close();
	//}
}