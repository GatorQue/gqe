#ifndef GQE_SHAPE_HPP_INCLUDED
#define GQE_SHAPE_HPP_INCLUDED
#include <GQE/Core/classes/Line.hpp>
#include <GQE/Core/interfaces/TVector2.hpp>
#include <SFML/Graphics.hpp>
namespace GQE
{
  class GQE_API IShape : public sf::Transformable
  {
    public:
      IShape();
			IShape(std::vector<Vector2f> thePoints);
			virtual ~IShape();
			bool Intersection(IShape& theOtherShape, Vector2f& theMinimumTranslation);
			IShape GetShape();
			void ProjectOntoAxis (const sf::Vector2f& theAxis, float& theMin, float& theMax);
			std::vector<Vector2f> mPoints;
      std::vector<Line> mLines;
    protected:
			Vector2f TransformPoint(Vector2f thePoint);
			void ConnectLines();
  };
}

#endif
