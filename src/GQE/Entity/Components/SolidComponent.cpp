#include <GQE/Entity/Components/SolidComponent.hpp>
#include <GQE/Entity/classes/Entity.hpp>
namespace GQE
{
	std::vector<Entity*> SolidComponent::mEntities;

	SolidComponent::SolidComponent(App& theApp, EntityManager* theEntityManager) :
	IComponent("SolidComponent",theApp,theEntityManager)
	{

	}
	SolidComponent::~SolidComponent()
	{

	}

	void SolidComponent::DoInit(Entity* theEntity)
	{
		IComponent::DoInit(theEntity);
		mEntity->AddProperty<sf::FloatRect>("BoundingBox",sf::FloatRect(0,0,0,0));
		mEntities.push_back(theEntity);
	}

	void SolidComponent::ReInit()
	{

	}

	void SolidComponent::HandleEvents(sf::Event theEvent)
	{

	}

	void SolidComponent::UpdateFixed()
	{
		std::vector<Entity*>::iterator it;
		std::vector<Entity*>::iterator it2;
		Entity* anEntity, anOtherEntity;
		for(it=mEntities.begin();it!=mEntities.end();++it)
		{
			for(it2=it+1;it2!=mEntities.end();++it2)
			{
				sf::FloatRect anRectA=(*it)->GetProperty<sf::FloatRect>("BoundingBox");
				sf::Vector2f anPosA=(*it)->GetProperty<sf::Vector2f>("Position");
				sf::FloatRect anRectB=(*it2)->GetProperty<sf::FloatRect>("BoundingBox");
				sf::Vector2f anPosB=(*it2)->GetProperty<sf::Vector2f>("Position");

			}
		}
	}

	void SolidComponent::UpdateVariable(float theElapstedTime)
	{

	}

	void SolidComponent::Draw()
	{

	}

	void SolidComponent::Cleanup(void)
	{
		IComponent::Cleanup();
	}
	IComponent* SolidComponent::MakeClone()
	{
		return (new SolidComponent(mApp,mEntityManager));
	}
}
