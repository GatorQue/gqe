#include <GQE/Entities/classes/EntityManager.hpp>
///Static Manager Funtions
std::vector<Entity*> EntityManager::mEntityList;
void EntityManager::AddEntity(typeEntityID theEntityID,int theNumber)
{
    if(theEntityID=="")
        return;
    for(int n=0;n<theNumber;n++)
    {
        mEntityList.push_back(new Entity(theEntityID));
    }
}
Entity* EntityManager::GetEntity(typeEntityID theEntityID)
{
    if(theEntityID=="")
        return NULL;
    Entity* anEntity;
    std::vector<Entity*>::iterator anEntityIter;
    for(anEntityIter=mEntityList.begin(); anEntityIter!=mEntityList.end(); ++anEntityIter)
    {
        anEntity=*anEntityIter;
        if(anEntity->GetID()==theEntityID)
        {
            return anEntity;
        }
    }
    return NULL;
}
void EntityManager::HandleEvents(sf::Event theEvent)
{
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
        (*it)->UpdateFixed();
    }
}
void EntityManager::UpdateFixed()
{
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
        (*it)->UpdateFixed();
    }
}
void EntityManager::UpdateVariable(float theElapsdTime)
{
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
        (*it)->UpdateVariable(theElapsdTime);
    }
}
void EntityManager::Draw()
{
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
        (*it)->Draw();
    }
}
AProperty* EntityManager::GetProperty(std::string theKey)
{
    if(theKey=="")
        return NULL;
    while(mPropertyList.find(theKey)!=mPropertyList.end())
        return NULL;

    return mPropertyList[theKey];
}

void EntityManager::SetProperty(AProperty* theProperty)
{
    if(theProperty->getLable()=="")
        return;
    if(mPropertyList.find(theProperty->getLable())!=mPropertyList.end())
    {
        if(mPropertyList[theProperty->getLable()]->getType()==theProperty->getType())
        {
            AProperty* anProperty=mPropertyList[theProperty->getLable()];
            delete anProperty;
        }
        else
        {
            //Log Error.
            return;
        }
    }
    mPropertyList[theProperty->getLable()]=theProperty;
}

void EntityManager::AttachComponent(IComponent* theComponent)
{
    if(theComponent==NULL)
        return;
    if(theComponent->GetID()=="")
        return;
    if(mComponentList.find(theComponent->GetID())!=mComponentList.end())
    {
        //log error
        return;
    }
    mComponentList[theComponent->GetID()]=theComponent;
    theComponent->DoInit(this);
}
void EntityManager::DetachComponent(typeComponentID theComponetID)
{
    if(theComponetID=="")
        return;
    if(mComponentList.find(theComponetID)==mComponentList.end())
        return;
    IComponent* anComponent=mComponentList[theComponetID];
    anComponent->DeInit();
}
}

