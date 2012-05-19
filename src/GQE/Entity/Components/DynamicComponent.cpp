#include <GQE/Entity/Components/DynamicComponent.hpp>
#include "GQE/Entity/interfaces/IEntity.hpp"
namespace GQE
{
	DynamicComponent::DynamicComponent(GQE::IApp& theApp) :
    IComponent("DynamicComponent",theApp)
  {

  }

  DynamicComponent::~DynamicComponent()
  {

  }

  void DynamicComponent::DoInit(IEntity* theEntity)
  {
	  IComponent::DoInit(theEntity);
	  theEntity->AddProperty<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
	  theEntity->AddProperty<sf::Vector2f>("Acceleration",sf::Vector2f(0,0));
	  theEntity->AddProperty<float>("RotationVelocity",0);
  }

  void DynamicComponent::ReInit()
  {

  }

  void DynamicComponent::HandleEvents(sf::Event theEvent)
  {

  }

  void DynamicComponent::UpdateFixed()
  {
	  sf::Vector2f anPosition=mEntity->GetProperty<sf::Vector2f>("Position");
	  sf::Vector2f anVelocity=mEntity->GetProperty<sf::Vector2f>("Velocity");
	  sf::Vector2f anAccelleration=mEntity->GetProperty<sf::Vector2f>("Accelleration");
	  anVelocity+=anAccelleration;
	  anPosition+=anVelocity;
	  mEntity->SetProperty<sf::Vector2f>("Velocity",anVelocity);
	  mEntity->SetProperty<sf::Vector2f>("Position",anPosition);

	  float anRotationalVelocity=mEntity->GetProperty<float>("RotationVelocity");
	  float anRotation=mEntity->GetProperty<float>("Rotation");
	  anRotation+=anRotationalVelocity;
	  mEntity->SetProperty<float>("Rotation",anRotation);
  }

  void DynamicComponent::UpdateVariable(float theElapstedTime)
  {

  }

  void DynamicComponent::Draw()
  {

  }

  void DynamicComponent::Cleanup(void)
  {
	  IComponent::Cleanup();
  }

  GQE::IComponent* DynamicComponent::MakeClone()
  {
	  return (new(std::nothrow) DynamicComponent(mApp));
  }
}
