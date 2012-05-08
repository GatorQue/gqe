#include "GQE/Entities/Entities_types.hpp"
#include "GQE/Entities/classes/Entity.hpp"
namespace GQE
{
  void setInt(Entity* theEntity, std::string theLable, int theValue)
  {
    if(theEntity==NULL || theLable=="")
      return;
    TProperty<int>* anPropterty=new TProperty<int>(NULL);
    anPropterty->setLable(theLable);
    anPropterty->setValue(theValue);
    theEntity->SetProperty(anPropterty);
  }
  int getInt(Entity* theEntity, std::string theLable, int theDefault)
  {
    AProperty* Propterty=NULL;
    Propterty=theEntity->GetProperty(theLable);
    if(Propterty==NULL)
      //Logerror
      return theDefault;
    if(typeid(TProperty<int>).name()!=Propterty->getType()->Name())
      return theDefault;
    return static_cast<TProperty<int>*>(Propterty)->getValue();
  }

  void setString(Entity* theEntity, std::string theLable, std::string theValue)
  {
    if(theEntity==NULL || theLable=="")
      return;
    TProperty<std::string>* anPropterty=new TProperty<std::string>(NULL);
    anPropterty->setLable(theLable);
    anPropterty->setValue(theValue);
    theEntity->SetProperty(anPropterty);
  }
  std::string getString(Entity* theEntity, std::string theLable, std::string theDefault)
  {
    AProperty* Propterty=NULL;
    Propterty=theEntity->GetProperty(theLable);
    if(Propterty==NULL)
      //Logerror
      return theDefault;
    if(typeid(TProperty<std::string>).name()!=Propterty->getType()->Name())
      return theDefault;
    return static_cast<TProperty<std::string>*>(Propterty)->getValue();
  }
}
