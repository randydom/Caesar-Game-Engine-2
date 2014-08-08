#include "GraphicObjectEntity.h"

#include "Rasterizer.h"
#include "GraphicManager.h"
#include "GraphicModel.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "XNAConverter.h"
#include "ResourceManager.h"
#include <AnimationCommunicator\AnimationControllerConfig.h>
#include <PhysicsCommunicator\RigidBodyConfig.h>

GraphicObjectEntity::GraphicObjectEntity(){}
GraphicObjectEntity::GraphicObjectEntity(std::weak_ptr<ObjectEntity> v)
{
	if(std::shared_ptr<ObjectEntity> spV = v.lock())
	{
		this->obj = std::dynamic_pointer_cast<ObjectEntity>(spV->Clone());
	}
}

bool GraphicObjectEntity::IsValidToDraw(const GraphicCameraEntity& camera) const
{
	if(this->GetVertexShader() == false){ return false; }
	if(this->GetPixelShader() == false){ return false; }
	if(this->GetGraphicModel() == false){ return false; }
	return true;
}
void GraphicObjectEntity::Draw(const GraphicCameraEntity& camera) const
{
	ID3D11DeviceContext* pImmediateContext = GraphicManager::GetInstance().D3DStuff.pImmediateContext;
	if(this->IsValidToDraw(camera))
	{
		this->SetupVertexShader(camera);
		this->SetupGeometryShader(camera);
		this->SetupPixelShader(camera);
		this->SetupModel(camera);
		Rasterizer::Setup(camera, *this);
		std::shared_ptr<GraphicModel> model = this->GetGraphicModel();
		pImmediateContext->DrawIndexed(model->GetNumberFaces(), 0, 0);
	}
}
void GraphicObjectEntity::SetupVertexShader(const GraphicCameraEntity& camera) const
{
	if(std::shared_ptr<VertexShader> vertexShader = this->GetVertexShader())
	{
		vertexShader->Setup(camera, *this);
	}
}
void GraphicObjectEntity::SetupGeometryShader(const GraphicCameraEntity& camera) const
{
	if(std::shared_ptr<GeometryShader> geometryShader = this->GetGeometryShader())
	{
		geometryShader->Setup(camera, *this);
	}
}
void GraphicObjectEntity::SetupPixelShader(const GraphicCameraEntity& camera) const
{
	if(std::shared_ptr<PixelShader> pixelShader = this->GetPixelShader())
	{
		pixelShader->Setup(camera, *this);
	}
}
void GraphicObjectEntity::SetupModel(const GraphicCameraEntity& camera) const
{
	if(std::shared_ptr<GraphicModel> model = this->GetGraphicModel())
	{
		model->Setup(camera, *this);
	}
}

XMFLOAT4 GraphicObjectEntity::GetLocation() const
{
	XMFLOAT4 returnValue(0.0, 0.0, 0.0, 0.0);
	if(this->obj)
	{
		returnValue = ConvertVec4(this->obj->GetLocation());
	}
	return returnValue;
}
XMFLOAT4 GraphicObjectEntity::GetRotation() const
{
	XMFLOAT4 returnValue(0.0, 0.0, 0.0, 0.0);
	if(this->obj)
	{
		returnValue = ConvertVec4(this->obj->GetRotation());
	}
	return returnValue;
}
XMFLOAT4 GraphicObjectEntity::GetScale() const
{
	XMFLOAT4 returnValue(1.0, 1.0, 1.0, 1.0);
	if(this->obj)
	{
		returnValue = ConvertVec4(this->obj->GetScale());
	}
	return returnValue;
}
bool GraphicObjectEntity::HasDepth() const
{
	bool returnValue = true;
	if(this->obj)
	{
		returnValue = this->obj->GetDepth();
	}
	return returnValue;
}
std::string GraphicObjectEntity::GetVertexShaderID() const
{
	std::string returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetVertexShaderID();
		returnValue = id;
	}
	return returnValue;
}
std::shared_ptr<GraphicModel> GraphicObjectEntity::GetGraphicModel() const
{
	std::shared_ptr<GraphicModel> returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetGraphicModelID();
		returnValue = ResourceManager::GraphicModelList.Find(id);
	}
	return returnValue;
}
std::shared_ptr<VertexShader> GraphicObjectEntity::GetVertexShader() const
{
	std::shared_ptr<VertexShader> returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetVertexShaderID();
		returnValue = ResourceManager::VertexShaderList.Find(id);
	}
	return returnValue;
}
std::shared_ptr<GeometryShader> GraphicObjectEntity::GetGeometryShader() const
{
	std::shared_ptr<GeometryShader> returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetGeometryShaderID();
		returnValue = ResourceManager::GeometryShaderList.Find(id);
	}
	return returnValue;
}
std::shared_ptr<PixelShader> GraphicObjectEntity::GetPixelShader() const
{
	std::shared_ptr<PixelShader> returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetPixelShaderID();
		returnValue = ResourceManager::PixelShaderList.Find(id);
	}
	return returnValue;
}
std::hash_map<std::string, XMFLOAT4X4> GraphicObjectEntity::GetAnimation() const
{
	std::hash_map<std::string, XMFLOAT4X4> returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetAnimationID();
		// to do
	}
	return returnValue;
}
XMFLOAT4X4 GraphicObjectEntity::GetJointAnimation() const
{
	XMFLOAT4X4 returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetAnimationID();
		std::string jointName = this->obj->GetJointName();
		CML::Matrix4x4 jaMatrix = AnimationControllerConfig::GetSingleJoint(id, jointName);
		returnValue = Convert4x4(jaMatrix);
	}
	return returnValue;
}
XMFLOAT4X4 GraphicObjectEntity::GetRigidBody() const
{
	XMFLOAT4X4 returnValue;
	if(this->obj)
	{
		std::string id = this->obj->GetRigidBodyID();
		CML::Matrix4x4 jaMatrix = RigidBodyConfig::GetTranslation(id);
		returnValue = Convert4x4(jaMatrix);
	}
	return returnValue;
}
std::hash_set<std::string> GraphicObjectEntity::GetGroupList() const
{
	std::hash_set<std::string> returnValue;
	if(this->obj)
	{
		returnValue = this->obj->GetGroupList();
	}
	return returnValue;
}
std::shared_ptr<BasicTexture> GraphicObjectEntity::FindTexture(std::string Name) const
{
	std::shared_ptr<BasicTexture> returnValue;

	if(this->obj)
	{
		std::string ID;
		if(this->obj->FindTexture(Name, ID))
		{
			returnValue = ResourceManager::TextureList.Find(ID);
		}
	}

	return returnValue;
}
std::hash_map<std::string, std::shared_ptr<Object>> GraphicObjectEntity::GetUserData() const
{
	std::hash_map<std::string, std::shared_ptr<Object>>  returnValue;
	if(this->obj)
	{
		returnValue = this->obj->GetUserData();
	}
	return returnValue;
}
std::shared_ptr<Object> GraphicObjectEntity::FindUserData(std::string ID) const
{
	std::shared_ptr<Object> returnValue;
	if(this->obj)
	{
		returnValue = this->obj->FindUserData(ID);
	}
	return returnValue;
}
D3D11_FILL_MODE GraphicObjectEntity::GetFillMode() const
{
	D3D11_FILL_MODE returnValue = D3D11_FILL_SOLID;
	if(this->obj)
	{
		returnValue = static_cast<D3D11_FILL_MODE>(this->obj->GetFillMode());
	}
	return returnValue;
}
D3D11_CULL_MODE GraphicObjectEntity::GetCullMode() const
{
	D3D11_CULL_MODE returnValue = D3D11_CULL_BACK;
	if(this->obj)
	{
		returnValue = static_cast<D3D11_CULL_MODE>(this->obj->GetCullMode());
	}
	return returnValue;
}

XMFLOAT4X4 GraphicObjectEntity::GetLRS() const
{
	XMFLOAT4X4 returnValue;
	if(this->obj)
	{
		
		XMFLOAT4 location = this->GetLocation();
		XMFLOAT4 rotation = this->GetRotation();
		XMFLOAT4 scale = this->GetScale();
		
		XMMATRIX xmTranslate = XMMatrixIdentity();
		XMMATRIX xmRotateX = XMMatrixIdentity();	XMMATRIX xmRotateY = XMMatrixIdentity();	XMMATRIX xmRotateZ = XMMatrixIdentity();
		XMMATRIX xmScaling = XMMatrixIdentity();
		XMMATRIX xmObjectFinal = XMMatrixIdentity();

		xmTranslate = XMMatrixTranslation(location.x, location.y, location.z);

		xmRotateX = XMMatrixRotationX(rotation.x);
		xmRotateY = XMMatrixRotationY(rotation.y);
		xmRotateZ = XMMatrixRotationZ(rotation.z);

		xmScaling = XMMatrixScaling(scale.x, scale.y, scale.z);


		xmObjectFinal = XMMatrixMultiply(xmObjectFinal, xmScaling);
		xmObjectFinal = XMMatrixMultiply(xmObjectFinal, xmRotateX);
		xmObjectFinal = XMMatrixMultiply(xmObjectFinal, xmRotateY);
		xmObjectFinal = XMMatrixMultiply(xmObjectFinal, xmRotateZ);
		xmObjectFinal = XMMatrixMultiply(xmObjectFinal, xmTranslate);

		XMStoreFloat4x4(&returnValue, xmObjectFinal);
	}
	else
	{
		XMStoreFloat4x4(&returnValue, XMMatrixIdentity());
	}
	return returnValue;
}
XMFLOAT4X4 GraphicObjectEntity::GetWorldTransformation() const
{
	XMFLOAT4X4 returnValue;
	if(this->obj)
	{
		XMFLOAT4X4 animation = this->GetJointAnimation();
		XMFLOAT4X4 rigidBody = this->GetRigidBody();
		XMFLOAT4X4 LRS = this->GetLRS();

		XMMATRIX worldMatrix = XMLoadFloat4x4(&animation) * XMLoadFloat4x4(&LRS) * XMLoadFloat4x4(&rigidBody);

		XMStoreFloat4x4(&returnValue, worldMatrix);
	}
	else
	{
		XMStoreFloat4x4(&returnValue, XMMatrixIdentity());
	}
	return returnValue;

	
}
