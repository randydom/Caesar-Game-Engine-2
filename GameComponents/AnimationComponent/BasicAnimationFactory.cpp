#include "BasicAnimationFactory.h"
#include <Message.h>
#include <GenerateGUID.h>
#include "AnimationManager.h"
#include "Resource.h"

std::string BasicAnimationFactory::Create(std::shared_ptr<CHL::Animation> animation)
{
	class  CreateMessage : public Message
	{
	public:
		CreateMessage(std::shared_ptr<CHL::Animation> animation)
		{
			this->ID = CHL::GenerateGUID();
			this->animation = animation;
		}

		virtual Message::Status Work()
		{
			std::lock_guard<std::mutex> lock(AnimationManager::GetInstance().mutex);
			std::shared_ptr<BasicAnimation> newBasicAnimation = BasicAnimation::Spawn(this->animation);
			Resource::AnimationsList[this->ID] = newBasicAnimation;

			return Message::Status::Complete;
		}

		std::shared_ptr<CHL::Animation> animation;
		std::string	ID;
	};
	std::shared_ptr<CHL::Animation> cloneAnimation(new CHL::Animation(*animation));
	std::shared_ptr<CreateMessage> msg(new CreateMessage(cloneAnimation));
	AnimationManager::GetInstance().SubmitMessage(msg);
	return msg->ID;
}

void BasicAnimationFactory::Release(std::string ID)
{
	class ReleaseMessage : public Message
	{
	public:
		std::string ID;
		ReleaseMessage(std::string ID)
		{
			this->ID = ID;
		}

		virtual Message::Status Work()
		{
			std::lock_guard<std::mutex> lock(AnimationManager::GetInstance().mutex);

			Resource::AnimationsList.erase(this->ID);

			return Message::Status::Complete;
		}
	};

	std::shared_ptr<ReleaseMessage> msg(new ReleaseMessage(ID));
	AnimationManager::GetInstance().SubmitMessage(msg);
}