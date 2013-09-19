/**
 * Provides the ISystemManager For handling multiple entity systems.
 *
 * @file include/GQE/Entity/interfaces/ISystemManager.hpp>
 * @author Jacob Dix
 */
#ifndef ISYSTEM_MANAGER_HPP_INCLUDED
#define ISYSTEM_MANAGER_HPP_INCLUDED
#include <GQE/Entity/Entity_types.hpp>
#include <GQE/Entity/classes/PrototypeManager.hpp>
namespace GQE
{
	class GQE_API ISystemManager
	{
	public:
		//Prototype Manager
		static GQE::PrototypeManager gPrototypeManager;
		ISystemManager(IApp& theApp);
		~ISystemManager();
		virtual void InitPrototypes()=0;
	private:
		IApp& mApp;	
	};
}

#endif