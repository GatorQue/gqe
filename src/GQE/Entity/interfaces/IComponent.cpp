#include <assert.h>
#include "GQE/Entity/interfaces/IComponent.hpp"
#include "GQE/Entity/classes/Entity.hpp"
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
	IComponent::IComponent(const typeComponentID theComponentID , App& theApp,EntityManager* theEntityManager) :
mApp(theApp),
	mEntity(NULL),
	mEntityManager(theEntityManager),
	mComponentID(theComponentID),
	mInit(false),
	mCleanup(false),
	mRemove(false)
{
	ILOG() << "IComponent::ctor(" << mComponentID << ")" << std::endl;
}
IComponent::~IComponent()
{
	ILOG() << "IComponent::dtor(" << mComponentID << ")" << std::endl;
}

const typeComponentID IComponent::GetID(void) const
{
	return mComponentID;
}

void IComponent::DoInit(Entity* theEntity)
{
	ILOG() << "IComponent::DoInit(" << mComponentID << ")" << std::endl;
	mEntity=theEntity;
	// If Cleanup hasn't been called yet, call it now!
	if(true == mCleanup)
	{
		HandleCleanup();
	}
	// Initialize if necessary
	if(false == mInit)
	{
		mInit = true;
	}
}

void IComponent::DeInit(void)
{
	ILOG() << "IState::DeInit(" << mComponentID << ")" << std::endl;

	if(true == mInit)
	{
		mCleanup = true;
		mInit = false;
	}
}

bool IComponent::IsInitComplete(void)
{
	return mInit;
}
bool IComponent::CanRemove(void)
{
	return mRemove;
}
void IComponent::HandleCleanup(void)
{
	if(true == mCleanup)
	{
		// Call cleanup
		Cleanup();

		// Clear our cleanup flag
		mCleanup = false;

		//Ready to be removed
		mRemove = true;
	}
}

void IComponent::Cleanup(void)
{
	ILOG() << "IComponent::Cleanup(" << mComponentID << ")" << std::endl;
}

}
