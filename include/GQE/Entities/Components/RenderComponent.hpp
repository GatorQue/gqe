#ifndef RENDER_COMPONENT_HPP_INCLUDED
#define RENDER_COMPONENT_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <GQE/Entities/Entities_types.hpp>

namespace GQE
{
class RenderComponent : public IComponent
{
	public:
		/**
		* RenderComponent constructor
		* @param[in] theApp is the address to the App derived class
		*/
		RenderComponent(App& theApp);

		/**
		* RenderComponent deconstructor
		*/
		virtual ~RenderComponent();
		/**
		* DoInit is responsible for initializing this component.  HandleCleanup will
		* be called if mCleanup is true so Derived classes should always call
		* RenderComponent::DoInit() first before initializing their assets.
		*/
		virtual void DoInit(Entity* theEntity);
		/**
		* ReInit is responsible for Reseting this component.
		*/
		virtual void ReInit(void);
		/**
		* HandleEvents is responsible for handling input events for this
		* component when it is the active State.
		* @param[in] theEvent to process from the App class Loop method
		*/
		virtual void HandleEvents(sf::Event theEvent);

		/**
		* UpdateFixed is responsible for handling all State fixed update needs for
		* this component.
		*/
		virtual void UpdateFixed(void);

		/**
		* UpdateVariable is responsible for handling all component variable update
		* needs for this component.
		* @param[in] theElapsedTime since the last Draw was called
		*/
		virtual void UpdateVariable(float theElapsedTime);

		/**
		* Draw is responsible for handling all Drawing needs for this component
		*/
		virtual void Draw(void);

	protected:
		/**
		* Cleanup is responsible for performing any cleanup required before
		* this component is removed.
		*/
		virtual void Cleanup(void);
	private:
		sf::Sprite* mSprite;
		/**
		* Our copy constructor is private because we do not allow copies of
		* our Singleton class
		*/
		RenderComponent(const RenderComponent&);  // Intentionally undefined

		/**
		* Our assignment operator is private because we do not allow copies
		* of our Singleton class
		*/
		RenderComponent& operator=(const RenderComponent&); // Intentionally undefined
};
}

#endif
