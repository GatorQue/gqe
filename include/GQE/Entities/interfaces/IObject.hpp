/**
 * The base class for a game object. This object includes Position, Velocity, Acceleration, Rotation,
 * and Scale. All of which are 3 dementional vectors(this will allow easy support for any 3d library
 * to use these objects)
 * @file include/GQE/Entities/interfaces/IObject.hpp
 * @author Jacob Dix
 * @date 20111205 - Inital release.
 * @date 20111208 - Fixed up and organized
 * @date 20110209 - Combined IObject.hpp and IObject.cpp to conform to the GQE Coding Standards.
 * @date 20110907 - Split IObject.hpp into IObject.cpp (and changed GQE Coding Standard)
 * @date 20110907 - Additional code and comment cleanup.
 */
#ifndef ENTITIES_IOBJECT_HPP_INCLUDED
#define ENTITIES_IOBJECT_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Entities/Entities_types.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace GQE
{
/// Provides the base class interface for all Object classes
class GQE_API IObject
{
public:
    /**
     * IObject deconstructor
     */
    virtual ~IObject();

    /**
     * DoInit is responsible for initializing this State
     */
    virtual void DoInit();

    /**
     * DeInit is responsible for marking this state to be cleaned up
     */
    void DeInit();

    /**
     * Access Methods
     */
    void Rotate(float theX, float theY, float theZ);

    /**
     * GetName will return the name of this object.
     * @return the name of this object
     */
    typeObjectID GetID(void);

    /**
     * GetPosition will return the position of this object.
     * @return the position of this object as a 3D vector
     */
    sf::Vector3f GetPosition(void);

    /**
     * GetRotation will return the rotation of this object.
     * @return the rotation of this object as a 3D vector
     */
    sf::Vector3f GetRotation(void);

    /**
     * GetScale will return the scale of this object.
     * @return the scale of this object as a 3D vector
     */
    sf::Vector3f GetScale(void);

    /**
     * SetPosition will set the position of this object to theX, theY, and theZ
     * coordinates provided.
     * @param[in] theX position value to set this object to
     * @param[in] theY position value to set this object to
     * @param[in] theZ position value to set this object to
     */
    void SetPosition(float theX, float theY, float theZ);

    /**
     * SetRotation will set the rotation of this object to theX, theY, and theZ
     * coordinates provided.
     * @param[in] theX rotation value to set this object to
     * @param[in] theY rotation value to set this object to
     * @param[in] theZ rotation value to set this object to
     */
    void SetRotation(float theX, float theY, float theZ);

    /**
     * SetScale will set the scale of this object to theX, theY, and theZ
     * scales provided.
     * @param[in] theX scale value to set this object to
     * @param[in] theY scale value to set this object to
     * @param[in] theZ scale value to set this object to
     */
    void SetScale(float theX, float theY, float theZ);

    /**
     * SetVelocity will set the velocity of this object to theX, theY, and theZ
     * velocities provided.
     * @param[in] theX velocity value to set this object to
     * @param[in] theY velocity value to set this object to
     * @param[in] theZ velocity value to set this object to
     */
    void SetVelocity(float theX, float theY, float theZ);

    /**
     * GetScale will return the velocity of this object.
     * @return the velocity of this object as a 3D vector
     */
    sf::Vector3f GetVelocity(void);

    /**
     * SetAcceleration will set the acceleration of this object to theX, theY,
     * and theZ accelerations provided.
     * @param[in] theX acceleration value to set this object to
     * @param[in] theY acceleration value to set this object to
     * @param[in] theZ acceleration value to set this object to
     */
    void SetAcceleration(float theX, float theY, float theZ);

    /**
     * GetScale will return the Acceleration of this object.
     * @return the Acceleration of this object as a 3D vector
     */
    sf::Vector3f GetAcceleration(void);

    /**
     * GetSolid will return true if this object is solid.
     * @return true if this object is solid
     */
    unsigned char GetFlags(void);

    /**
     * SetSprite will set the sprite for this object using theSpriteName
     * provided to find the Sprite.
     * @param[in] theSpriteName to use to find the Sprite in AssetManager
     */
    void SetSprite(std::string theSpriteName);

    sf::Sprite* GetSprite();

    /**
     * HandleEvents will be called to allow the object to handle events
     * @param[in] theEvent to be handled.
     */
    virtual void HandleEvents(sf::Event theEvent);

    /**
     * Collision can be called to test if another object collides with this
     * object.
     * @param[in] theOtherObject to test against
     * @param[out] theResult of the collision test
     */
    virtual void Collision(IObject* theOtherObject, typeCollisionResult theResult);

    /**
     * UpdateFixed is responsible for handling all fixed update needs for this
     * Object.
     */
    virtual void UpdateFixed(void);

    /**
     * UpdateVariable is responsible for handling all variable update needs for
     * this Object.
     */
    virtual void UpdateVariable(float theElapsedTime);

    /**
     * Draw is called to draw this Object.
     */
    virtual void Draw(void);

    /**
     * SetBoundingRect sets the bounding rect.
     * @param[in] theRect to use for collision testing
     */
    void SetBoundingRect(sf::FloatRect theRect);

    /**
     * GetCollisionPolygon will return the current polygon being used for
     * collision detection tests against this object.
     * @return the collision polygon being used for collision detection
     */
    sf::FloatRect GetBoundingRect(void);

    /**
     * HandleCleanup is responsible for calling Cleanup if this class has been
     * flagged to be cleaned up after it completes the game loop.
     */
    void HandleCleanup(void);
protected:
    //Pointer to the App object
    App* mApp;

    // Variables
    ///////////////////////////////////////////////////////////////////////////
    ///The name of the object
    typeObjectID mObjectID;
    ///Position in three demensions.
    sf::Vector3f mPosition;
	///Last Frames Position
    sf::Vector3f mOldPosition;
    ///Velocity in three demensions. Changes Position.
    sf::Vector3f mVelocity;
	///Acceleration in three demensions. Changes Velocity.
    sf::Vector3f mAcceleration;
	///Rotation in three demensions.
    sf::Vector3f mRotation;
	///Scale in three demensions(currently does nothing).
    sf::Vector3f mScale;
    ///SFML Sprite
    sf::Sprite* mSprite;
	std::string mSpriteName;

    ///Bounding rect
    sf::FloatRect mBoundRect;
    sf::FloatRect mCurrentRect;
    /**
     * IObject constructor
     */
    IObject(App* theApp, typeObjectID theObjectID, unsigned char theFlags);



    /**
     * Cleanup is responsible for performing any cleanup required before
     * this State is removed.
     */
    virtual void Cleanup(void);

private:
    ///Object Flags
    bool mInit;
    bool mCleanup;
    unsigned char mFlags;
    /**
     * Our copy constructor is private because we do not allow copies of
     * our Singleton class
     */
    IObject(const IObject&);  // Intentionally undefined

    /**
     * Our assignment operator is private because we do not allow copies
     * of our Singleton class
     */
    IObject& operator=(const IObject&); // Intentionally undefined
};
} // namespace GQE

#endif  // ENTITIES_IOBJECT_HPP_INCLUDED

/**
 * @class GQE::IObject
 * @ingroup Entities
 * This is the base object class. It has code for basic image rendering and
 * basic movement, rotation, scaling, etc.
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
