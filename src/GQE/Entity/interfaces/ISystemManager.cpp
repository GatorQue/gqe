/**
 * Provides the Instance class that is produced by the Prototype class.
 *
 * @file src/GQE/Entity/interfaces/ISystemManager.cpp>
 * @author Jacob Dix
 */
#include <GQE/Entity/interfaces/ISystemManager.hpp>
namespace GQE
{
	PrototypeManager ISystemManager::gPrototypeManager;
	ISystemManager::ISystemManager(IApp& theApp):
	mApp(theApp)
	{

	}
	ISystemManager::~ISystemManager()
	{

	}
}
