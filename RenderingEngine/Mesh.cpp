#include "Mesh.h"
#include "HDL_Renderer.h"
#include "HDL_VertexBuffer.h"
#include "HDL_IndexBuffer.h"

Mesh::Mesh() : pRenderer(HDL_Renderer::GetInstance())
{
}

Mesh::~Mesh()
{
	delete pIndexBuff;
	delete pVertexBuff;
}

void Mesh::Init()
{
	//Vertex
	UINT64 size   = sizeof(Vertex) * mVertices.size();
	UINT   stride = sizeof(Vertex);

	pVertexBuff   = new HDL_VertexBuffer(size, stride);
	pVertexBuff->CopyBufferToVRAM(mVertices.data());

	//Index
	size		  = sizeof(uint16_t) * mIndices.size();

	pIndexBuff	  = new HDL_IndexBuffer(size);
	pIndexBuff->CopyBufferToVRAM(mIndices.data());
}

void Mesh::Draw()
{
	//Get CmdList
	auto cmdList = pRenderer->GetCmdList();

	//Set PrimitiveTopology
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set VertexBuffer
	cmdList->IASetVertexBuffers(0, 1, pVertexBuff->GetView());

	//Set IndexBuffer
	cmdList->IASetIndexBuffer(pIndexBuff->GetView());

	//Draw
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
