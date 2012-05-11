#ifndef COORD_COMPONENT_HPP_INCLUDED
#define COORD_COMPONENT_HPP_INCLUDED

#include <SFML/System.hpp>
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/interfaces/IComponent.hpp>

namespace GQE
{
	class GQE_API CoordComponent : public IComponent
	{
	public:
		/**
		* RenderComponent constructor
		* @param[in] theApp is the address to the App derived class
		*/
		CoordComponent(App& theApp,EntityManager* theEntityManager);

		/**
		* RenderComponent deconstructor
		*/
		virtual ~CoordComponent();
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
		IComponent* MakeClone();
	protected:
		/**
		* Cleanup is responsible for performing any cleanup required before
		* this component is removed.
		*/
		virtual void Cleanup(void);
	private:
		/**
		* Our copy constructor is private because we do not allow copies of
		* our Singleton class
		*/
		CoordComponent(const CoordComponent&);  // Intentionally undefined

		/**
		* Our assignment operator is private because we do not allow copies
		* of our Singleton class
		*/
		CoordComponent& operator=(const CoordComponent&); // Intentionally undefined
	};
}

#endif
