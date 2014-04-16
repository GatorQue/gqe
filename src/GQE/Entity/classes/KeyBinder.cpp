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
      std::map<GQE::Uint32, InputCommand>::iterator anIter;
      anIter=mBindings.find(theEvent.key.code);
      if(anIter!=mBindings.end())
      {
        if((mBindings[theEvent.key.code].InputType==INPUT_PRESSED && theEvent.type==sf::Event::KeyPressed)||
          (mBindings[theEvent.key.code].InputType==INPUT_RELEASED &&theEvent.type==sf::Event::KeyReleased))
        {
          InputContext anContext(theEvent,mEntity);
          mApp.mEventManager.DoEvent(mBindings[theEvent.key.code].EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::UpdateFixed()
  {
    if(mEntity!=NULL)
    {
      std::map<GQE::Uint32, InputCommand>::iterator anIter;
      for(anIter=mBindings.begin();anIter!=mBindings.end();++anIter)
      {
        if(anIter->second.InputType==INPUT_REALTIME)
        {
          InputContext anContext(sf::Event(),mEntity);
          mApp.mEventManager.DoEvent(anIter->second.EventID,&anContext);
        }
      }
    }
  }
  void KeyBinder::RegisterEvent(GQE::Uint32 theBinding, typeEventID theEventID, Uint8 theInputType)
  {
    InputCommand anCommand;
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
