/**
* Provides a mouse binding interface
*
* @file src/GQE/Entity/classes/MouseBinder.hpp
* @author Jacob Dix
* @date 20140419 - Initial Release
*/
#ifndef MOUSE_BINDER_HPP_INCLUDED
#define MOUSE_BINDER_HPP_INCLUDED
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
namespace GQE
{
  class GQE_API MouseBinder
  {
    public:
      MouseBinder(IApp& theApp);
      void HandleEvents(sf::Event theEvent);
      void UpdateFixed();
      void RegisterButton(Uint32 theBinding, std::string theEventID, Uint8 theInputType, Uint32 theAction);
      void RegisterMovement(std::string theEventID, Uint8 theInputType, Uint32 theAction, sf::Vector2f theAxis=sf::Vector2f(1,1));
      void RegisterEntity(IEntity* theEntity);
    private:
      IApp& mApp;
      std::map<GQE::Uint32, InputData> mButtonBindings;
      InputData mMoveBinding;
      IEntity* mEntity;//the controllable entity.
  };
}

#endif
