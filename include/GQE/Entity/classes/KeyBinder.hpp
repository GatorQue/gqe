/**
* Provides a key binding interface
*
* @file src/GQE/Entity/classes/KeyBinder.hpp
* @author Jacob Dix
* @date 20140419 - Initial Release
*/
#ifndef KEY_BINDER_HPP_INCLUDED
#define KEY_BINDER_HPP_INCLUDED
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
namespace GQE
{
  class GQE_API KeyBinder
  {
    public:
      KeyBinder(IApp& theApp);
      void HandleEvents(sf::Event theEvent);
      void UpdateFixed();
      void RegisterKey(Uint32 theKey, std::string theEventID, Uint8 theInputType, Uint32 theAction);
      void RegisterEntity(IEntity* theEntity);
    private:
      IApp& mApp;
      std::map<GQE::Uint32, InputData> mKeyBindings;
      IEntity* mEntity;//the controllable entity.
  };
}

#endif
