/**
* The IComponent interface class used for all derived IComponent entities that
* can be added to an IEntity class.
*
* @file include/GQE/Entity/interfaces/IComponent.hpp
* @author Jacob Dix
* @date 20120417 - Initial Release
*/
#ifndef RENDER_SYSTEM_HPP_INCLUDED
#define RENDER_SYSTEM_HPP_INCLUDED

#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
	/// The component interface class used by all IEntity derived classes
	class GQE_API RenderSystem : public ISystem
	{
	public:
		/**
		RenderSystem Constructor.
		@param[in] theApp is the Current GQE App.
		**/
		RenderSystem(IApp& theApp);

		/**
		ISystem Destructor.
		**/
		~RenderSystem();
		/**
		* InitInstance is called when an instance is added to the system.
		* @Param[in] theInstance is the new instance added.
		*/
		virtual void InitInstance(Instance* theInstance);
		/**
		* RegisterPrototype is responsible for adding the properties to a prototype.
		* @param[in] thePrototype is the prototype to use.
		*/
		virtual void RegisterPrototype(Prototype* thePrototype);
		
		/**
		* HandleEvents is responsible for letting each Instance class have a
		* chance to handle theEvent specified.
		* @param[in] theEvent to handle
		*/
		virtual void HandleEvents(sf::Event theEvent);

		/**
		* UpdateFixed is called a specific number of times every game loop and
		* this method will allow each Instance class a chance to have its
		* UpdateFixed method called for each game loop iteration.
		*/
		virtual void UpdateFixed(void);

		/**
		* UpdateVariable is called every time the game loop draws a frame and
		* includes the elapsed time between the last UpdateVariable call for
		* use with equations that use time as a variable. (e.g. physics velocity
		* and acceleration equations).
		*/
		virtual void UpdateVariable(float theElapsedTime);

		/**
		* Draw is called during the game loop after events and the fixed update
		* loop calls are completed and depends largely on the speed of the
		* computer to determine how frequently it will be called. This gives the
		* EntityManager a chance to call the Draw method for each Instance
		* class.
		*/
		virtual void Draw(void);

		/**
		* HandleCleanup will be called during the game loop or IState shutdown
		* process and gives a chance for each Instance and Prototype class an
		* opportunity to clean up any loose ends and free up memory resources.
		*/
		virtual void HandleCleanup(void);
	private:
		struct typeRenderData
		{
			ImageAsset RenderImage;
			sf::Sprite RenderSprite;	
			typeInstanceID RenderID;
		};
		std::vector<typeRenderData> mRenderList;
	};
} // namespace GQE

#endif // ICOMPONENT_HPP_INCLUDED

/**
* @class GQE::ISystem
* @ingroup Entity
* The ISystem class represents the interface used by all Components that
* can be added to an IEntity derived class. Components enable IEntity classes
* to differ from each other without requiring the game designer to create
* complicated inheritance models for all their game entities.
*
* Copyright (c) 2010-2012 Jacob Dix
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

