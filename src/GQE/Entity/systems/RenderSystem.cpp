#include <GQE/Entity/systems/RenderSystem.hpp>
#include <GQE/Core/assets/ImageAsset.hpp>
#include <GQE/Entity/classes/Prototype.hpp>
#include <GQE/Entity/classes/Instance.hpp>
#include <SFML/Graphics.hpp>
namespace GQE
{
	RenderSystem::RenderSystem(IApp& theApp):
		ISystem("RenderSystem",theApp)
	{
		
	}
	RenderSystem::~RenderSystem()
	{

	}
	void RenderSystem::RegisterPrototype(Prototype* thePrototype)
	{
		thePrototype->AddProperty<sf::Vector2f>("Position",sf::Vector2f(0,0));
		thePrototype->AddProperty<sf::Vector2f>("Scale",sf::Vector2f(1,1));
		thePrototype->AddProperty<sf::Vector2f>("Orgin",sf::Vector2f(0,0));
		thePrototype->AddProperty<float>("Rotation",0);
		thePrototype->AddProperty<ImageAsset>("Image",ImageAsset(""));
		thePrototype->AddProperty<sf::IntRect>("SubRect",sf::IntRect(0,0,0,0));
		thePrototype->AddProperty<bool>("Visible",false);
		thePrototype->AddSystem(this);
	}
	void RenderSystem::InitInstance(Instance* theInstance)
	{

	}
	void RenderSystem::HandleEvents(sf::Event theEvent)
	{

	}
	void RenderSystem::UpdateFixed()
	{

	}
	void RenderSystem::UpdateVariable(float theElaspedTime)
	{
		
	}
	void RenderSystem::Draw()
	{
		Instance* anInstance=NULL;
		std::vector<Instance*>::iterator anInstanceIter;
		for(anInstanceIter=mInstanceList.begin();
			anInstanceIter!=mInstanceList.end();
			++anInstanceIter)
		{
			anInstance=(*anInstanceIter);
			sf::Sprite anSprite;
			if(anInstance!=NULL)
			{
				if(anInstance->GetProperty<ImageAsset>("Image").GetID().length()>0)
				{
					#if SFML_VERSION_MAJOR<2

					#else
					anSprite.setPosition(anInstance->GetProperty<sf::Vector2f>("Position"));
					anSprite.setOrigin(anInstance->GetProperty<sf::Vector2f>("Orgin"));
					anSprite.setScale(anInstance->GetProperty<sf::Vector2f>("Scale"));
					anSprite.setRotation(anInstance->GetProperty<float>("Rotation"));
					anSprite.setTextureRect(anInstance->GetProperty<sf::IntRect>("SubRect"));
					anSprite.setTexture(anInstance->GetProperty<ImageAsset>("Image").GetAsset());
					mApp.mWindow.draw(anSprite);
					#endif
				}
			}
		}	
	}
	void RenderSystem::HandleCleanup()
	{

	}
}
