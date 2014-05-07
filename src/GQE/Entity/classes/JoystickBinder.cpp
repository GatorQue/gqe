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
      typeJoystickList::iterator anListIter;
      anListIter = mButtonBindings.find(theEvent.joystickButton.joystickId);
      if (anListIter != mButtonBindings.end())
      {
        std::map<Uint32,IEntity*>::iterator anEntityIter=mEntitys.find(theEvent.joystickButton.joystickId);
        typeInputDataList anJoystick = anListIter->second;
        typeInputDataList::iterator anButtonIter = anJoystick.find(theEvent.joystickButton.button);
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
      typeJoystickList::iterator anListIter;
      anListIter = mAxisBindings.find(theEvent.joystickMove.joystickId);
      if (anListIter != mAxisBindings.end())
      {
        std::map<Uint32,IEntity*>::iterator anEntityIter=mEntitys.find(theEvent.joystickButton.joystickId);
        std::map<Uint32, InputData> anJoystick = anListIter->second;
        typeInputDataList::iterator anMovmentIter = anJoystick.find(theEvent.joystickMove.axis);
        if(anMovmentIter!=anJoystick.end() && anEntityIter!=mEntitys.end())
        {
          if ((anMovmentIter->second.Type == INPUT_MOTION && theEvent.type == sf::Event::JoystickMoved) &&
            anMovmentIter->second.EventID!="")
          {
            PropertyManager anContext;

            anContext.Add<IEntity*>("Entity",mEntitys[theEvent.joystickMove.joystickId]);
            anContext.Add<InputData>("InputData", anMovmentIter->second);
            float anAxisPosition = theEvent.joystickMove.position;
            sf::Vector2f anPosition(0, 0);
            if ((anMovmentIter->second.Axis & AXIS_HORZ) == AXIS_HORZ)
            {
              anPosition.x = anAxisPosition;
            }
            if ((anMovmentIter->second.Axis & AXIS_VERT) == AXIS_VERT)
            {
              anPosition.y = anAxisPosition;
            }
            anContext.Add<sf::Vector2f>("vInputPosition", anPosition);
            mApp.mEventManager.DoEvent(anMovmentIter->second.EventID, &anContext);
          }
        }
      }
    }
  }
  void JoystickBinder::UpdateFixed()
  {
    typeJoystickList::iterator anListIter;
    for (anListIter = mButtonBindings.begin(); anListIter != mButtonBindings.end(); ++anListIter)
    {
      typeInputDataList::iterator anButtonIter;
      for (anButtonIter = anListIter->second.begin(); anButtonIter != anListIter->second.end(); ++anButtonIter)
      {
        if (anButtonIter->second.Type == INPUT_REALTIME && sf::Joystick::isButtonPressed(anListIter->first, anButtonIter->first))
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity", mEntitys[anListIter->first]);
          anContext.Add<InputData>("InputData", anButtonIter->second);
          anContext.Add<sf::Vector2f>("vInputPosition", sf::Vector2f(0, 0));
          mApp.mEventManager.DoEvent(anButtonIter->second.EventID, &anContext);
        }
      }
    }
    for (anListIter = mAxisBindings.begin(); anListIter != mAxisBindings.end(); ++anListIter)
    {
      std::map<GQE::Uint32, InputData>::iterator anMovmentIter;
      for (anMovmentIter = anListIter->second.begin(); anMovmentIter != anListIter->second.end(); ++anMovmentIter)
      {
        if (anMovmentIter->second.Type == INPUT_REALTIME)
        {
          PropertyManager anContext;
          anContext.Add<IEntity*>("Entity", mEntitys[anListIter->first]);
          anContext.Add<InputData>("InputData", anMovmentIter->second);
          float anAxisPosition = sf::Joystick::getAxisPosition(anListIter->first, (sf::Joystick::Axis)anMovmentIter->first);
          sf::Vector2f anPosition(0, 0);
          if ((anMovmentIter->second.Axis & AXIS_HORZ) == AXIS_HORZ)
          {
            anPosition.x = anAxisPosition;
          }
          if ((anMovmentIter->second.Axis & AXIS_VERT) == AXIS_VERT)
          {
            anPosition.y = anAxisPosition;
          }
          anContext.Add<sf::Vector2f>("vInputPosition", anPosition);
          mApp.mEventManager.DoEvent(anMovmentIter->second.EventID, &anContext);
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
    anCommand.Axis = AXIS_NONE;
    mButtonBindings[theJoystick][theButton]=anCommand;
  }
  void JoystickBinder::RegisterAxis(GQE::Uint32 theJoystick,GQE::Uint32 theAxis, typeEventID theEventID, Uint8 theType, Uint32 theAction, Uint32 theMoveAxis)
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
