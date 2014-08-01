#include "GraphicModel.h"

#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "GraphicManager.h"
#include <Logger.h>
#include <Windows.h>
#include <xnamath.h>
#include <ByteCopy.h>

GraphicModel::GraphicModel(){}

void GraphicModel::InitVertexBuffer()
{
	auto& graphicD3D = GraphicManager::GetInstance().D3DStuff;

	std::vector<char> verticeBytes = this->GetVerticeByte();

	// Create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = verticeBytes.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = verticeBytes.data();
	ID3D11Buffer* buffer = nullptr;
	HRESULT hr = graphicD3D.pd3dDevice->CreateBuffer(&bd, &InitData, &buffer);
	if(FAILED(hr)){	Logger::LogError("Failed at creating vertex buffer");}
	this->pVertexBuffer = buffer;
}
void GraphicModel::InitIndexBuffer()
{
	auto& graphicD3D = GraphicManager::GetInstance().D3DStuff;

	std::vector<WORD> indices;
	indices.resize(this->Model->Faces.size());
	std::copy(this->Model->Faces.begin(), this->Model->Faces.end(), indices.begin());

	// Create index buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)* indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices.data();
	ID3D11Buffer* buffer = nullptr;
	HRESULT hr = graphicD3D.pd3dDevice->CreateBuffer(&bd, &InitData, &buffer);
	if(FAILED(hr)){	Logger::LogError("Failed at creating index buffer");}
	this->pIndexBuffer = buffer;
}

void GraphicModel::Setup(const GraphicCameraEntity& camera, const GraphicObjectEntity& object)
{
	ID3D11DeviceContext* pImmediateContext = GraphicManager::GetInstance().D3DStuff.pImmediateContext;

	UINT stride = this->GetInputLayoutSize();
	UINT offset = 0;
	ID3D11Buffer* pV = this->pVertexBuffer;
	pImmediateContext->IASetVertexBuffers(0, 1, &pV, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(this->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<VertexLayout> GraphicModel::GetVertexLayout()
{
	return this->VertexLayoutList;
}

unsigned int GraphicModel::GetNumberFaces()
{
	return this->Model->Faces.size();
}

void GraphicModel::SetupModelLayout()
{
	if(this->Model->Vertices.size() == 0){ Logger::LogError("The model that was passed in doesn't not contain any vertices"); }
	this->AddInputLayout(InputLayoutName::Position, InputLayoutType::Position, sizeof(XMFLOAT4));
	if(this->Model->Normal.size() > 0){ this->AddInputLayout(InputLayoutName::Normal, InputLayoutType::Normal, sizeof(XMFLOAT4)); }
	if(this->Model->Tangents.size() > 0){ this->AddInputLayout(InputLayoutName::Tangents, InputLayoutType::Tangents, sizeof(XMFLOAT4)); }
	if(this->Model->Bitangents.size() > 0){ this->AddInputLayout(InputLayoutName::Bitangents, InputLayoutType::Bitangents, sizeof(XMFLOAT4)); }
	if(this->Model->Color.size() > 0){ this->AddInputLayout(InputLayoutName::Color, InputLayoutType::Color, sizeof(XMFLOAT4)); }
	if(this->Model->Texture.size() > 0){ this->AddInputLayout(InputLayoutName::Texture, InputLayoutType::Texture, sizeof(XMFLOAT4)); }
}
std::vector<char> GraphicModel::GetVerticeByte()
{
	this->SetupModelLayout();

	unsigned int sizeOfLayout = this->GetInputLayoutSize();
	unsigned int numOfVertices = this->Model->Vertices.size();
	unsigned int finalSize = sizeOfLayout * numOfVertices;

	std::vector<char> verticeBytes;
	verticeBytes.reserve(finalSize);

	for(unsigned int i = 0; i < numOfVertices; ++i)
	{
		for(const VertexLayout& ilIter : VertexLayoutList)
		{
			if(ilIter.Name == InputLayoutName::Position)
			{
				const CML::Vec3& v = this->Model->Vertices[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
			else if(ilIter.Name == InputLayoutName::Normal)
			{
				const CML::Vec3& v = this->Model->Normal[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
			else if(ilIter.Name == InputLayoutName::Tangents)
			{
				const CML::Vec3& v = this->Model->Tangents[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
			else if(ilIter.Name == InputLayoutName::Bitangents)
			{
				const CML::Vec3& v = this->Model->Bitangents[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
			else if(ilIter.Name == InputLayoutName::Color)
			{
				const CML::Vec4& v = this->Model->Color[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
			else if(ilIter.Name == InputLayoutName::Texture)
			{
				const CML::Vec3& v = this->Model->Texture[i];
				XMFLOAT4 c((float)v[0], (float)v[1], (float)v[2], 1.0);
				CHL::ByteCopy<XMFLOAT4>(c, verticeBytes, verticeBytes.size());
			}
		}
	}
	return verticeBytes;
}

void GraphicModel::AddInputLayout(std::string Name, DXGI_FORMAT format, unsigned int Size)
{
	unsigned int offset = this->GetInputLayoutSize();
	this->VertexLayoutList.push_back({Name, format, Size, offset});
}
unsigned int GraphicModel::GetInputLayoutSize()
{
	unsigned int offset = 0;
	if(this->VertexLayoutList.size() > 0)
	{
		const VertexLayout& lastElement = this->VertexLayoutList.back();
		offset = lastElement.Offset + lastElement.Size;
	}
	return offset;
}

std::shared_ptr<GraphicModel> GraphicModel::Spawn(std::shared_ptr<CHL::Model> model)
{
	std::shared_ptr<GraphicModel> newObject(new GraphicModel());

	newObject->Model = model;
	newObject->InitVertexBuffer();
	newObject->InitIndexBuffer();

	return newObject;
}