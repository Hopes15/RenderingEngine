#include "Loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

namespace fs = std::filesystem;

//�t�@�C���p�X����f�B���N�g���p�X�݂̂��擾
std::wstring GetDirPath(const std::wstring& path)
{
    fs::path p = path.c_str();
    return p.remove_filename().c_str();
}

//Wstring����String�^�ɕϊ�
std::string ConvertWString(const std::wstring& wstr)
{
    auto length = WideCharToMultiByte(CP_UTF8, 0U, wstr.data(), -1, nullptr, 0, nullptr, nullptr);

    //Length���z����m��
    auto buffer = new char[length];

    //�m�ۂ����z��ɕϊ���̓��e���R�s�[
    WideCharToMultiByte(CP_UTF8, 0U, wstr.data(), -1, buffer, length, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;
    buffer = nullptr;

    return result;
}

//String����Wstring�ɕϊ�
std::wstring ConvertString(const std::string& str)
{
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

    std::wstring wstr;
    wstr.resize(num1);

    //�ϊ���̏���wstr�ɃR�s�[
    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

    //�������`�F�b�N
    assert(num1 == num2);

    return wstr;
}

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

    //�G���[�`�F�b�N
    if (scene == nullptr)
    {
        //std::cout << importer.GetErrorString() << std::endl;

        return;
    }

    //Mesh�f�[�^�̓ǂݎ��
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

        //UV���]�̗L��
        if (inverseU) uv->x = 1 - uv->x;
        if (inverseV) uv->y = 1 - uv->y;

        //���_���
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
