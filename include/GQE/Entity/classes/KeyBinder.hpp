#ifndef KEY_BINDER_HPP_INCLUDED
#define KEY_BINDER_HPP_INCLUDED
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Entity/Entity_types.hpp>
namespace GQE
{
  class KeyBinder
  {
    public:
      KeyBinder(IApp& theApp, IEntity* theEntity);
      void HandleEvents(sf::Event theEvent);
      void UpdateFixed();
      void RegisterEvent(Uint32 theBinding, std::string theEventID, Uint8 theInputType);
      void RegisterEntity(IEntity* theEntity);
    private:
      IApp& mApp;
      std::map<GQE::Uint32, InputCommand> mBindings;
      IEntity* mEntity;//the controllable entity.
  };
}

#endif
