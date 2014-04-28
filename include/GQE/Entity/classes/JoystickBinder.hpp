/**
* Provides a joystick binding interface
*
* @file src/GQE/Entity/classes/JoystickBinder.hpp
* @author Jacob Dix
* @date 20140419 - Initial Release
*/
#ifndef JOYSTICK_BINDER_HPP_INCLUDED
#define JOYSTICK_BINDER_HPP_INCLUDED
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
namespace GQE
{
  class GQE_API JoystickBinder
  {
    typedef std::map<Uint32, InputData > typeJoystickData;
    typedef std::map<Uint32, typeJoystickData > typeJoystickList;

    public:
      JoystickBinder(IApp& theApp);
      void HandleEvents(sf::Event theEvent);
      void UpdateFixed();
      void RegisterButton(Uint32 theJoystickID, Uint32 theButton, std::string theEventID, Uint8 theInputType, Uint32 theAction);
      void RegisterAxis(GQE::Uint32 theJoystick, GQE::Uint32 theAxis, typeEventID theEventID, Uint8 theType, Uint32 theAction, Uint32 theMoveAxis);
      void RegisterEntity(Uint32 theJoystickID, IEntity* theEntity);
    private:
      IApp& mApp;
      typeJoystickList mButtonBindings;
      typeJoystickList mAxisBindings;
      std::map<Uint32,IEntity*> mEntitys;//the controllable entitys for each Joystick.
  };
}
#endif
