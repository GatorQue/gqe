#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <SFML/Graphics.hpp>
namespace GQE
{
	MovementSystem::MovementSystem(IApp& theApp):
		ISystem("MovementSystem",theApp)
	{
		
	}
	MovementSystem::~MovementSystem()
	{

	}
	void MovementSystem::RegisterPrototype(Prototype* thePrototype)
	{
		thePrototype->AddProperty<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
		thePrototype->AddProperty<sf::Vector2f>("Accelleration",sf::Vector2f(0,0));
		thePrototype->AddProperty<float>("RotationalVelocity",0);
		thePrototype->AddProperty<float>("RotationalAccelleration",0);
		thePrototype->AddProperty<bool>("Warp",true);
		thePrototype->AddSystem(this);
		ISystem::RegisterPrototype(thePrototype);
	}
	void MovementSystem::InitInstance(Instance* theInstance)
	{

	}
	void MovementSystem::HandleEvents(sf::Event theEvent)
	{

	}
	void MovementSystem::UpdateFixed()
	{
		Instance* anInstance=NULL;
		std::vector<Instance*>::iterator anInstanceIter;
		sf::Vector2f anPosition;
		sf::Vector2f anVelocity;
		sf::Vector2f anAccelleration;
		float anRotation;
		float anRotationalVelocity;
		float anRotationalAccelleration;

		for(anInstanceIter=mInstanceList.begin();
			anInstanceIter!=mInstanceList.end();
			++anInstanceIter)
		{
			anInstance=(*anInstanceIter);
			anPosition=anInstance->GetProperty<sf::Vector2f>("Position");
			anVelocity=anInstance->GetProperty<sf::Vector2f>("Velocity");
			anAccelleration=anInstance->GetProperty<sf::Vector2f>("Accelleration");
			anRotation=anInstance->GetProperty<float>("Rotation");
			anRotationalVelocity=anInstance->GetProperty<float>("RotationalVelocity");
			anRotationalAccelleration=anInstance->GetProperty<float>("RotationalAccelleration");
			anVelocity+=anAccelleration;
			anPosition+=anVelocity;
			anRotationalVelocity+=anRotationalAccelleration;
			anRotation+=anRotationalVelocity;
			if(anPosition.x>mApp.mWindow.getSize().x)
			{
				anPosition.x=0;
			}
			else if(anPosition.x<0)
			{
				anPosition.x=mApp.mWindow.getSize().x;
			}
			if(anPosition.y>mApp.mWindow.getSize().y)
			{
				anPosition.y=0;
			}
			else if(anPosition.y<0)
			{
				anPosition.y=mApp.mWindow.getSize().y;
			}
			anInstance->SetProperty<sf::Vector2f>("Velocity",anVelocity);
			anInstance->SetProperty<sf::Vector2f>("Position",anPosition);
			anInstance->SetProperty<float>("Rotation",anRotation);
			anInstance->SetProperty<float>("RotationalVelocity",anRotationalVelocity);
		}
	}
	void MovementSystem::UpdateVariable(float theElaspedTime)
	{
		
	}
	void MovementSystem::Draw()
	{
	
	}
	void MovementSystem::HandleCleanup()
	{

	}
}
