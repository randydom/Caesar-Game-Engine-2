#include "BasicScreenCapture.h"
#include <GenerateGUID.h>
#include "GraphicManager.h"
#include "BasicTexture.h"

BasicScreenCapture::BasicScreenCapture(const std::string& inputID)
	:ScreenCapture(inputID)
{
	this->current = 0;
}
void BasicScreenCapture::Init()
{
	this->ScreenShot[0] = BasicScreenShot::Spawn(CHL::GenerateGUID(), this->width, this->height);
	this->ScreenShot[1] = BasicScreenShot::Spawn(CHL::GenerateGUID(), this->width, this->height);
}
void BasicScreenCapture::Destory()
{
	this->ScreenShot[0]->Release();
	this->ScreenShot[1]->Release();
}
void BasicScreenCapture::Update(double realTime, double deltaTime)
{

}
void BasicScreenCapture::Snap(std::hash_map<std::string, SP_INFO>& objects)
{
	this->current += 1;
	if(this->current >= 2){ this->current = 0; }

	this->ScreenShot[this->current]->D3DInfo.cameraMatrix = this->cameraMatrix;
	this->ScreenShot[this->current]->D3DInfo.prespectiveMatrix = this->prespectiveMatrix;
	this->ScreenShot[this->current]->Snap(objects);

	auto allTexture = GraphicManager::GetInstance().AllTexture();
	auto textureIter = allTexture.find(this->TextureID);
	if(textureIter != allTexture.end())
	{
		auto basicTexture = std::dynamic_pointer_cast<BasicTexture>(textureIter->second);
		if(basicTexture != 0)
		{
			basicTexture->D3DInfo.pTexture = this->ScreenShot[this->current]->pScreenTexture;
		}
	}
}
std::shared_ptr<ScreenCapture> BasicScreenCapture::clone() const
{
	std::shared_ptr<ScreenCapture> newObject(new BasicScreenCapture(*this));
	newObject->Init();
	return newObject;

}

std::shared_ptr<BasicScreenCapture> BasicScreenCapture::Spawn(const std::string& inputID,
																		const std::string& textureID,
																		unsigned int width,
																		unsigned int height,
																		CHL::Matrix4x4 cameraMatrix,
																		CHL::Matrix4x4 prespectiveMatrix)
{
	std::shared_ptr<BasicScreenCapture> newObject(new BasicScreenCapture(inputID));

	newObject->TextureID = textureID;
	newObject->width = width;
	newObject->height = height;
	newObject->prespectiveMatrix = prespectiveMatrix;
	newObject->cameraMatrix = cameraMatrix;

	newObject->Init();

	return newObject;
}