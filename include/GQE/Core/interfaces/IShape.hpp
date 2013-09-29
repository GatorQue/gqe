#ifndef GQE_SHAPE_HPP_INCLUDED
#define GQE_SHAPE_HPP_INCLUDED
#include <GQE/Core/classes/Line.hpp>
#include <SFML/Graphics.hpp>
namespace GQE
{
  class GQE_API IShape : public sf::Transformable
  {
    public:
      IShape();
			IShape(std::vector<sf::Vector2f> thePoints);
			virtual ~IShape();
			bool Intersection(IShape& theOtherShape, sf::Vector2f& theMinimumTranslation);
			IShape GetShape();
			void ProjectOntoAxis (const sf::Vector2f& theAxis, float& theMin, float& theMax);
			std::vector<sf::Vector2f> mPoints;
      std::vector<Line> mLines;
    protected:
			sf::Vector2f TransformPoint(sf::Vector2f thePoint);
			void ConnectLines();
  };
}

#endif
