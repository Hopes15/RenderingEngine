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
	Mesh*				m_pMesh			= nullptr;
	HDL_DescriptorHeap* m_pDescHeap		= nullptr;
	HDL_RootSignature*	m_pRootSig		= nullptr;
	HDL_PSO*			m_pPSO			= nullptr;
	HDL_Renderer*		m_pRenderer		= nullptr;
};

