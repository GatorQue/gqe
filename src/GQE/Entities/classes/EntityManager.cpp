#include <GQE/Entities/classes/EntityManager.hpp>
namespace GQE
{
Entity* EntityManager::AddEntity(Entity* thePrototype)
{
    if(thePrototype==NULL)
        return NULL;
    Entity* anEntity=thePrototype->MakeCopy(mEntityCount);
    mEntityCount++;
    mEntityList.push_back(anEntity);
    return anEntity;
}
Entity* EntityManager::AddPrototype(typePrototypeID thePrototypeID)
{
    if(thePrototypeID=="")
        return NULL;
    Entity* anEntity=new Entity(thePrototypeID);
    mPrototypeList.push_back(anEntity);
    return anEntity;
}
Entity* EntityManager::GetEntity(typeEntityID theEntityID)
{
    if(theEntityID==0)
        return NULL;
    Entity* anEntity;
    std::vector<Entity*>::iterator anEntityIter;
    for(anEntityIter=mEntityList.begin(); anEntityIter!=mEntityList.end(); ++anEntityIter)
    {
        anEntity=*anEntityIter;
        if(anEntity->GetEntityID()==theEntityID)
        {
            return anEntity;
        }
    }
    return NULL;
}
Entity* EntityManager::GetPrototype(typePrototypeID thePrototypeID)
{
    if(thePrototypeID=="")
        return NULL;
    Entity* anEntity;
    std::vector<Entity*>::iterator anEntityIter;
    for(anEntityIter=mPrototypeList.begin(); anEntityIter!=mPrototypeList.end(); ++anEntityIter)
    {
        anEntity=*anEntityIter;
        if(anEntity->GetPrototypeID()==thePrototypeID)
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
}
