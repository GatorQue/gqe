/**
* Provides a key binding interface
*
* @file src/GQE/Entity/classes/KeyBinder.cpp
* @author Jacob Dix
* @date 20140419 - Initial Release
*/
#include <GQE/Entity/classes/KeyBinder.hpp>

namespace GQE
{
  KeyBinder::KeyBinder(IApp& theApp):
  mApp(theApp)
  {

  }
  void KeyBinder::HandleEvents(sf::Event theEvent)
  {
    if(mEntity!=NULL && (theEvent.type==sf::Event::KeyPressed || theEvent.type==sf::Event::KeyReleased))
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      anIter=mKeyBindings.find(theEvent.key.code);
      if(anIter!=mKeyBindings.end())
      {
        if((mKeyBindings[theEvent.key.code].Type==INPUT_PRESSED && theEvent.type==sf::Event::KeyPressed)||
          (mKeyBindings[theEvent.key.code].Type==INPUT_RELEASED &&theEvent.type==sf::Event::KeyReleased))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity", mEntity);
          anContext.Add<InputData>("InputData", anIter->second);
          mApp.mEventManager.DoEvent(mKeyBindings[theEvent.key.code].EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::UpdateFixed()
  {
    if(mEntity!=NULL)
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      for(anIter=mKeyBindings.begin();anIter!=mKeyBindings.end();++anIter)
      {
        if(anIter->second.Type==INPUT_REALTIME && sf::Keyboard::isKeyPressed(sf::Keyboard::Key(anIter->first)))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntity);
          anContext.Add<InputData>("InputData", anIter->second);
          mApp.mEventManager.DoEvent(anIter->second.EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::RegisterKey(GQE::Uint32 theKey, typeEventID theEventID, Uint8 theType, Uint32 theAction)
  {
    InputData anCommand;
    anCommand.EventID=theEventID;
    anCommand.Type=theType;
    anCommand.Action = theAction;
    mKeyBindings[theKey]=anCommand;
  }
  void KeyBinder::RegisterEntity(IEntity* theEntity)
  {
    if (theEntity!=NULL)
    {
      mEntity=theEntity;
    }
  }
}
