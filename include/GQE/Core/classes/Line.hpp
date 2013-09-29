#ifndef GQE_LINE_HPP_INCLUDED
#define GQE_LINE_HPP_INCLUDED
#include <GQE/Core/interfaces/TVector2.hpp>
#include <vector>
namespace GQE
{
class Line
{
  public:
  Line():
  PointA(NULL),
  PointB(NULL)
  {

  }
  Line(Vector2f* thePointA, Vector2f* thePointB):
  PointA(thePointA),
  PointB(thePointB)
  {

  }
  Vector2f LineNormal()
  {
    return(Vector2f(PointB->y-PointA->y,-(PointB->x-PointA->x)));
  }
  bool Intersection(Line theLine)
  {
    Vector2f CmP=Vector2f(theLine.PointA->x-PointA->x,theLine.PointA->y-PointA->y);
    Vector2f r=Vector2f(PointB->x-PointA->x,PointB->y-PointA->y);
    Vector2f s=Vector2f(theLine.PointB->x-theLine.PointA->x,theLine.PointB->y-theLine.PointA->y);
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
  Vector2f* PointA;
  Vector2f* PointB;
};
}

#endif

