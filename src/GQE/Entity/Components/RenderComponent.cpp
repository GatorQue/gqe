#include <GQE/Entity/Components/RenderComponent.hpp>
#include <GQE/Entity/classes/Entity.hpp>
namespace GQE
{
	RenderComponent::RenderComponent(App& theApp,EntityManager* theEntityManager) :
IComponent("RenderComponent",theApp,theEntityManager),
	mSprite(NULL)
{

}
RenderComponent::~RenderComponent()
{

}

void RenderComponent::DoInit(Entity* theEntity)
{
	IComponent::DoInit(theEntity);
	theEntity->AddProperty<std::string>("SpriteName","");
}

void RenderComponent::ReInit()
{

}

void RenderComponent::HandleEvents(sf::Event theEvent)
{

}

void RenderComponent::UpdateFixed()
{
	std::string anSpriteName="";
	sf::Vector2f anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
	float anRotation=mEntity->GetProperty<float>("Rotation");
	sf::Vector2f anScale=mEntity->GetProperty<sf::Vector2f>("Scale");
	if(mSprite==NULL)
	{
		anSpriteName=mEntity->GetProperty<std::string>("SpriteName");
		if(anSpriteName!="")
			mSprite=mApp.mAssetManager.GetSprite(anSpriteName);
		if(mSprite!=NULL)
			mSprite->setOrigin(mSprite->getTexture()->getSize().x/2,mSprite->getTexture()->getSize().y/2);
	}
#if (SFML_VERSION_MAJOR < 2)
			mSprite->SetPosition(anPosition);
			mSprite->SetRotation(anRotation);
			mSprite->SetScale(anScale);
#else
			mSprite->setPosition(anPosition);
			mSprite->setRotation(anRotation);
			mSprite->setScale(anScale);
#endif
}

void RenderComponent::UpdateVariable(float theElapstedTime)
{

}

void RenderComponent::Draw()
{
	if(mSprite!=NULL)
	{
#if (SFML_VERSION_MAJOR < 2)
	mApp.mWindow.Draw(*mSprite);
#else
	mApp.mWindow.draw(*mSprite);
#endif
	}
}

void RenderComponent::Cleanup(void)
{
	IComponent::Cleanup();
}
IComponent* RenderComponent::MakeClone()
{
	return (new RenderComponent(mApp,mEntityManager));
}
}
