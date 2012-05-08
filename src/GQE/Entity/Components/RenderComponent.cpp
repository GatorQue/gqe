#include <GQE/Entity/Components/RenderComponent.hpp>
#include <GQE/Entity/classes/Entity.hpp>
namespace GQE
{
	RenderComponent::RenderComponent(App& theApp) :
IComponent("RenderComponent",theApp),
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
	theEntity->AddProperty<sf::Vector2f>("Position",sf::Vector2f(0,0));
	if(mSprite!=NULL)
	{
#if (SFML_VERSION_MAJOR < 2)
		mSprite->SetPosition(0,0);
#else
		mSprite->setPosition(0,0);
#endif
	}
}

void RenderComponent::ReInit()
{

}

void RenderComponent::HandleEvents(sf::Event theEvent)
{

}

void RenderComponent::UpdateFixed()
{
	sf::Vector2f anPosition;
	std::string anSpriteName="";
	anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
	if(mSprite==NULL)
	{
		anSpriteName=mEntity->GetProperty<std::string>("SpriteName");
		if(anSpriteName!="")
			mSprite=mApp.mAssetManager.GetSprite(anSpriteName);
	}
#if (SFML_VERSION_MAJOR < 2)
			mSprite->SetPosition((anPosition));
#else
			mSprite->setPosition((anPosition));
#endif
}

void RenderComponent::UpdateVariable(float theElapstedTime)
{

}

void RenderComponent::Draw()
{
	if(mSprite==NULL)
		return;
#if (SFML_VERSION_MAJOR < 2)
	mApp.mWindow.Draw(*mSprite);
#else
	mApp.mWindow.draw(*mSprite);
#endif
}

void RenderComponent::Cleanup(void)
{

}
IComponent* RenderComponent::MakeClone()
{
	return (new RenderComponent(mApp));
}
}
