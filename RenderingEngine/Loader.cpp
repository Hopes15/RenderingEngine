#include "Loader.h"
#include "Utilities.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader(ImportSettings settings)
{
    if (settings.fileName == nullptr) return;

    auto& meshes  = settings.meshes;
    auto inverseU = settings.inverseU;
    auto inverseV = settings.inverseV;
    auto filePath = ConvertWString(settings.fileName);

    Assimp::Importer importer;
    int flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;

    auto scene = importer.ReadFile(filePath, flag);

    //エラーチェック
    if (scene == nullptr)
    {
        //std::cout << importer.GetErrorString() << std::endl;

        return;
    }

    //Meshデータの読み取り
    meshes.clear();
    meshes.resize(scene->mNumMeshes);

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        const auto pMesh = scene->mMeshes[i];
        LoadMesh(meshes[i], pMesh, inverseU, inverseV);

        const auto pMaterial = scene->mMaterials[i];
        LoadTexture(settings.fileName, meshes[i], pMaterial);
    }

    scene = nullptr;
}

void ModelLoader::LoadMesh(MeshData& dst, const aiMesh* src, bool inverseU, bool inverseV)
{
    aiVector3D zero3D(0.0f, 0.0f, 0.0f);
    aiColor4D  zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

    dst.vertices.resize(src->mNumVertices);

    for (unsigned int i = 0u; i < src->mNumVertices; ++i)
    {
        auto position = &(src->mVertices[i]);
        auto normal   = &(src->mNormals[i]);
        auto uv       = (src->HasTextureCoords(0))        ? &(src->mTextureCoords[0][i]) : &zero3D;
        auto tangent  = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i])         : &zero3D;
        auto color    = (src->HasVertexColors(0))         ? &(src->mColors[0][i])        : &zeroColor;

        //UV反転の有無
        if (inverseU) uv->x = 1 - uv->x;
        if (inverseV) uv->y = 1 - uv->y;

        //頂点情報
        Vertex vertex = {};
        vertex.position = XMFLOAT3(position->x, position->y, position->z);
        vertex.normal   = XMFLOAT3(normal->x, normal->y, normal->z);
        vertex.uv       = XMFLOAT2(uv->x, uv->y);
        vertex.tangent  = XMFLOAT3(tangent->x, tangent->y, tangent->z);
        vertex.color    = XMFLOAT4(color->r, color->g, color->b, color->a);

        dst.vertices[i] = vertex;
    }

    dst.indices.resize(src->mNumFaces * 3);

    for (unsigned int i = 0u; i < src->mNumFaces; ++i)
    {
        const auto& face = src->mFaces[i];

        dst.indices[i * 3 + 0] = face.mIndices[0];
        dst.indices[i * 3 + 1] = face.mIndices[1];
        dst.indices[i * 3 + 2] = face.mIndices[2];
    }
}

void ModelLoader::LoadTexture(const wchar_t* filePath, MeshData& dst, const aiMaterial* src)
{
    aiString path;
    if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
    {
        auto dir  = GetDirPath(filePath);
        auto file = std::string(path.C_Str());
        dst.diffuseMap = dir + ConvertString(file);
    }
    else
    {
        dst.diffuseMap.clear();
    }
}
