#include "Mesh.h"
#include "HDL_Renderer.h"
#include <fstream>
#include "Utilities.h"

Mesh::Mesh() :
	mNumParts(0)
{
}

Mesh::~Mesh()
{
	for (unsigned int i = 0; i < mNumParts; i++)
	{
		delete mTexBuffs[i];
		delete mCBuffs[i];
		delete mVertexBuffs[i];
	}
}

void Mesh::Load(const char* fileName)
{
	//Load File
	std::ifstream file(fileName);
	assert(!file.fail());

	file >> mNumParts;

	mMeshData.resize(mNumParts);

	for (UINT i = 0; i < mNumParts; i++)
	{
		std::string dataType;
		file >> dataType;

		//Vertex
		assert(dataType == "vertices");
		UINT numVertices = 0;
		file >> numVertices;
		mMeshData[i].numVertices = numVertices;
		mMeshData[i].vertices.resize(numVertices);

		for (UINT j = 0; j < numVertices; j++)
		{
			Vertex vert = {};
			file >> vert.pos.x	  >> vert.pos.y    >> vert.pos.z;
			file >> vert.normal.x >> vert.normal.y >> vert.normal.z;
			file >> vert.uv.x     >> vert.uv.y;

			mMeshData[i].vertices[j] = vert;
		}

		//Material
		Material mat = {};
		file >> dataType;
		assert(dataType == "material");

		file >> mat.ambient.x  >> mat.ambient.y  >> mat.ambient.z >> mat.ambient.w;
		file >> mat.diffuse.x  >> mat.diffuse.y  >> mat.diffuse.z >> mat.diffuse.w;
		file >> mat.specular.x >> mat.specular.y >> mat.specular.z >> mat.specular.w;
		mMeshData[i].material = mat;

		//Texture
		file >> dataType;
		assert(dataType == "texture");

		std::string fileName;
		file >> fileName;
		mMeshData[i].fileName = ConvertString(fileName);
	}

	file.close();
}

void Mesh::Create(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle)
{	
	auto device  = HDL_Renderer::GetInstance()->GetDevice();
	auto incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//確保
	mVertexBuffs.resize(mNumParts);
	mCBuffs.resize(mNumParts);
	mTexBuffs.resize(mNumParts);

	//メッシュごとにバッファ作成
	for (UINT i = 0; i < mNumParts; i++)
	{
		//VertexBuffer
		UINT64 size   = sizeof(Vertex) * mMeshData[i].numVertices;
		UINT   stride = sizeof(Vertex);

		mVertexBuffs[i] = new HDL_VertexBuffer(size, stride);
		mVertexBuffs[i]->CopyBufferToVRAM(mMeshData[i].vertices.data());

		//Material
		size = (sizeof(Material) + 0xff) & ~0xff;
		mCBuffs[i] = new HDL_ConstantBuffer(size);
		mCBuffs[i]->CopyBufferToVRAM(&mMeshData[i].material);
		mCBuffs[i]->CreateCBV(heapHandle);

		heapHandle.ptr += incSize;

		//Texture
#ifdef _DEBUG
		std::cout << ConvertWString(mMeshData[i].fileName) << std::endl;
#endif
		HDL_Texture texture(mMeshData[i].fileName.c_str());

		mTexBuffs[i] = new HDL_TextureBuffer(texture.GetMetaData(), texture.GetImage());
		mTexBuffs[i]->CopyBufferToVRAM(texture.GetImage());
		mTexBuffs[i]->CreateSRV(heapHandle);

		heapHandle.ptr += incSize;
	}
}
