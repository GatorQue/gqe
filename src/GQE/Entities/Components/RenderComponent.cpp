#include <GQE/Entities/Components/RenderComponent.hpp>

RenderComponent::RenderComponent(App& theApp) :
    IComponent("Render")
{
	ILOG() << "RenderComponent::ctor(" << mComponentID << ")" << std::endl;
}
RenderComponent::~RenderComponent()
{
	ILOG() << "RenderComponent::dtor(" << mStateID << ")" << std::endl;
}

void RenderComponent::DoInit(Entity* theEntity)
{
	IComponent::DoInit(theEntity);
	TProperty<sf::vector2f>* anPosition=new TProperty<sf::vector2f>;
	anPosition->setLable("Position");
	anPosition->setValue(sf::vector2f(0.0f,0.0f));
	mEntity->setProperty(anPosition);
}
void RenderComponent::ReInit()
{

}

void RenderComponent::HandleEvents(sf::Event theEvent)
{

}
void RenderComponent::UpdateFixed()
{

}
void RenderComponent::UpdateVariable(float theElapstedTime)
{

}
void RenderComponent::Draw()
{

}
void RenderComponent::Cleanup(void)
{
    ILOG() << "IComponent::Cleanup(" << mComponentID << ")" << std::endl;
}

