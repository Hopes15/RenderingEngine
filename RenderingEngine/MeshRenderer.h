#pragma once

class MeshRenderer
{
public:
	MeshRenderer(class HDL_DescriptorHeap* descHeap);

	~MeshRenderer();

	void Init();

	void Draw();

private:
	class HDL_RootSignature*  pRootSig  = nullptr;
	class HDL_PSO*			  pPSO	    = nullptr;
	class HDL_Renderer*       pRenderer = nullptr;
	class HDL_DescriptorHeap* pDescHeap = nullptr;
};

