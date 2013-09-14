#include "ObjectManager.h"
#include "Camera.h"
#include "Prespective.h"

void ObjectManager::Init()
{
	std::shared_ptr<Object> obj(new Camera);
	this->Insert(obj);
	std::shared_ptr<Prespective> prespective(new Prespective);
	this->Insert(prespective);
}

void ObjectManager::Update(double realTime, double deltaTime)
{

}

void ObjectManager::Work()	
{

}

void ObjectManager::Shutdown()	
{

}

void ObjectManager::Insert(std::shared_ptr<Object> obj)
{
	this->objects.push_back(obj);
}

const CHL::VectorQuerable<std::shared_ptr<Object>>  ObjectManager::AllObjects()
{
	return this->objects;
}