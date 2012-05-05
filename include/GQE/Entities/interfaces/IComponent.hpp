/**
 * The Base Component class. Used to make other components.
 *
 * @file include/GQE/Entity/interfaces/IComponent.hpp
 * @author Jacob Dix
 * @date 20120417 - Initial Release
 */
#ifndef ICOMPONENT_HPP_INCLUDED
#define ICOMPONENT_HPP_INCLUDED

#include <GQE/Core/classes/App.hpp>
#include <GQE/Entities/Entities_types.hpp>
#include <GQE/Entities/interfaces/TProperty.hpp>

namespace GQE
{
  class GQE_API IComponent
  {
    public:
      /**
       * IComponent constructor
       * @param[in] theComponentID to use for this component object
       * @param[in] theApp is the address to the App derived class
       */
      IComponent(const typeComponentID theComponentID, App& theApp);

      /**
       * IComponent deconstructor
       */
      virtual ~IComponent();
      /**
       * GetID will return the ID used to identify this component object.
       * @return GQE::typeCompomentID is the ID for this component object.
       */
      const typeComponentID GetID(void) const;

      /**
       * DoInit is responsible for initializing this component.  HandleCleanup will
       * be called if mCleanup is true so Derived classes should always call
       * IComponent::DoInit() first before initializing their assets.
       */
      virtual void DoInit(Entity* theEntity);
      /**
       * ReInit is responsible for Reseting this component.
       */
      virtual void ReInit(void) = 0;

      /**
       * DeInit is responsible for marking this component to be cleaned up.
       */
      void DeInit(void);

      /**
       * IsInitComplete will return true if the DoInit method has been called
       * for this component.
       * @return true if DoInit has been called, false otherwise or if DeInit
       *         was called.
       */
      bool IsInitComplete(void);

      /**
       * CanRemove will return true if mRemoved is true;
       * @return true if the componet has been cleaned.
       */
      bool CanRemove(void);
      /**
       * HandleEvents is responsible for handling input events for this
       * component when it is the active State.
       * @param[in] theEvent to process from the App class Loop method
       */
      virtual void HandleEvents(sf::Event theEvent) = 0;

      /**
       * UpdateFixed is responsible for handling all State fixed update needs for
       * this component.
       */
      virtual void UpdateFixed(void) = 0;

      /**
       * UpdateVariable is responsible for handling all component variable update
       * needs for this component.
       * @param[in] theElapsedTime since the last Draw was called
       */
      virtual void UpdateVariable(float theElapsedTime) = 0;

      /**
       * Draw is responsible for handling all Drawing needs for this component
       */
      virtual void Draw(void) = 0;

      /**
       * HandleCleanup is responsible for calling Cleanup if this class has been
       * flagged to be cleaned up after it completes the game loop.
       */
      void HandleCleanup(void);

	  virtual IComponent* MakeClone()=0;
    protected:
      /// Address to the App class
      App&                  mApp;
      //pointer to the attached entity.
      Entity* mEntity;
      /**
       * Cleanup is responsible for performing any cleanup required before
       * this component is removed.
       */
      virtual void Cleanup(void);
    private:
      /// The component ID
      const typeComponentID     mComponentID;
      /// Boolean that indicates that DoInit has been called
      bool                  mInit;
      /// component needs to be cleaned up at the end of the next game loop
      bool                  mCleanup;
      ///component is cleaned and ready to be removed;
      bool mRemove;
      /**
       * Our copy constructor is private because we do not allow copies of
       * our Singleton class
       */
      IComponent(const IComponent&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our Singleton class
       */
      IComponent& operator=(const IComponent&); // Intentionally undefined
  };
}

#endif
