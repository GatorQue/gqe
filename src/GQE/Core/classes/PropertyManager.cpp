/**
 * Provides a class that manages all properties for an IEntity. This enables
 * using the PropertyManager class in another class than IEntity if desired.
 *
 * @file src/GQE/Core/classes/PropertyManager.cpp
 * @author Jacob Dix
 * @date 20120609 - Initial Release
 * @date 20120620 - Remove excess warning message when adding existing properties
 * @date 20120702 - Fix variable misspelling with iterators
 */

#include <GQE/Core/classes/PropertyManager.hpp>
#include <GQE/Core/utils/StringUtil.hpp>
namespace GQE
{
  PropertyManager::PropertyManager()
  {
  }

  PropertyManager::~PropertyManager()
  {
    // Make sure to remove all registered properties on desstruction
    std::map<const typePropertyID, IProperty*>::iterator anPropertyIter;
    for(anPropertyIter = mList.begin();
        anPropertyIter != mList.end();
        ++anPropertyIter)
    {
      IProperty* anProperty = (anPropertyIter->second);
      delete anProperty;
      anProperty = NULL;
    }
  }

  bool PropertyManager::HasID(const typePropertyID thePropertyID) const
  {
    bool anResult = false;

    // See if thePropertyID was found in our list of properties
    anResult = (mList.find(thePropertyID) != mList.end());

    // Return true if thePropertyID was found above, false otherwise
    return anResult;
  }


  void PropertyManager::Add(IProperty* theProperty)
  {
    if(mList.find(theProperty->GetID()) == mList.end())
    {
      mList[theProperty->GetID()] = theProperty;
    }
  }

  void PropertyManager::Clone(const PropertyManager& thePropertyManager)
  {
    // Make sure to remove all registered properties on desstruction
    std::map<const typePropertyID, IProperty*>::const_iterator anPropertyIter;
    for(anPropertyIter = thePropertyManager.mList.begin();
        anPropertyIter != thePropertyManager.mList.end();
        ++anPropertyIter)
    {
      IProperty* anProperty = (anPropertyIter->second);
      Add(anProperty->MakeClone());
    }
  }

  void PropertyManager::ParseProperty(std::string thePropertyName, std::string thePropertyValue)
  {
    std::string anLowerName=GQE::StringToLowercase(thePropertyName);
    char anType=anLowerName.at(0);
    // Is this a boolean property value? then its name will start with b or B
    if(anType == 'b')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        bool anPrevious = Get<bool>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<bool>(thePropertyName,
            GQE::ParseBool(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<bool>(thePropertyName,
            GQE::ParseBool(thePropertyValue, false));
      }
    }
    // Is this a color value? then its name will start with c or C
    else if(anType == 'c')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Color anPrevious = Get<sf::Color>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Color>(thePropertyName,
            GQE::ParseColor(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a sf::Color property value
        Add<sf::Color>(thePropertyName,
            GQE::ParseColor(thePropertyValue, sf::Color(0,0,0,0)));
      }
    }
    // Is this a float value? then its name will start with f or F
    else if(anType == 'f')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        float anPrevious = Get<float>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<float>(thePropertyName,
            GQE::ParseFloat(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a float property value
        Add<float>(thePropertyName,
            GQE::ParseFloat(thePropertyValue, 0.0f));
      }
    }
    // Is this a signed numeric property value? then its name will start with i or I
    else if(anType == 'i')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        GQE::Int32 anPrevious = Get<GQE::Int32>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<GQE::Int32>(thePropertyName,
            GQE::ParseInt32(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<GQE::Int32>(thePropertyName,
            GQE::ParseInt32(thePropertyValue, 0));
      }
    }
    // Is this an sf::IntRect property value? then its name will start with r or R
    if(anType == 'r')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::IntRect anPrevious = Get<sf::IntRect>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::IntRect>(thePropertyName,
            GQE::ParseIntRect(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::IntRect>(thePropertyName,
            GQE::ParseIntRect(thePropertyValue, sf::IntRect(0,0,0,0)));
      }
    }
    // Is this an unsigned numeric property value? then its name will start with u or U
    if(anType == 'u')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        GQE::Uint32 anPrevious = Get<GQE::Uint32>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<GQE::Uint32>(thePropertyName,
            GQE::ParseUint32(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<GQE::Uint32>(thePropertyName,
            GQE::ParseUint32(thePropertyValue, 0));
      }
    }
    // Is this a sf::Vector2f property value? then its name will start with v or V
    if(anType == 'v')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector2f anPrevious = Get<sf::Vector2f>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector2f>(thePropertyName,
            GQE::ParseVector2f(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector2f>(thePropertyName,
            GQE::ParseVector2f(thePropertyValue, sf::Vector2f(0.0f,0.0f)));
      }
    }
    // Is this a sf::Vector2u property value? then its name will start with w or W
    if(anType == 'w')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector2u anPrevious = Get<sf::Vector2u>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector2u>(thePropertyName,
            GQE::ParseVector2u(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector2u>(thePropertyName,
            GQE::ParseVector2u(thePropertyValue, sf::Vector2u(0,0)));
      }
    }
    // Is this a sf::Vector3f property value? then its name will start with z or Z
    if(anType == 'z')
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector3f anPrevious = Get<sf::Vector3f>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector3f>(thePropertyName,
            GQE::ParseVector3f(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector3f>(thePropertyName,
            GQE::ParseVector3f(thePropertyValue, sf::Vector3f(0.0f,0.0f,0.0f)));
      }
    }
    // Otherwise assume its a string property
    else
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Set the new value as the string value provided
        Set<std::string>(thePropertyName, thePropertyValue);
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<std::string>(thePropertyName, thePropertyValue);
      }
    }
  }
  void PropertyManager::ParseProperty(std::string thePropertyName, std::string thePropertyValue, std::string thePropertyType)
  {
    std::string theLowerType=StringToLowercase(thePropertyType);
    // Is this a boolean property value? then its name will start with b or B
    if(theLowerType == "bool")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        bool anPrevious = Get<bool>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<bool>(thePropertyName,
            GQE::ParseBool(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<bool>(thePropertyName,
            GQE::ParseBool(thePropertyValue, false));
      }
    }
    // Is this a color value? then its name will start with c or C
    else if(theLowerType=="color")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Color anPrevious = Get<sf::Color>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Color>(thePropertyName,
            GQE::ParseColor(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a sf::Color property value
        Add<sf::Color>(thePropertyName,
            GQE::ParseColor(thePropertyValue, sf::Color(0,0,0,0)));
      }
    }
    // Is this a float value? then its name will start with f or F
    else if(theLowerType=="float")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        float anPrevious = Get<float>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<float>(thePropertyName,
            GQE::ParseFloat(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a float property value
        Add<float>(thePropertyName,
            GQE::ParseFloat(thePropertyValue, 0.0f));
      }
    }
    // Is this a signed numeric property value? then its name will start with i or I
    else if(theLowerType=="int" || theLowerType=="int32")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        GQE::Int32 anPrevious = Get<GQE::Int32>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<GQE::Int32>(thePropertyName,
            GQE::ParseInt32(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<GQE::Int32>(thePropertyName,
            GQE::ParseInt32(thePropertyValue, 0));
      }
    }
    // Is this an sf::IntRect property value? then its name will start with r or R
    if(theLowerType=="rect"||theLowerType=="intrect")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::IntRect anPrevious = Get<sf::IntRect>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::IntRect>(thePropertyName,
            GQE::ParseIntRect(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::IntRect>(thePropertyName,
            GQE::ParseIntRect(thePropertyValue, sf::IntRect(0,0,0,0)));
      }
    }
    // Is this an unsigned numeric property value? then its name will start with u or U
    if(theLowerType=="uint" || theLowerType=="uint32")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        GQE::Uint32 anPrevious = Get<GQE::Uint32>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<GQE::Uint32>(thePropertyName,
            GQE::ParseUint32(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<GQE::Uint32>(thePropertyName,
            GQE::ParseUint32(thePropertyValue, 0));
      }
    }
    // Is this a sf::Vector2f property value? then its name will start with v or V
    if(theLowerType=="vector" || theLowerType =="vector2" || theLowerType== "vector2f")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector2f anPrevious = Get<sf::Vector2f>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector2f>(thePropertyName,
            GQE::ParseVector2f(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector2f>(thePropertyName,
            GQE::ParseVector2f(thePropertyValue, sf::Vector2f(0.0f,0.0f)));
      }
    }
    // Is this a sf::Vector2u property value? then its name will start with w or W
    if(theLowerType=="vector2u")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector2u anPrevious = Get<sf::Vector2u>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector2u>(thePropertyName,
            GQE::ParseVector2u(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector2u>(thePropertyName,
            GQE::ParseVector2u(thePropertyValue, sf::Vector2u(0,0)));
      }
    }
    // Is this a sf::Vector3f property value? then its name will start with z or Z
    if(theLowerType=="vector3" || theLowerType=="vector3f")
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Get the previous value and use it as the default
        sf::Vector3f anPrevious = Get<sf::Vector3f>(thePropertyName);
        // Now set the new value by parsing the string provided
        Set<sf::Vector3f>(thePropertyName,
            GQE::ParseVector3f(thePropertyValue, anPrevious));
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<sf::Vector3f>(thePropertyName,
            GQE::ParseVector3f(thePropertyValue, sf::Vector3f(0.0f,0.0f,0.0f)));
      }
    }
    // Otherwise assume its a string property
    else
    {
      // If the property already exists then replace it by setting a new property value
      if(HasID(thePropertyName))
      {
        // Set the new value as the string value provided
        Set<std::string>(thePropertyName, thePropertyValue);
      }
      // Otherwise just add the new property value
      else
      {
        // Create a boolean property by associating 0 as a false value, otherwise true
        Add<std::string>(thePropertyName, thePropertyValue);
      }
    }
  }

} // namespace GQE

/**
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
