#include <GQE/Entities/classes/EntityManager.hpp>
namespace GQE
{
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
AProperty* EntityManager::GetProperty(typeEntityID theEntityID,std::string theLable)
{
    if(theEntityID=="")
        return NULL;
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
		if(theEntityID==(*it)->GetID())
		{
			return (*it)->GetProperty(theLable);
		}
    }
	return NULL;
}

void EntityManager::SetProperty(typeEntityID theEntityID, AProperty* theProperty)
{
    if(theEntityID=="")
        return;
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
		if(theEntityID==(*it)->GetID())
		{
			(*it)->SetProperty(theProperty);
		}
    }
}

void EntityManager::AttachComponent(typeEntityID theEntityID, IComponent* theComponent)
{
    if(theEntityID=="")
        return;
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
		if(theEntityID==(*it)->GetID())
		{
			(*it)->AttachComponent(theComponent);
		}
    }
}
void EntityManager::DetachComponent(typeEntityID theEntityID,typeComponentID theComponetID)
{
    if(theEntityID=="")
        return;
    std::vector<Entity*>::iterator it;
    for(it=mEntityList.begin(); it!=mEntityList.end(); ++it)
    {
		if(theEntityID==(*it)->GetID())
		{
			(*it)->DetachComponent(theComponetID);
		}
    }
}
}