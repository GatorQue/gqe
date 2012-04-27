#include <GQE/Entities/Components/RenderComponent.hpp>
#include <GQE/Entities/classes/Entity.hpp>
namespace GQE
{
  RenderComponent::RenderComponent(App& theApp) :
    IComponent("Render",theApp),
    mSprite(NULL)
  {

  }
  RenderComponent::~RenderComponent()
  {

  }

  void RenderComponent::DoInit(Entity* theEntity)
  {
    IComponent::DoInit(theEntity);
    TProperty<sf::Vector2f>* anPosition=new TProperty<sf::Vector2f>;
    anPosition->setLable("Position");
    anPosition->setValue(sf::Vector2f(0.0f,0.0f));
    mEntity->SetProperty(anPosition);
    std::string anSpriteName=getString(mEntity,"SpriteName","");
    if(anSpriteName!="")
    {
      mSprite=mApp.mAssetManager.GetSprite(anSpriteName);
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
    AProperty* anPosition;
    anPosition=mEntity->GetProperty("Position");
    if(anPosition->getType()->Name()==typeid(TProperty<sf::Vector2f>).name())
    {
#if (SFML_VERSION_MAJOR < 2)
      mSprite->SetPosition(static_cast<TProperty<sf::Vector2f>*>(anPosition)->getValue());
#else
      mSprite->setPosition(static_cast<TProperty<sf::Vector2f>*>(anPosition)->getValue());
#endif
    }
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
}
