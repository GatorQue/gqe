#include <GQE/Core/utils/MathUtil.hpp>

namespace GQE
{
	void SetSeed(GQE::Uint32 theSeed)
	{
		srand(theSeed);
	}

	float Random(float theMin, float theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}
	int Random(int theMin, int theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}

	GQE::Uint32 Random(GQE::Uint32 theMin, GQE::Uint32 theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}
  sf::Vector2f NormalizeVector(sf::Vector2f theVector)
  {
    float anMagnitude=sqrt((theVector.x*theVector.x)+(theVector.y*theVector.y));
    sf::Vector2f anNormal=sf::Vector2f(theVector.x/anMagnitude,theVector.y/anMagnitude);
    return anNormal;
  }
  float dotProduct(sf::Vector2f theVectorA, sf::Vector2f theVectorB)
  {
    return ((theVectorA.x*theVectorB.x)+(theVectorA.y*theVectorB.y));
  }

  float ToRadians(float theDegrees)
  {
    return theDegrees*PI / 180;
  }
  float ToDegrees(float theRadians)
  {
    return theRadians * 180 / PI;
  }
}
