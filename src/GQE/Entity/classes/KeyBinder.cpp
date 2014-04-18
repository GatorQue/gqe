#include <GQE/Entity/classes/KeyBinder.hpp>

namespace GQE
{
  KeyBinder::KeyBinder(IApp& theApp, IEntity* theEntity):
  mApp(theApp),
  mEntity(theEntity)
  {

  }
  void KeyBinder::HandleEvents(sf::Event theEvent)
  {
    if(mEntity!=NULL && (theEvent.type==sf::Event::KeyPressed || theEvent.type==sf::Event::KeyReleased))
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      anIter=mBindings.find(theEvent.key.code);
      if(anIter!=mBindings.end())
      {
        if((mBindings[theEvent.key.code].InputType==INPUT_PRESSED && theEvent.type==sf::Event::KeyPressed)||
          (mBindings[theEvent.key.code].InputType==INPUT_RELEASED &&theEvent.type==sf::Event::KeyReleased))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntity);
          anContext.Add<sf::Keyboard::Key>("Key",theEvent.key.code);
          mApp.mEventManager.DoEvent(mBindings[theEvent.key.code].EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::UpdateFixed()
  {
    if(mEntity!=NULL)
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      for(anIter=mBindings.begin();anIter!=mBindings.end();++anIter)
      {
        if(anIter->second.InputType==INPUT_REALTIME && sf::Keyboard::isKeyPressed(sf::Keyboard::Key(anIter->first)))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntity);
          anContext.Add<sf::Keyboard::Key>("Key",sf::Keyboard::Key(anIter->first));
          mApp.mEventManager.DoEvent(anIter->second.EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::RegisterEvent(GQE::Uint32 theBinding, typeEventID theEventID, Uint8 theInputType)
  {
    InputData anCommand;
    anCommand.EventID=theEventID;
    anCommand.InputType=theInputType;
    mBindings[theBinding]=anCommand;
  }
  void KeyBinder::RegisterEntity(IEntity* theEntity)
  {
    if (theEntity!=NULL)
    {
      mEntity=theEntity;
    }
  }
}
