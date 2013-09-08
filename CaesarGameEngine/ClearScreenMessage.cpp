#include "ClearScreenMessage.h"

#include "GraphicManager.h"

ClearScreenMessage::ClearScreenMessage(const boost::numeric::ublas::vector<double>& input)
	: colour(input)
{
}

void ClearScreenMessage::Proccess()
{
	GraphicManager::GetInstance().ClearColour = this->colour;
}