/**
* Provides a mouse binding interface
*
* @file src/GQE/Entity/classes/MouseBinder.cpp
* @author Jacob Dix
* @date 20140419 - Initial Release
*/
#include <GQE/Entity/classes/MouseBinder.hpp>

namespace GQE
{
  MouseBinder::MouseBinder(IApp& theApp, IEntity* theEntity) :
  mApp(theApp),
  mEntity(theEntity),
  mLastPosition(sf::Vector2f(sf::Mouse::getPosition()))
  {

  }
  void MouseBinder::HandleEvents(sf::Event theEvent)
  {
    if(mEntity!=NULL && (theEvent.type==sf::Event::MouseButtonPressed || theEvent.type==sf::Event::MouseButtonReleased|| theEvent.type==sf::Event::MouseMoved))
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      anIter = mButtonBindings.find(theEvent.key.code);
      if (anIter != mButtonBindings.end())
      {
        if ((mButtonBindings[theEvent.mouseButton.button].Type == INPUT_PRESSED && theEvent.type == sf::Event::MouseButtonPressed) ||
          (mButtonBindings[theEvent.mouseButton.button].Type == INPUT_RELEASED &&theEvent.type == sf::Event::MouseButtonReleased))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntity);
          anContext.Add<InputData>("InputData", anIter->second);
          anContext.Add<sf::Vector2f>("InputPosition", sf::Vector2f(theEvent.mouseButton.x, theEvent.mouseButton.y));
          mApp.mEventManager.DoEvent(mButtonBindings[theEvent.mouseButton.button].EventID, &anContext);
        }
      }
      if (theEvent.type == sf::Event::MouseMoved)
      {
        PropertyManager anContext;
        anContext.Add<IEntity*>("Entity", mEntity);
        anContext.Add<InputData>("InputData", mMoveBinding);
        anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(theEvent.mouseMove.x, theEvent.mouseMove.y));
        mApp.mEventManager.DoEvent(mMoveBinding.EventID, &anContext);
      }
    }
  }
  void MouseBinder::UpdateFixed()
  {
    if(mEntity!=NULL)
    {
      std::map<GQE::Uint32, InputData>::iterator anIter;
      for (anIter = mButtonBindings.begin(); anIter != mButtonBindings.end(); ++anIter)
      {
        if(anIter->second.Type==INPUT_REALTIME && sf::Mouse::isButtonPressed(sf::Mouse::Button(anIter->first)))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntity);
          anContext.Add<InputData>("InputData", anIter->second);
          anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(sf::Mouse::getPosition()));
          mApp.mEventManager.DoEvent(anIter->second.EventID,&anContext);
        }
      }
      if (mMoveBinding.Type == INPUT_REALTIME && (mLastPosition.x != sf::Mouse::getPosition().x || mLastPosition.y != sf::Mouse::getPosition().y))
      {
        PropertyManager anContext;
        anContext.Add<IEntity*>("Entity", mEntity);
        anContext.Add<InputData>("InputData", anIter->second);
        anContext.Add<sf::Vector2f>("InputPosition", sf::Vector2f(sf::Mouse::getPosition()));
        mApp.mEventManager.DoEvent(anIter->second.EventID, &anContext);
        mLastPosition = sf::Vector2f(sf::Mouse::getPosition());
      }
    }
  }
  void MouseBinder::RegisterButton(GQE::Uint32 theBinding, typeEventID theEventID, Uint8 theType, Uint32 theAction)
  {
    InputData anCommand;
    anCommand.EventID=theEventID;
    anCommand.Type=theType;
    anCommand.Action = theAction;
    mButtonBindings[theBinding]=anCommand;
  }
  void MouseBinder::RegisterMovement(typeEventID theEventID, Uint8 theType, Uint32 theAction)
  {
    InputData anCommand;
    anCommand.EventID = theEventID;
    anCommand.Type = theType;
    anCommand.Action = theAction;
    mMoveBinding = anCommand;
  }
  void MouseBinder::RegisterEntity(IEntity* theEntity)
  {
    if (theEntity!=NULL)
    {
      mEntity=theEntity;
    }
  }
}
