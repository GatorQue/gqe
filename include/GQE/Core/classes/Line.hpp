#ifndef GQE_LINE_HPP_INCLUDED
#define GQE_LINE_HPP_INCLUDED
#include <vector>
#include <SFML/System.hpp>
#include <GQE/Core/Core_types.hpp>
namespace GQE
{
class GQE_API Line
{
  public:
  Line():
  PointA(NULL),
  PointB(NULL)
  {

  }
  Line(sf::Vector2f* thePointA, sf::Vector2f* thePointB):
  PointA(thePointA),
  PointB(thePointB)
  {

  }
  sf::Vector2f LineNormal()
  {
    return(sf::Vector2f(PointB->y-PointA->y,-(PointB->x-PointA->x)));
  }
  bool Intersection(Line theLine)
  {
    sf::Vector2f CmP=sf::Vector2f(theLine.PointA->x-PointA->x,theLine.PointA->y-PointA->y);
    sf::Vector2f r=sf::Vector2f(PointB->x-PointA->x,PointB->y-PointA->y);
    sf::Vector2f s=sf::Vector2f(theLine.PointB->x-theLine.PointA->x,theLine.PointB->y-theLine.PointA->y);
    float CmPxr = CmP.x * r.y - CmP.y * r.x;
    float CmPxs = CmP.x * s.y - CmP.y * s.x;
    float rxs = r.x * s.y - r.y * s.x;

    if (CmPxr == 0)
		{
			// Lines are collinear, and so intersect if they have any overlap

			return ((theLine.PointA->x - PointA->x < 0) != (theLine.PointA->x - PointB->x < 0))
				|| ((theLine.PointA->y - PointA->y < 0) != (theLine.PointA->y - PointA->y < 0));
		}

		if (rxs == 0)
			return false; // Lines are parallel.

    float rxsr = 1 / rxs;
    float t = CmPxs * rxsr;
    float u = CmPxr * rxsr;

    return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
  }
  bool Intersection(std::vector<Line> theLines)
  {
    std::vector<Line>::iterator anIter;
    for(anIter=theLines.begin();anIter!=theLines.end();++anIter)
    {
      if(Intersection(*anIter))
      {
        return true;
      }
    }
    return false;
  }
  sf::Vector2f* PointA;
  sf::Vector2f* PointB;
};
}

#endif

