#include <GQE/Entity/Components/CoordComponent.hpp>
#include "GQE/Entity/interfaces/IEntity.hpp"
namespace GQE
{
	CoordComponent::CoordComponent(IApp& theApp) :
    IComponent("CoordComponent",theApp)
  {

  }

  CoordComponent::~CoordComponent()
  {

  }

  void CoordComponent::DoInit(IEntity* theEntity)
  {
	  IComponent::DoInit(theEntity);
	  theEntity->AddProperty<sf::Vector2f>("Position",sf::Vector2f(0,0));
	  theEntity->AddProperty<float>("Rotation",0);
	  theEntity->AddProperty<sf::Vector2f>("Scale",sf::Vector2f(1,1));
  }

  void CoordComponent::ReInit()
  {

  }

  void CoordComponent::HandleEvents(sf::Event theEvent)
  {

  }

  void CoordComponent::UpdateFixed()
  {

  }

  void CoordComponent::UpdateVariable(float theElapstedTime)
  {

  }

  void CoordComponent::Draw()
  {

  }

  void CoordComponent::Cleanup(void)
  {
	  IComponent::Cleanup();
  }

  IComponent* CoordComponent::MakeClone()
  {
    return (new(std::nothrow) CoordComponent(mApp));
  }
}
