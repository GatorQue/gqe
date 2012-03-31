/**
 * Object Manager used for managing in game objects(any thing that inharets from the IObject class).
 * @file include/GQE/Entities/classes/ObjectManager.hpp
 * @author Jacob Dix.
 * @date 20110905 - Cleaned up and Commented Code.
 * @date 20110907 - Additional code and comment cleanup.
 */
#ifndef ENTITIES_OBJECT_MANAGER_HPP_INCLUDED
#define ENTITIES_OBJECT_MANAGER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Entities/Entities_types.hpp>
#include <queue>

namespace GQE
{
  /// Provides the Object manager class for managing all IObject derived classes
  class ObjectManager
  {
    public:
      /**
       * ObjectManager constructer.
       */
      ObjectManager();

      /**
       * ObjectManager deconstructer
       *
       */
      virtual ~ObjectManager();

      /**
       * AddObject Adds a object to the general object list and calls that objects DoInit functions
       * @param[in] theObject to be added
       */
      void AddObject(IObject* theObject);

      /**
       * RemoveObjects adds the object to the remove object list and calls that objects Deinit functions
       * @param[in] theObject to be removed
       */
      void RemoveObject(IObject* theObject);

      /**
       * AddCollision Adds a object to the Collision object list.
       */
      void CheckAllCollision(void);

      /**
       * CheckCollision will check the two collision polygon's provided to see
       * if a collision has occurred.
       * @param[in] theSourcePolygon to test with
       * @param[in] theOtherPolygon to test against
       * @return the collision result information
       */
      typeCollisionResult CheckCollision(CollisionPolygon* theSourcePolygon,
          CollisionPolygon* theOtherPolygon);

      /**
       * UpdateVariable is responsible for handling all Object fixed update
       * needs for all Objects.
       */
      void UpdateFixed(void);

      /**
       * UpdateVariable is responsible for handling all Object variable update
       * needs for all Objects.
       * @param[in] theElapsedTime since the last Draw was called
       */
      virtual void UpdateVariable(float theElapsedTime);

      /**
       * Clean clears and frees all the objects that are set to be removed. Also Adds objects in mAddlist.
       */
      void UpdateLists(void);

      /**
       * Clear Sets all Objects to be removed and deleted.
       */
      void ClearObjects(void);

      /**
       * Draw calls all the Draw commands for all objects in mObjectList.
       */
      void Draw(void);

    private:
      //The Main Object List.
      std::vector<IObject*> mObjectList;
      //Add and Remove lists.
      std::queue<IObject*> mAddList;
      std::queue<IObject*> mRemoveList;

      std::vector<IObject*> mCollisionList;
      std::vector<IObject*> mRenderList;

  };
} // namespace GQE

#endif // ENTITIES_OBJECT_MANAGER_HPP_INCLUDED
/**
 * @class GQE::ObjectManager
 * @ingroup Entities
 * The Object Manager class can handle all Game Objects(any object that inhairates from IObject) as well
 * as collision, updates and rendering for those objects.
 *
 * Copyright (c) 2011 Jacob Dix
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
