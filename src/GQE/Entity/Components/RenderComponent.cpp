#include <GQE/Entity/Components/RenderComponent.hpp>
#include <GQE/Entity/classes/Entity.hpp>
namespace GQE
{
	RenderComponent::RenderComponent(IApp& theApp, EntityManager* theEntityManager,
    const typeAssetID theAssetID, bool theLoadFlag) :
    IComponent("RenderComponent",theApp,theEntityManager),
    mImage(theAssetID, theLoadFlag),
	  mSprite(mImage.GetAsset()),
    mLoadFlag(theLoadFlag)
  {
  }

  RenderComponent::~RenderComponent()
  {

  }

  void RenderComponent::DoInit(Entity* theEntity)
  {
	  IComponent::DoInit(theEntity);
	  theEntity->AddProperty<std::string>("SpriteName", mImage.GetID());
  }

  void RenderComponent::ReInit()
  {
  }

  void RenderComponent::HandleEvents(sf::Event theEvent)
  {
  }

  void RenderComponent::UpdateFixed()
  {
	  sf::Vector2f anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
	  float anRotation=mEntity->GetProperty<float>("Rotation");
	  sf::Vector2f anScale=mEntity->GetProperty<sf::Vector2f>("Scale");
  #if (SFML_VERSION_MAJOR < 2)
	  mSprite.SetPosition(anPosition);
	  mSprite.SetRotation(anRotation);
	  mSprite.SetScale(anScale);
  #else
	  mSprite.setOrigin(mSprite.getTexture().getSize().x/2,mSprite.getTexture().getSize().y/2);
	  mSprite.setPosition(anPosition);
	  mSprite.setRotation(anRotation);
	  mSprite.setScale(anScale);
  #endif
  }

  void RenderComponent::UpdateVariable(float theElapstedTime)
  {

  }

  void RenderComponent::Draw()
  {
  #if (SFML_VERSION_MAJOR < 2)
	  mApp.mWindow.Draw(mSprite);
  #else
	  mApp.mWindow.draw(mSprite);
  #endif
  }

  void RenderComponent::Cleanup(void)
  {
	  IComponent::Cleanup();
  }

  IComponent* RenderComponent::MakeClone()
  {
	  return (new(std::nothrow) RenderComponent(mApp, mEntityManager, mImage.GetID(), mLoadFlag));
  }
}
