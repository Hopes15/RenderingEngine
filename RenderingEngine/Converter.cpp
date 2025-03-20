#include "Converter.h"
#include <cassert>
#include <algorithm>
#include <fstream>

void Converter::ConvertFBX(const char* fbxFile, const char* textFile, float fx, float fy, float fz, int idx0, int idx1, int idx2)
{
	//Create Manager
	FbxManager* manager = fbxsdk::FbxManager::Create();
	assert(manager != nullptr);

	//Create Importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	assert(importer != nullptr);

	//Set File to Importer
	bool result = importer->Initialize(fbxFile);
	assert(result == true);

	//Create Scene
	FbxScene* scene = FbxScene::Create(manager, "");
	assert(scene != nullptr);

	//Import
	result = importer->Import(scene);
	assert(result == true);
	importer->Destroy();

	//Prepare Converter
	FbxGeometryConverter converter(manager);
	converter.SplitMeshesPerMaterial(scene, true);
	//Triangulate Porygons
	converter.Triangulate(scene, true);

	//Load Materials
	int numMaterials = scene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < numMaterials; i++)
	{
		LoadMaterial(scene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	//Load Mesh per Parts
	mIdx0 = idx0; mIdx1 = idx1; mIdx2 = idx2;
	NumParts = scene->GetSrcObjectCount<FbxMesh>();
	m_pParts = new Parts[NumParts];
	for (int k = 0; k < NumParts; k++)
	{
		auto* mesh = scene->GetSrcObject<FbxMesh>(k);
		LoadPosition(mesh, k);
		LoadNormals(mesh, k);
		LoadTexcoords(mesh, k);
		LoadIndices(mesh, k);
		LoadMaterialName(mesh, k);
	}

	//Destroy Fbx Data
	scene->Destroy();
	manager->Destroy();

	CreateTextCombineVertex(textFile, fx, fy, fz);
	delete[] m_pParts;
}

void Converter::LoadMaterial(FbxSurfaceMaterial* material)
{
	Material tmp;
	FbxDouble3 colors;
	FbxProperty prop;

	//Lambert
	//if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		//Ambient
		prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (prop.IsValid()) colors = prop.Get<FbxDouble3>();
		else			    colors = FbxDouble3(0.0, 0.0, 0.0);

		tmp.materials.emplace_back((float)colors[0]);
		tmp.materials.emplace_back((float)colors[1]);
		tmp.materials.emplace_back((float)colors[2]);
		tmp.materials.emplace_back(0.0f);

		//Diffuse
		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid()) colors = prop.Get<FbxDouble3>();
		else				colors = FbxDouble3(1.0, 1.0, 1.0);

		tmp.materials.emplace_back((float)colors[0]);
		tmp.materials.emplace_back((float)colors[1]);
		tmp.materials.emplace_back((float)colors[2]);
		tmp.materials.emplace_back(1.0f);
	}
	//Phong
	//if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		//Specular
		prop = material->FindProperty(FbxSurfaceMaterial::sSpecular);
		if (prop.IsValid()) colors = prop.Get<FbxDouble3>();
		else				colors = FbxDouble3(0.0, 0.0, 0.0);

		tmp.materials.emplace_back((float)colors[0]);
		tmp.materials.emplace_back((float)colors[1]);
		tmp.materials.emplace_back((float)colors[2]);

		double shininess;
		prop = material->FindProperty(FbxSurfaceMaterial::sShininess);
		if (prop.IsValid()) shininess = prop.Get<FbxDouble>();
		else				shininess = 0.0;

		tmp.materials.emplace_back((float)shininess);
	}

	FbxFileTexture* texture = nullptr;

	//Diffuse Prop
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0) texture = prop.GetSrcObject<FbxFileTexture>(0);
	else
	{
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0) texture = prop.GetSrcObject<FbxFileTexture>(0);
	}

	//No Texture
	if (texture == nullptr)
	{
		mMaterialMap[material->GetName()] = tmp;
		return;
	}

	std::string pathName = texture->GetRelativeFileName();
	std::replace(pathName.begin(), pathName.end(), '/', '\\');
	size_t lastSlashPos = pathName.rfind('\\');
	if (lastSlashPos != std::string::npos)
	{
		tmp.textureFile = pathName.c_str() + lastSlashPos + 1;
	}
	else
	{
		tmp.textureFile = pathName;
	}

	//Register Material to Map
	mMaterialMap[material->GetName()] = tmp;
}

void Converter::LoadPosition(FbxMesh* mesh, int k)
{
	//Vertex Coordinate
	FbxVector4* positions = mesh->GetControlPoints();
	//Index Info
	int* indices = mesh->GetPolygonVertices();
	//Num Vertices
	int polygonVertexCount = mesh->GetPolygonVertexCount();
	
	m_pParts[k].positions.reserve(polygonVertexCount * 3);
	for (int i = 0; i < polygonVertexCount; i++)
	{
		int index = indices[i];
		m_pParts[k].positions.emplace_back((float)positions[index][mIdx0]);
		m_pParts[k].positions.emplace_back((float)positions[index][mIdx1]);
		m_pParts[k].positions.emplace_back((float)positions[index][mIdx2]);
	}
}

void Converter::LoadNormals(FbxMesh* mesh, int k)
{
	FbxArray<FbxVector4> normals;
	mesh->GetPolygonVertexNormals(normals);
	m_pParts[k].normals.reserve(normals.Size() * 3);
	for (int i = 0; i < normals.Size(); i++)
	{
		m_pParts[k].normals.emplace_back((float)normals[i][mIdx0]);
		m_pParts[k].normals.emplace_back((float)normals[i][mIdx1]);
		m_pParts[k].normals.emplace_back((float)normals[i][mIdx2]);
	}
}

void Converter::LoadTexcoords(FbxMesh* mesh, int k)
{
	FbxStringList uvset_names;
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> texcoords;
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), texcoords);
	m_pParts[k].texcoords.reserve(texcoords.Size() * 2);
	for (int i = 0; i < texcoords.Size(); i++)
	{
		m_pParts[k].texcoords.emplace_back((float)texcoords[i][0]);
		m_pParts[k].texcoords.emplace_back((float)(1.0 - texcoords[i][1]));
	}
}

void Converter::LoadIndices(FbxMesh* mesh, int k)
{
	int polygonCount = mesh->GetPolygonCount();
	m_pParts[k].indices.reserve(polygonCount * 3);
	for (int i = 0; i < polygonCount; i++)
	{
		m_pParts[k].indices.emplace_back(i * 3 + 0);
		m_pParts[k].indices.emplace_back(i * 3 + 1);
		m_pParts[k].indices.emplace_back(i * 3 + 2);
	}
}

void Converter::LoadMaterialName(FbxMesh* mesh, int k)
{
	//None Material
	if (mesh->GetElementMaterialCount() == 0)
	{
		m_pParts[k].materialName = "";
		return;
	}

	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
	int index = material->GetIndexArray().GetAt(0);

	FbxSurfaceMaterial* surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	if (surface_material != nullptr)
	{
		m_pParts[k].materialName = surface_material->GetName();
	}
	else
	{
		m_pParts[k].materialName = "";
	}
}

void Converter::CreateTextCombineVertex(const char* textFile, float fx, float fy, float fz)
{
	std::ofstream ofs(textFile);
	ofs << NumParts << '\n';

	for (int k = 0; k < NumParts; k++)
	{
		//Vertex
		{
			size_t numVertices = m_pParts[k].positions.size() / 3;
			ofs << "vertices " << numVertices << '\n';
			
			size_t i = 0;
			size_t j = 0;
			while (i < m_pParts[k].positions.size())
			{
				ofs << fx * m_pParts[k].positions[i] << " ";
				ofs << fy * m_pParts[k].positions[i + 1] << " ";
				ofs << fz * m_pParts[k].positions[i + 2] << " ";
				//X
				if (fx >= 0) ofs << m_pParts[k].normals[i] << " ";
				else ofs << -m_pParts[k].normals[i] << " ";
				//Y
				if (fy >= 0) ofs << m_pParts[k].normals[i + 1] << " ";
				else ofs << -m_pParts[k].normals[i + 1] << " ";
				//Z
				if (fz >= 0) ofs << m_pParts[k].normals[i + 2] << " ";
				else ofs << -m_pParts[k].normals[i + 2] << " ";

				i += 3;

				ofs << m_pParts[k].texcoords[j + 0] << " ";
				ofs << m_pParts[k].texcoords[j + 1] << "\n";

				j += 2;
			}
		}

#ifdef USE_INDEX
		//Index
		{
			size_t numIndices = m_pParts[k].indices.size();
			ofs << "indices " << numIndices << "\n";

			size_t i = 0;
			while (i < numIndices)
			{
				ofs << m_pParts[k].indices[i++] << " ";
				ofs << m_pParts[k].indices[i++] << " ";
				ofs << m_pParts[k].indices[i++] << "\n";
			}
		}
#endif
		//Material
		{
			ofs << "material\n";
			float* d = mMaterialMap[m_pParts[k].materialName].materials.data();
			ofs << d[0] << " " << d[1] << " " <<  d[2] << " " <<  d[3] << "\n";
			ofs << d[4] << " " << d[5] << " " <<  d[6] << " " <<  d[7] << "\n";
			ofs << d[8] << " " << d[9] << " " << d[10] << " " << d[11] << "\n";
		}

		//Texture
		{
			ofs << "texture ";
			std::string& textureFileName = mMaterialMap[m_pParts[k].materialName].textureFile;

			//None Texture
			if (textureFileName == "")
			{
				ofs << "Assets/White.png\n";
				continue;
			}

			std::string pathName = textFile;
			std::replace(pathName.begin(), pathName.end(), '/', '\\');

			size_t lastSlashPos = pathName.rfind('\\');
			if (lastSlashPos != std::string::npos)
			{
				pathName.erase(lastSlashPos + 1);
				ofs << pathName + textureFileName << "\n";
			}
		}
	}
}

//void Converter::CreateText(const char* textFile, float fx, float fy, float fz)
//{
//}
