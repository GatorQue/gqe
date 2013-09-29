#ifndef GQE_VECTOR2_HPP_INCLUDED
#define GQE_VECTOR2_HPP_INCLUDED
#include <SFML/System.hpp>
#include <GQE/Core/Core_types.hpp>
namespace GQE
{
  template<typename T>
  class TVector2 : public sf::Vector2<T>
  {
   public:
		TVector2()
		{
			sf::Vector2<T>();
		}
		TVector2(T theX, T theY)
		{
			x=theX;
			y=theY;
		}
    TVector2<T> Normalize()
    {
      T anMagnitude=std::sqrt((x*x)+(y*y));
      sf::Vector2<T> anNormal=sf::Vector2<T>(x/anMagnitude,y/anMagnitude);
      x=anNormal.x;
      y=anNormal.y;
			return TVector2<T>(x,y);
    }
    T dotProduct(sf::Vector2<T> theVector)
    {
      return ((x*theVector.x)+(y*theVector.y));
		}
  };
  // Define the most common types
  typedef TVector2<int>          Vector2i;
  typedef TVector2<unsigned int> Vector2u;
  typedef TVector2<float>        Vector2f;
}


#endif
