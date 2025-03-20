#pragma once

class Mesh;
class HDL_DescriptorHeap;
class HDL_RootSignature;
class HDL_PSO;
class HDL_Renderer;
class MeshRenderer
{
public:
	MeshRenderer(Mesh* mesh, HDL_DescriptorHeap* descHeap);

	~MeshRenderer();

	void Draw();

private:
	Mesh* m_pMesh;
	HDL_DescriptorHeap* m_pDescHeap;
	HDL_RootSignature* m_pRootSig;
	HDL_PSO* m_pPSO;
	HDL_Renderer* m_pRenderer;
};

