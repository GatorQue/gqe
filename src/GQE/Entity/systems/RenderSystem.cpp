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
		thePrototype->AddProperty<float>("Rotation",0);
		thePrototype->AddProperty<sf::Sprite>("Sprite",sf::Sprite());
		thePrototype->AddProperty<bool>("Visible",true);
		thePrototype->AddSystem(this);
		ISystem::RegisterPrototype(thePrototype);
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
			if(anInstance!=NULL)
			{
					#if SFML_VERSION_MAJOR<2

					#else
					sf::Sprite anSprite=anInstance->GetProperty<sf::Sprite>("Sprite");
					anSprite.setPosition(anInstance->GetProperty<sf::Vector2f>("Position"));
					anSprite.setRotation(anInstance->GetProperty<float>("Rotation"));
					mApp.mWindow.draw(anSprite);
					#endif
			}
		}	
	}
	void RenderSystem::HandleCleanup()
	{

	}
}
