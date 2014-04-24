/**
* Provides a mouse binding interface
*
* @file src/GQE/Entity/classes/JoystickBinder.cpp
* @author Jacob Dix
* @date 20140423 - Initial Release
*/
#include <GQE/Entity/classes/JoystickBinder.hpp>

namespace GQE
{
  JoystickBinder::JoystickBinder(IApp& theApp) :
  mApp(theApp)
  {

  }
  void JoystickBinder::HandleEvents(sf::Event theEvent)
  {
    if(theEvent.type==sf::Event::JoystickButtonPressed||
      theEvent.type==sf::Event::JoystickButtonReleased)
    {
      typeJoystickData::iterator anIter;
      anIter = mButtonBindings.find(theEvent.joystickButton.joystickId);
      if (anIter != mButtonBindings.end())
      {
        std::map<Uint32,IEntity*>::iterator anEntityIter=mEntitys.find(theEvent.joystickButton.joystickId);
        std::map<Uint32,InputData> anJoystick=anIter->second;
        std::map<Uint32,InputData>::iterator anButtonIter=anJoystick.find(theEvent.joystickButton.button);
        if(anButtonIter!=anJoystick.end() && anEntityIter!=mEntitys.end())
        {
          if (((anButtonIter->second.Type == INPUT_PRESSED && theEvent.type == sf::Event::JoystickButtonPressed) ||
            (anButtonIter->second.Type == INPUT_RELEASED &&theEvent.type == sf::Event::JoystickButtonReleased))&&
            anButtonIter->second.EventID!="")
          {
            PropertyManager anContext;
            anContext.Add<IEntity*>("Entity",mEntitys[theEvent.joystickButton.joystickId]);
            anContext.Add<InputData>("InputData", anButtonIter->second);
            anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(0, 0));
            mApp.mEventManager.DoEvent(anButtonIter->second.EventID, &anContext);
          }
        }
      }
    }
    if(theEvent.type==sf::Event::JoystickMoved)
    {
      typeJoystickData::iterator anIter;
      anIter = mAxisBindings.find(theEvent.joystickMove.joystickId);
      if (anIter != mButtonBindings.end())
      {
        std::map<Uint32,IEntity*>::iterator anEntityIter=mEntitys.find(theEvent.joystickButton.joystickId);
        std::map<Uint32,InputData> anJoystick=anIter->second;
        std::map<Uint32,InputData>::iterator anMovmentIter=anJoystick.find(theEvent.joystickMove.axis);
        if(anMovmentIter!=anJoystick.end() && anEntityIter!=mEntitys.end())
        {
          if ((anMovmentIter->second.Type == INPUT_MOTION && theEvent.type == sf::Event::JoystickMoved) &&
            anMovmentIter->second.EventID!="")
          {
            PropertyManager anContext;

            anContext.Add<IEntity*>("Entity",mEntitys[theEvent.joystickMove.joystickId]);
            anContext.Add<InputData>("InputData", anMovmentIter->second);
            anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(theEvent.joystickMove.position*anMovmentIter->second.Axis.x,theEvent.joystickMove.position*anMovmentIter->second.Axis.y));
            mApp.mEventManager.DoEvent(anMovmentIter->second.EventID, &anContext);
          }
        }
      }
    }
  }
  void JoystickBinder::UpdateFixed()
  {
    typeJoystickData::iterator anIter;
    for (anIter = mButtonBindings.begin(); anIter != mButtonBindings.end(); ++anIter)
    {
      std::map<GQE::Uint32, InputData>::iterator anButtonIter;
      for (anButtonIter = anIter->second.begin(); anButtonIter != anIter->second.end(); ++anButtonIter)
      {
        if(anButtonIter->second.Type==INPUT_REALTIME && sf::Joystick::isButtonPressed(anIter->first,anButtonIter->first))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity",mEntitys[anIter->first]);
          anContext.Add<InputData>("InputData", anButtonIter->second);
          anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(0,0));
          mApp.mEventManager.DoEvent(anButtonIter->second.EventID,&anContext);
        }
      }

      for (anIter = mAxisBindings.begin(); anIter != mAxisBindings.end(); ++anIter)
      {
        std::map<GQE::Uint32, InputData>::iterator anMoveIter;
        for (anMoveIter = anIter->second.begin(); anMoveIter != anIter->second.end(); ++anMoveIter)
        {
          if(anMoveIter->second.Type==INPUT_REALTIME)
          {
            PropertyManager anContext;
            anContext.Add<IEntity*>("Entity",mEntitys[anIter->first]);
            anContext.Add<InputData>("InputData", anMoveIter->second);
            float anAxisPosition=sf::Joystick::getAxisPosition(anIter->first,sf::Joystick::Axis(anMoveIter->first));
            anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(anAxisPosition*anMoveIter->second.Axis.x,anAxisPosition*anMoveIter->second.Axis.y));
            mApp.mEventManager.DoEvent(anMoveIter->second.EventID,&anContext);
          }
        }
      }
    }
  }
  void JoystickBinder::RegisterButton(GQE::Uint32 theJoystick,GQE::Uint32 theButton, typeEventID theEventID, Uint8 theType, Uint32 theAction)
  {
    InputData anCommand;
    anCommand.EventID=theEventID;
    anCommand.Type=theType;
    anCommand.Action = theAction;
    anCommand.Axis=sf::Vector2f(0,0);
    mButtonBindings[theJoystick][theButton]=anCommand;
  }
  void JoystickBinder::RegisterAxis(GQE::Uint32 theJoystick,GQE::Uint32 theAxis, typeEventID theEventID, Uint8 theType, Uint32 theAction, sf::Vector2f theMoveAxis)
  {
    InputData anCommand;
    anCommand.EventID = theEventID;
    anCommand.Type = theType;
    anCommand.Action = theAction;
    anCommand.Axis = theMoveAxis;
    mAxisBindings[theJoystick][theAxis] = anCommand;
  }
  void JoystickBinder::RegisterEntity(Uint32 theJoystickID,IEntity* theEntity)
  {
    if (theEntity!=NULL)
    {
      mEntitys[theJoystickID]=theEntity;
    }
  }
}
