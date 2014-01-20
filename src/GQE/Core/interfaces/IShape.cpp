#include <GQE/Core/interfaces/IShape.hpp>
#include <GQE/Core/utils/MathUtil.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <limits>
namespace GQE
{
	IShape::IShape():
		Transformable()
	{

	}
	IShape::IShape(std::vector<sf::Vector2f> thePoints)
	{
		mPoints=thePoints;
		ConnectLines();
	}
	IShape::~IShape()
	{

	}
	IShape IShape::GetShape()
	{
		std::vector<sf::Vector2f> anPoints;
		for(int i=0;i<mPoints.size();i++)
		{
			anPoints.push_back(TransformPoint(mPoints[i]));
		}
		return IShape(anPoints);
	}
  sf::ConvexShape IShape::GetDrawableShape()
  {
    sf::ConvexShape anConvexShape(mPoints.size());
    for (int i = 0; i<mPoints.size(); i++)
    {
      anConvexShape.setPoint(i, TransformPoint(mPoints[i]));
    }
    return anConvexShape;
  }
	sf::Vector2f IShape::GetSize()
	{
		sf::Vector2f anXAxis(1,0), anYAxis(0,1);
		float anMinX, anMinY, anMaxX, anMaxY;
		//GetWidth
		for(int i=0;i<mPoints.size();++i)
		{
			ProjectOntoAxis(anXAxis, anMinX, anMaxX);
		}
		//GetHeight
		for(int i=0;i<mPoints.size();++i)
		{
			ProjectOntoAxis(anYAxis, anMinY, anMaxY);
		}
		return sf::Vector2f(anMaxX-anMinX,anMaxY-anMinY);
	}
	sf::Vector2f IShape::TransformPoint(sf::Vector2f thePoint)
	{
		return getTransform().transformPoint((sf::Vector2f)thePoint);
	}
	bool IShape::Intersection(IShape& theOtherShape, sf::Vector2f& theMinimumTranslation)
	{
		//Exit if either shape is empty;
		if(theOtherShape.mPoints.empty() || mPoints.empty())
			return false;
		//Update Point Positions before Test.
		IShape anShapeA=GetShape();
		anShapeA.ConnectLines();
		IShape anShapeB=theOtherShape.GetShape();
		anShapeB.ConnectLines();
		//Variables
		std::vector<sf::Vector2f> anAxes;
		Uint32 anIndex;
		sf::Vector2f anSmallestAxis;
		double anOverlap=std::numeric_limits<double>::max();
		//Axes for this object.
		for(anIndex=0;anIndex<anShapeA.mLines.size();++anIndex)
		{
			anAxes.push_back(NormalizeVector(anShapeA.mLines[anIndex].LineNormal()));
		}

		//Axes for other object.
		for(anIndex=0;anIndex<anShapeB.mLines.size();++anIndex)
		{
			anAxes.push_back(NormalizeVector(anShapeB.mLines[anIndex].LineNormal()));
		}
		for(anIndex=0;anIndex<anAxes.size();++anIndex)
		{
			float anMinA, anMaxA, anMinB, anMaxB;
			// ... project the points of both OBBs onto the axis ...
			anShapeA.ProjectOntoAxis(anAxes[anIndex], anMinA, anMaxA);
			anShapeB.ProjectOntoAxis(anAxes[anIndex], anMinB, anMaxB);
			// ... and check whether the outermost projected points of both OBBs overlap.
			// If this is not the case, the Seperating Axis Theorem states that there can be no collision between the rectangles
			if(!((anMinB<=anMaxA)&&(anMaxB>=anMinA)))
			{
				return false;
			}
			else
			{
				double o;
				if(anMinB<=anMaxA)
					o=anMaxA-anMinB;
				else
					o=anMaxB-anMinA;
				if(o<anOverlap)
				{
					anSmallestAxis=anAxes[anIndex];
					anOverlap=o;
				}
			}
		}
		theMinimumTranslation=anSmallestAxis;
		theMinimumTranslation=NormalizeVector(theMinimumTranslation);
		theMinimumTranslation*=(float)anOverlap;
		return true;
	}
	void IShape::ProjectOntoAxis (const sf::Vector2f& theAxis, float& theMin, float& theMax)
	{
		theMin = (mPoints[0].x*theAxis.x+mPoints[0].y*theAxis.y);
		theMax = theMin;
		for (int j = 1; j<mPoints.size(); j++)
		{
			float Projection = (mPoints[j].x*theAxis.x+mPoints[j].y*theAxis.y);

			if (Projection<theMin)
				theMin=Projection;
			if (Projection>theMax)
				theMax=Projection;
		}
	}
	void IShape::ConnectLines()
	{
		mLines.clear();
		for(int i=0;i<mPoints.size()-1;++i)
		{
			mLines.push_back(Line(&mPoints[i],&mPoints[i+1]));
		}
		mLines.push_back(Line(&mPoints[mPoints.size()-1],&mPoints[0]));
	}
}
